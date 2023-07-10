package com.learnopengl.code

import android.content.Context
import android.opengl.GLSurfaceView
import android.opengl.GLU
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TextureCubeRender(private val context: Context): GLSurfaceView.Renderer {
    var currentTextureFilter = 0 // Texture filter (NEW)

    private val textureCube = TextureCube()

    var angleX = 0f
    var angleY = 0f
    var speedX = 0f
    var speedY = 0f
    var z = -6.0f

    // Lighting (NEW)
    var lightingEnabled = false // Is lighting on? (NEW)

    // Blending (NEW)
    var blendingEnabled = false // Is blending on? (NEW)

    private val lightAmbient = floatArrayOf(0.5f, 0.5f, 0.5f, 1.0f)
    private val lightDiffuse = floatArrayOf(1.0f, 1.0f, 1.0f, 1.0f)
    private val lightPosition = floatArrayOf(0.0f, 0.0f, 2.0f, 1.0f)

    // Called when the surface is first created or recreated.
    // It can be used to perform one-time initialization tasks
    // such as setting the clear-value for color and depth, enabling depth-test, etc.
    override fun onSurfaceCreated(gl: GL10, p1: EGLConfig) {
        gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f)  // Set color's clear-value to black
        gl.glClearDepthf(1.0f)            // Set depth's clear-value to farthest
        gl.glEnable(GL10.GL_DEPTH_TEST)   // Enables depth-buffer for hidden surface removal
        gl.glDepthFunc(GL10.GL_LEQUAL)    // The type of depth testing to do
        gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_NICEST)  // nice perspective view
        gl.glShadeModel(GL10.GL_SMOOTH)   // Enable smooth shading of color
        gl.glDisable(GL10.GL_DITHER)      // Disable dithering for better performance

        textureCube.loadTexture(gl, context)
        gl.glEnable(GL10.GL_TEXTURE_2D)


        // Setup lighting GL_LIGHT1 with ambient and diffuse lights (NEW)
        gl.glLightfv(GL10.GL_LIGHT1, GL10.GL_AMBIENT, lightAmbient, 0);
        gl.glLightfv(GL10.GL_LIGHT1, GL10.GL_DIFFUSE, lightDiffuse, 0);
        gl.glLightfv(GL10.GL_LIGHT1, GL10.GL_POSITION, lightPosition, 0);
        gl.glEnable(GL10.GL_LIGHT1);   // Enable Light 1 (NEW)
        gl.glEnable(GL10.GL_LIGHT0);   // Enable the default Light 0 (NEW)
        // Setup Blending (NEW)
        gl.glColor4f(1.0f, 1.0f, 1.0f, 0.5f) // Full brightness, 50% alpha (NEW)
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE); // Select blending function (NEW)
    }

    // Called when the surface is first displayed and after window's size changes.
    // It is used to set the view port and projection mode.
    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        var mHeight = height
        if (mHeight == 0) mHeight = 1 // To prevent divide by zero

        val aspect: Float = width / mHeight.toFloat()

        // Set the viewport (display area) to cover the entire window
        gl.glViewport(0, 0, width, height)

        // OpenGL uses two transformation matrices: projection matrix and model-view matrix
        // We select the projection matrix to setup the projection
        gl.glMatrixMode(GL10.GL_PROJECTION) // Select projection matrix
        gl.glLoadIdentity() // Reset projection matrix

        // Use perspective projection with the projection volume defined by
        //   fovy, aspect-ration, z-near and z-far
        GLU.gluPerspective(gl, 45f, aspect, 0.1f, 100f)

        // Select the model-view matrix to manipulate objects (Deselect the projection matrix)
        gl.glMatrixMode(GL10.GL_MODELVIEW) // Select model-view matrix
        gl.glLoadIdentity() // Reset
    }

    override fun onDrawFrame(gl: GL10) {
        // Clear color and depth buffers using clear-value set earlier
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT or GL10.GL_DEPTH_BUFFER_BIT)

        // Enable lighting? (NEW)
        if (lightingEnabled) {
            gl.glEnable(GL10.GL_LIGHTING);
        } else {
            gl.glDisable(GL10.GL_LIGHTING);
        }

        // Blending Enabled? (NEW)
        if (blendingEnabled) {
            gl.glEnable(GL10.GL_BLEND);       // Turn blending on (NEW)
            gl.glDisable(GL10.GL_DEPTH_TEST); // Turn depth testing off (NEW)

        } else {
            gl.glDisable(GL10.GL_BLEND);      // Turn blending off (NEW)
            gl.glEnable(GL10.GL_DEPTH_TEST);  // Turn depth testing on (NEW)
        }

        gl.glLoadIdentity()              // Reset the model-view matrix
        gl.glTranslatef(0.0f, 0.0f, z)   // Translate into the screen
        gl.glRotatef(angleX, 1.0f, 0.0f, 0.0f) // Rotate
        gl.glRotatef(angleY, 0.0f, 1.0f, 0.0f) // Rotate
        textureCube.draw(gl, currentTextureFilter)

        // Update the rotational angle after each refresh
        angleX += speedX
        angleY += speedY
    }
}