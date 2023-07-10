package com.learnopengl.code

import android.content.Context
import android.opengl.GLSurfaceView
import android.opengl.GLU
import com.learnopengl.code.Cube
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10


class MyGLRender(private val context: Context): GLSurfaceView.Renderer {

    private val triangle = Triangle()
    private val square = Square()
    private val cube = Cube()
    private val pyramid = Pyramid()
    private val photoCube = PhotoCube(context)

    // Rotational angle and speed
    private var angleTriangle = 0.0f
    private var angleQuad = 0.0f
    private val speedTriangle = 0.5f
    private val speedQuad = -0.4f
    private var anglePyramid = 0f
    private var angleCube = 0f
    private val speedPyramid = 2f
    private val speedCube = -1.5f

    // For controlling photoCube's z-position, x and y angles and speeds
    var angleX = 0f // (NEW)
    var angleY = 0f // (NEW)
    var speedX = 0f // (NEW)
    var speedY = 0f // (NEW)
    var z = -6.0f // (NEW)

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

        // Setup Texture, each time the surface is created
        cube.loadTexture(gl, context)    // Load image into Texture
        gl.glEnable(GL10.GL_TEXTURE_2D)  // Enable texture

        photoCube.loadTexture(gl)
        gl.glEnable(GL10.GL_TEXTURE_2D)
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

        gl.glLoadIdentity() // Reset model-view matrix
        gl.glTranslatef(-5f, 0f, -6.0f) // Translate left and into the screen
        gl.glRotatef(angleTriangle, 0.0f, 1.0f, 0.0f) // Rotate the triangle about the y-axis
        triangle.draw(gl)

        gl.glLoadIdentity() // Reset the mode-view matrix
        gl.glTranslatef(-3.5f, 0.0f, -6.0f) // Translate right and into the screen
        gl.glRotatef(angleQuad, 1.0f, 0.0f, 0.0f) // Rotate the square about the x-axis
        square.draw(gl)

        gl.glLoadIdentity() // Reset the mode-view matrix
        gl.glTranslatef(-0.5f, 0.0f, -6.0f) // Translate right and into the screen
        gl.glRotatef(anglePyramid, 0.1f, 1.0f, -0.1f) // Rotate
        pyramid.draw(gl)  // Draw the pyramid

        gl.glLoadIdentity() // Reset the model-view matrix
        gl.glTranslatef(1.5f, 0.0f, -6.0f) // Translate right and into the screen
        gl.glScalef(0.8f, 0.8f, 0.8f) // Scale down
        gl.glRotatef(angleCube, 1.0f, 1.0f, 1.0f) // rotate about the axis (1,1,1)
        cube.draw(gl) // Draw the cube

        // ----- Render the Cube -----
        gl.glLoadIdentity();              // Reset the model-view matrix
        gl.glTranslatef(3.5f, 0.0f, z);   // Translate into the screen (NEW)
        gl.glRotatef(angleX, 1.0f, 0.0f, 0.0f); // Rotate (NEW)
        gl.glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Rotate (NEW)
        photoCube.draw(gl);

        // Update the rotational angle after each refresh (NEW)
        angleX += speedX;  // (NEW)
        angleY += speedY;  // (NEW)

        // Update the rotational angle after each refresh
        angleTriangle += speedTriangle
        angleQuad += speedQuad
        anglePyramid += speedPyramid
        angleCube += speedCube
    }
}