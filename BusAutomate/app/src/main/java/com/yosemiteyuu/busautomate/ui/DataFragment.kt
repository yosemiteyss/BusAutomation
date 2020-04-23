package com.yosemiteyuu.busautomate.ui

import android.os.Bundle
import android.view.View
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.yosemiteyuu.busautomate.R
import com.yosemiteyuu.busautomate.network.Resource
import kotlinx.android.synthetic.main.fragment_data.*

class DataFragment : Fragment(R.layout.fragment_data) {

    private lateinit var mainViewModel: MainViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mainViewModel = ViewModelProvider(requireActivity())
            .get(MainViewModel::class.java)

        mainViewModel.getBusData()
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        mainViewModel.busData.observe(viewLifecycleOwner, Observer {
            if (it.status == Resource.Status.SUCCESS) {
                it.data?.let { busData ->
                    temp_textView.text = busData.temp.toString()
                    humidity_textView.text = busData.humidity.toString()
                    light_textView.text = busData.light.toString()
                    seats_textView.text = busData.seats.toString()

                    seat1_view.setBackgroundColor(getSeatColor(busData.seat1))
                    seat2_view.setBackgroundColor(getSeatColor(busData.seat2))
                    seat3_view.setBackgroundColor(getSeatColor(busData.seat3))
                }
            }
        })
    }

    private fun getSeatColor(available: Int?): Int {
        return if (available == 1)
            mainViewModel.greenColor else mainViewModel.redColor
    }

    companion object {
        @JvmStatic
        fun newInstance() = DataFragment()
    }
}