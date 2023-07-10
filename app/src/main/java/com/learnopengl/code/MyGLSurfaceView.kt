package com.learnopengl.code

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.KeyEvent
import android.view.MotionEvent




class MyGLSurfaceView(context: Context): GLSurfaceView(context) {
    var renderer: TextureCubeRender? = null // Custom GL Renderer

    // For touch event
    private val TOUCH_SCALE_FACTOR = 180.0f / 320.0f
    private var previousX = 0f
    private var previousY = 0f

    // Constructor - Allocate and set the renderer
    init {
        renderer = TextureCubeRender(context)
        setRenderer(renderer!!)
        // Request focus, otherwise key/button won't react
        this.requestFocus()
        this.isFocusableInTouchMode = true
    }

    // Handler for key event
    override fun onKeyUp(keyCode: Int, evt: KeyEvent?): Boolean {
        renderer?.let {
            when (keyCode) {
                KeyEvent.KEYCODE_DPAD_LEFT -> it.speedY -= 0.1f
                KeyEvent.KEYCODE_DPAD_RIGHT -> it.speedY += 0.1f
                KeyEvent.KEYCODE_DPAD_UP -> it.speedX -= 0.1f
                KeyEvent.KEYCODE_DPAD_DOWN -> it.speedX += 0.1f
                KeyEvent.KEYCODE_A -> it.z -= 0.2f
                KeyEvent.KEYCODE_Z -> it.z += 0.2f
                KeyEvent.KEYCODE_DPAD_CENTER -> it.currentTextureFilter = (it.currentTextureFilter + 1) % 3  // Select texture filter (NEW)
                KeyEvent.KEYCODE_L -> it.lightingEnabled = !it.lightingEnabled  // Toggle lighting on/off (NEW)
                KeyEvent.KEYCODE_B -> it.blendingEnabled = !it.blendingEnabled // Toggle Blending on/off (NEW)
            }
        }
        return true // Event handled
    }

    // Handler for touch event
    override fun onTouchEvent(evt: MotionEvent): Boolean {
        renderer?.let {
            val currentX = evt.x
            val currentY = evt.y
            val deltaX: Float
            val deltaY: Float
            when (evt.action) {
                MotionEvent.ACTION_MOVE -> {
                    // Modify rotational angles according to movement
                    deltaX = currentX - previousX
                    deltaY = currentY - previousY
                    it.angleX += deltaY * TOUCH_SCALE_FACTOR
                    it.angleY += deltaX * TOUCH_SCALE_FACTOR
                }
            }
            // Save current x, y
            previousX = currentX
            previousY = currentY
        }
        return true // Event handled
    }
}