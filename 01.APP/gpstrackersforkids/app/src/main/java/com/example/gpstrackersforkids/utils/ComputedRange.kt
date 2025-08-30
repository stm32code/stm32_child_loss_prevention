package com.example.gpstrackersforkids.utils
import kotlin.math.*
import java.lang.Math.PI
import java.lang.Math.sqrt
object ComputedRange {
    fun haversine(location1: Location, location2: Location): Double {
        val R = 6371 // 地球半径，单位是千米
        val lat1 = location1.lat.toRadians()
        val lat2 = location2.lat.toRadians()
        val dLat = (location2.lat - location1.lat).toRadians()
        val dLon = (location2.lon - location1.lon).toRadians()

        val a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1) * cos(lat2) *
                sin(dLon / 2) * sin(dLon / 2)
        val c = 2 * atan2(sqrt(a), sqrt(1 - a))
        return R * c * 1000 //将千米转化为米
    }

    fun Double.toRadians(): Double {
        return this * PI / 180
    }
}