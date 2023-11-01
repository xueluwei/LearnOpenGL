package com.learnopengl.nativecode

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet


class NativeGLSurfaceView @JvmOverloads constructor(
    context: Context,
    attributeSet: AttributeSet? = null
): GLSurfaceView(context, attributeSet) {

    init {
        setEGLContextFactory(ContextFactory())
        setEGLConfigChooser(ConfigChooser())
        setRenderer(NativeRender())
    }

}