package com.yosemiteyuu.busautomate.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.Marker
import com.google.android.gms.maps.model.MarkerOptions
import com.yosemiteyuu.busautomate.R
import com.yosemiteyuu.busautomate.network.Resource

class MapFragment : Fragment(), OnMapReadyCallback {

    private lateinit var mainViewModel: MainViewModel

    private var map: GoogleMap? = null
    private var markerOptions: MarkerOptions? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        activity?.let {
            mainViewModel = ViewModelProvider(it).get(MainViewModel::class.java)
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_map, container, false)

        val mapFragment = childFragmentManager.findFragmentById(R.id.map_fragment) as SupportMapFragment?
        mapFragment?.getMapAsync(this)

        mainViewModel.busData.observe(this, Observer {
            if (it != null) {
                if (it.status == Resource.Status.SUCCESS) {
                    if (it.data?.latitude != null && it.data.longitude != null) {
                        val myLocation = LatLng(it.data.latitude, it.data.longitude)

                        map?.clear()

                        map?.addMarker(MarkerOptions()
                            .position(myLocation)
                            .title("Me"))


                        map?.animateCamera(CameraUpdateFactory
                            .newLatLngZoom(myLocation, 15f))
                    }
                }
            }
        })

        return rootView
    }

    override fun onMapReady(p0: GoogleMap?) {
        map = p0
    }

    private fun updateMap(map: GoogleMap?) {


    }


    companion object {
        @JvmStatic
        fun newInstance() = MapFragment()
    }

}