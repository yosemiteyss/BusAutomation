package com.yosemiteyuu.busautomate.network

class Resource<T>(
    val status: Status,
    val data: T?,
    val message: String?) {

    companion object {
        @JvmStatic
        fun <T> success(data: T?): Resource<T> =
            Resource(Status.SUCCESS, data, null)

        @JvmStatic
        fun <T> error(data: T?, message: String?): Resource<T> =
            Resource(Status.ERROR, data, message)

        @JvmStatic
        fun <T> loading(data: T?): Resource<T> =
            Resource(Status.LOADING, data, null)
    }

    enum class Status {
        SUCCESS, ERROR, LOADING
    }
}