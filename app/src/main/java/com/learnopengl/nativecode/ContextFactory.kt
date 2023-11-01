package com.learnopengl.nativecode

import android.opengl.EGL14.EGL_CONTEXT_CLIENT_VERSION
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLContext
import javax.microedition.khronos.egl.EGLDisplay

/**
 * 用于创建和销毁EGLContext，指定OpenGL ES版本
 */
class ContextFactory: GLSurfaceView.EGLContextFactory {

    override fun createContext(egl: EGL10, display: EGLDisplay?, config: EGLConfig?): EGLContext? {
        val attrList = intArrayOf(
            EGL_CONTEXT_CLIENT_VERSION, 2, // （OpenGL ES 版本）2.0
            EGL10.EGL_NONE // EGL10.EGL_NONE表示数组结束
        )
        return egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attrList)
    }



    override fun destroyContext(egl: EGL10, display: EGLDisplay?, context: EGLContext?) {
        egl.eglDestroyContext(display,  context)
    }
}