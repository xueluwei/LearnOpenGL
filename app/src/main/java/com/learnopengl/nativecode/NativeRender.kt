package com.learnopengl.nativecode

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class NativeRender: GLSurfaceView.Renderer {

    companion object {
        init {
            // 加载C库
            System.loadLibrary("Native")
        }
    }

    external fun init(width: Int, height: Int)

    external fun setup()

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        init(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        setup()
    }
}