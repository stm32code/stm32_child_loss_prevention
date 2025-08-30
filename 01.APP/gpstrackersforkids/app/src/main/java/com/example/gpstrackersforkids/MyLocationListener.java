package com.example.gpstrackersforkids;

import android.content.Context;
import android.util.Log;

import com.baidu.location.BDAbstractLocationListener;
import com.baidu.location.BDLocation;
import com.baidu.mapapi.map.MapStatus;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.model.LatLng;

public class MyLocationListener extends BDAbstractLocationListener {
    private MapView mMapView;
    private Context mContext;
    private boolean isFirst;
    private LatLng nowLocationLatlng = new LatLng(39.923411, 116.403236);

    public MyLocationListener(MapView mMapView, Context mContext, boolean isFirst) {
        this.mMapView = mMapView;
        this.mContext = mContext;
        this.isFirst = isFirst;
    }

    @Override
    public void onReceiveLocation(BDLocation location) {
        //mapView 销毁后不在处理新接收的位置
        if (location == null || mMapView == null) {
            Log.e("定位监听", "location is null");
            return;
        } else {
//            Log.e("定位监听BDLocation", String.valueOf(location.getDirection()));
        }
        MyLocationData locData = new MyLocationData.Builder()
                .accuracy(location.getRadius())
                // 此处设置开发者获取到的方向信息，顺时针0-360
                .direction(location.getDirection()).latitude(location.getLatitude())
                .longitude(location.getLongitude()).build();
        mMapView.getMap().setMyLocationData(locData);

        // 第一次定位视角会直接跳转到定位的位置
        if (isFirst) {
            isFirst = false;
            LatLng ll = new LatLng(location.getLatitude(), location.getLongitude());
            MapStatus.Builder builder = new MapStatus.Builder();
            builder.target(ll).zoom(19f);
            mMapView.getMap().animateMapStatus(MapStatusUpdateFactory.newMapStatus(builder.build()));
            nowLocationLatlng = new LatLng(location.getLatitude(), location.getLongitude());
        }

    }

    public boolean isFirst() {
        return isFirst;
    }

    public void setFirst(boolean first) {
        isFirst = first;
    }

    public LatLng getNowLocationLatlng() {
        return nowLocationLatlng;
    }

    public void setNowLocationLatlng(LatLng nowLocationLatlng) {
        this.nowLocationLatlng = nowLocationLatlng;
    }
}