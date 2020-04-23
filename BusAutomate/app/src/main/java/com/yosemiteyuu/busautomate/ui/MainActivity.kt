package com.yosemiteyuu.busautomate.ui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.google.android.material.tabs.TabLayout
import com.yosemiteyuu.busautomate.R
import com.yosemiteyuu.busautomate.network.Resource
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.fragment_data.view.*

class MainActivity : AppCompatActivity() {

    private lateinit var mainViewModel: MainViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mainViewModel = ViewModelProvider(this).get(MainViewModel::class.java)

        main_viewPager.adapter = MainPagerAdapter(supportFragmentManager)
        main_tabLayout.setupWithViewPager(main_viewPager)

        mainViewModel.busData.observe(this, Observer {
            when (it.status) {
                Resource.Status.LOADING -> { }
                Resource.Status.SUCCESS -> Toast.makeText(this, "Success", Toast.LENGTH_SHORT).show()
                Resource.Status.ERROR -> Toast.makeText(this, "Error: ${it.message}", Toast.LENGTH_SHORT).show()
            }
        })
    }

    override fun onResume() {
        super.onResume()
        if (mainViewModel.fetchJob?.isActive == false) {
            mainViewModel.getBusData()
        }
    }
}
