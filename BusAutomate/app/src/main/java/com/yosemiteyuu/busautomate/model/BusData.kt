package com.yosemiteyuu.busautomate.model

import com.google.gson.annotations.Expose
import com.google.gson.annotations.SerializedName

data class BusData (

    @SerializedName("temp")
    @Expose
    val temp: Double? = null,

    @SerializedName("humidity")
    @Expose
    val humidity: Double? = null,

    @SerializedName("light")
    @Expose
    val light: Int? = null,

    @SerializedName("seats")
    @Expose
    val seats: Int? = null,

    @SerializedName("seat1")
    @Expose
    val seat1: Int? = null,

    @SerializedName("seat2")
    @Expose
    val seat2: Int? = null,

    @SerializedName("seat3")
    @Expose
    val seat3: Int? = null,

    @SerializedName("lat")
    @Expose
    val latitude: Double? = null,

    @SerializedName("lon")
    @Expose
    val longitude: Double? = null

)