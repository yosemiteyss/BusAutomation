package com.yosemiteyuu.busautomate.ui

import android.app.Application
import androidx.core.content.ContextCompat
import androidx.lifecycle.*
import com.yosemiteyuu.busautomate.R
import com.yosemiteyuu.busautomate.model.BusData
import com.yosemiteyuu.busautomate.network.BusDataApi
import com.yosemiteyuu.busautomate.network.Resource
import kotlinx.coroutines.*
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

class MainViewModel(application: Application) : AndroidViewModel(application) {

    val redColor = ContextCompat.getColor(application.applicationContext,
        R.color.colorAccent
    )

    val greenColor = ContextCompat.getColor(application.applicationContext,
        R.color.colorPrimary
    )

    private val _busData = MutableLiveData<Resource<BusData>>()
    val busData: LiveData<Resource<BusData>>
        get() = _busData

    var fetchJob: Job? = null

    private val exceptionHandler = CoroutineExceptionHandler { _, throwable ->
        _busData.postValue(
            Resource.error(
                BusData(
                    null,
                    null,
                    null,
                    null
                ),
                "Failed to get data: $throwable"))
    }

    fun getBusData() {
        fetchJob?.cancel()

        fetchJob = viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
            while (true) {
                _busData.postValue(Resource.success(busDataApi?.getBusData()))
                delay(fetchDelay)
            }
        }
    }

    companion object {
        private const val fetchDelay = 2000L
        private val busDataApi: BusDataApi? = Retrofit.Builder()
            .baseUrl("http://192.168.1.41:3000/")
            .addConverterFactory(GsonConverterFactory.create())
            .build()
            .create(BusDataApi::class.java)
    }
}