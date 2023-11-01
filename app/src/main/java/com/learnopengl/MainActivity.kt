package com.learnopengl

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.learnopengl.nativecode.NativeGLSurfaceView

class MainActivity : AppCompatActivity() {
    private var openglView: NativeGLSurfaceView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        openglView = NativeGLSurfaceView(this)
        setContentView(openglView)
    }

    override fun onPause() {
        super.onPause()
        openglView?.onPause()
    }

    override fun onResume() {
        super.onResume()
        openglView?.onResume()
    }

}