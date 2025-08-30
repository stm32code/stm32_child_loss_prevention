package com.example.gpstrackersforkids

import android.content.SharedPreferences
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import com.example.gpstrackersforkids.bean.Pdu
import com.example.gpstrackersforkids.databinding.ActivitySetBinding
import com.example.gpstrackersforkids.utils.Common
import com.example.gpstrackersforkids.utils.Common.isSendPone
import com.example.gpstrackersforkids.utils.HandlerAction
import com.example.gpstrackersforkids.utils.MToast
import com.example.gpstrackersforkids.utils.PduConvert

import com.google.gson.Gson
import com.gyf.immersionbar.BarHide
import com.gyf.immersionbar.ImmersionBar

class SetActivity : AppCompatActivity(), HandlerAction {
    private lateinit var binding: ActivitySetBinding
    private var sendSuccess = false
    private lateinit var sharedPreferences: SharedPreferences
    private lateinit var editor: SharedPreferences.Editor
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySetBinding.inflate(layoutInflater)
        setContentView(binding.root)
        sharedPreferences = getSharedPreferences("phone", MODE_PRIVATE)
        editor = sharedPreferences.edit()
        initView()
    }

    private fun initView() {
        //设置共同沉浸式样式
        ImmersionBar.with(this).hideBar(BarHide.FLAG_HIDE_STATUS_BAR).init()
        val getPhone = sharedPreferences.getString("phone", null)
        if (getPhone != null)
            binding.phoneText.text = Editable.Factory.getInstance().newEditable(getPhone)
        binding.backBtn.setOnClickListener {
            finish()
        }
        binding.submitBtn.setOnClickListener {
            val phone = binding.phoneText.text.toString()
            val text1 = binding.warring1.text.toString()
            val text2 = binding.warring2.text.toString()
            if (phone.isNotEmpty() && text1.isNotEmpty() && text2.isNotEmpty()) {
                sendMessage(
                    Common.PushTopic,
                    Gson().toJson(Pdu(1, PduConvert.toPdt(phone, text1)!!))
                )
                postDelayed({
                    if (sendSuccess) {
                        sendMessage(
                            Common.PushTopic,
                            Gson().toJson(Pdu(2, PduConvert.toPdt(phone, text2)!!))
                        )
                        editor.putString("phone", phone)
                        editor.commit()
                    }
                }, 2500)
            } else {
                MToast.mToast(this, "主体信息不能为空！")
            }
        }
    }

    /**
     * 封装发送消息失败重发
     */
    private fun sendMessage(publishTopic: String, message: String, qos: Int = 1) {
        Common.mqtt!!.publish(publishTopic, message, qos)
        postDelayed({
            sendSuccess = if (Common.mqtt!!.sendError) {
                MToast.mToast(this, "发送失败，请重新发送")
                false
            } else {
                isSendPone = true
                true
            }
        }, 500)
    }
}