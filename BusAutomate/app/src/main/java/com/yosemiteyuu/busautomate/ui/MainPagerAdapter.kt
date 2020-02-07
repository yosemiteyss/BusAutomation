package com.yosemiteyuu.busautomate.ui

import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentPagerAdapter

class MainPagerAdapter(fragmentManager: FragmentManager) :
    FragmentPagerAdapter(fragmentManager, BEHAVIOR_RESUME_ONLY_CURRENT_FRAGMENT) {

    override fun getItem(position: Int) =
        when (position) {
            0 -> DataFragment.newInstance()

            else -> MapFragment.newInstance()
        }


    override fun getCount() =
        PAGE_COUNT

    companion object {
        private const val PAGE_COUNT = 2
    }



}
