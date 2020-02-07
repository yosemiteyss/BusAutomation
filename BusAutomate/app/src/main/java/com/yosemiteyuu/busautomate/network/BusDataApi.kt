package com.yosemiteyuu.busautomate.network

import com.yosemiteyuu.busautomate.model.BusData
import retrofit2.http.GET

interface BusDataApi {

    @GET("data")
    suspend fun getBusData(): BusData

}