package com.example.gpstrackersforkids.utils


import com.example.gpstrackersforkids.bean.PositionData
import com.google.gson.Gson

object GsonConvert {

    fun stringToJson(str: String): PositionData {
        return Gson().fromJson(str, PositionData::class.java)
    }
}