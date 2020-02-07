package com.yosemiteyuu.busautomate.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.yosemiteyuu.busautomate.R
import com.yosemiteyuu.busautomate.network.Resource
import kotlinx.android.synthetic.main.fragment_data.view.*

class DataFragment : Fragment() {

    private lateinit var mainViewModel: MainViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        activity?.let {
            mainViewModel = ViewModelProvider(it).get(MainViewModel::class.java)
        }

        mainViewModel.getBusData()

    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_data, container, false)

        mainViewModel.busData.removeObservers(this)

        mainViewModel.busData.observe(this, Observer {
            if (it != null) {
                if (it.status == Resource.Status.SUCCESS) {
                    it.data?.let { busData ->
                        rootView.temp_textView.text = busData.temp.toString()
                        rootView.humidity_textView.text = busData.humidity.toString()
                        rootView.light_textView.text = busData.light.toString()
                        rootView.seats_textView.text = busData.seats.toString()

                        rootView.seat1_view.setBackgroundColor(
                            if (busData.seat1 == 1) mainViewModel.greenColor else mainViewModel.redColor)

                        rootView.seat2_view.setBackgroundColor(
                            if (busData.seat2 == 1) mainViewModel.greenColor else mainViewModel.redColor )

                        rootView.seat3_view.setBackgroundColor(
                            if (busData.seat3 == 1) mainViewModel.greenColor else mainViewModel.redColor )
                    }
                }
            }
        })

        return rootView
    }

    companion object {
        @JvmStatic
        fun newInstance() = DataFragment()
    }
}