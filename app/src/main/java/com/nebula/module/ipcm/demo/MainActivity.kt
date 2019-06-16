package com.nebula.module.ipcm.demo

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.nebula.module.ipcm.IPCM

/**
 * crated by nebula on 2019/06/01
 */
class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("ipcm")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        IPCM.init(this)
        val ipcm = IPCM.obtain()

    }
}
