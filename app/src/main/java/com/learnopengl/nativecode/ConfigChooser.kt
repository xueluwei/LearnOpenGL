package com.learnopengl.nativecode

import android.opengl.EGL14
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLDisplay

/**
 * 用于选择EGL配置
 */
class ConfigChooser: GLSurfaceView.EGLConfigChooser {
    companion object {
        // 使用32位记录，argb各占8位
        const val redSize = 8
        const val greenSize = 8
        const val blueSize = 8
        const val alphaSize = 8

        const val depthSize = 16 // 深度，用于设置位置
        const val sampleSize = 4 // 样品，用于消除锯齿(暂时无用)
        const val stencilSize = 0 // 模板
    }
    private val value = IntArray(1) // 用于获取属性值

    override fun chooseConfig(egl: EGL10, display: EGLDisplay): EGLConfig {
        // 设置配置列表
        val configAttributes = intArrayOf(
            EGL10.EGL_RED_SIZE, redSize,
            EGL10.EGL_GREEN_SIZE, greenSize,
            EGL10.EGL_BLUE_SIZE, blueSize,
            EGL10.EGL_ALPHA_SIZE, alphaSize,
            EGL10.EGL_DEPTH_SIZE, depthSize,
            EGL10.EGL_STENCIL_SIZE, stencilSize,
            EGL10.EGL_RENDERABLE_TYPE, EGL14.EGL_OPENGL_ES2_BIT, // 选择OpenGL ES 2.0
            EGL10.EGL_NONE // 表示属性列表结束
        )
        // 获取配置数量
        val numConfig = IntArray(1)
        egl.eglChooseConfig(
            display,
            configAttributes,
            null, // configs 属性为 null 时表示获取满足条件的配置数量
            0,
            numConfig
        )
        // 获取满足条件的配置列表
        val configsNumber = numConfig[0]

        val configs = arrayOfNulls<EGLConfig?>(configsNumber)
        egl.eglChooseConfig(display, configAttributes, configs, configsNumber, numConfig)
        return selectConfig(egl, display, configs) ?: throw IllegalArgumentException("No config chosen")
    }


    /**
     * EGL会给出比我们要求的配置更好的配置（ARGB-565可能会给ARGB-8888的配置），所以我们需要从中选择一个最适合的
     */
    private fun selectConfig(egl: EGL10, display: EGLDisplay, configs: Array<EGLConfig?>): EGLConfig? {
        for (config in configs) {
            val r: Int = getConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE)
            val g: Int = getConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE)
            val b: Int = getConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE)
            val a: Int = getConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE)
            val d: Int = getConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE)
            val s: Int = getConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE)
            if (r == redSize && g == greenSize && b == blueSize && a == alphaSize
                && d >= depthSize && s >= stencilSize) {
                return config
            }
        }
        return null
    }

    /**
     * 调用EGL方法获取配置属性
     */
    private fun getConfigAttrib(
        egl: EGL10,
        display: EGLDisplay,
        config: EGLConfig?,
        attribute: Int,
        defaultValue: Int = 0
    ): Int {
        return if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
            value[0]
        } else {
            defaultValue
        }
    }

}