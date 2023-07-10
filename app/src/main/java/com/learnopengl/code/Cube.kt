package com.learnopengl.code

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.BitmapDrawable
import android.graphics.drawable.Drawable
import android.opengl.GLUtils
import androidx.core.content.res.ResourcesCompat
import com.learnopengl.R
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import javax.microedition.khronos.opengles.GL10


class Cube {
    private var vertexBuffer: FloatBuffer? = null // Buffer for vertex-array
    private var texBuffer: FloatBuffer? = null // Buffer for texture-coords-array

    private val numFaces = 6

    private val colors = arrayOf(
        floatArrayOf(1.0f, 0.5f, 0.0f, 1.0f),
        floatArrayOf(1.0f, 0.0f, 1.0f, 1.0f),
        floatArrayOf(0.0f, 1.0f, 0.0f, 1.0f),
        floatArrayOf(0.0f, 0.0f, 1.0f, 1.0f),
        floatArrayOf(1.0f, 0.0f, 0.0f, 1.0f),
        floatArrayOf(1.0f, 1.0f, 0.0f, 1.0f)
    )

    private val vertices = floatArrayOf( // Vertices of the 6 faces
        // FRONT
        -1.0f, -1.0f, 1.0f,  // 0. left-bottom-front
        1.0f, -1.0f, 1.0f,  // 1. right-bottom-front
        -1.0f, 1.0f, 1.0f,  // 2. left-top-front
        1.0f, 1.0f, 1.0f,  // 3. right-top-front
        // BACK
        1.0f, -1.0f, -1.0f,  // 6. right-bottom-back
        -1.0f, -1.0f, -1.0f,  // 4. left-bottom-back
        1.0f, 1.0f, -1.0f,  // 7. right-top-back
        -1.0f, 1.0f, -1.0f,  // 5. left-top-back
        // LEFT
        -1.0f, -1.0f, -1.0f,  // 4. left-bottom-back
        -1.0f, -1.0f, 1.0f,  // 0. left-bottom-front
        -1.0f, 1.0f, -1.0f,  // 5. left-top-back
        -1.0f, 1.0f, 1.0f,  // 2. left-top-front
        // RIGHT
        1.0f, -1.0f, 1.0f,  // 1. right-bottom-front
        1.0f, -1.0f, -1.0f,  // 6. right-bottom-back
        1.0f, 1.0f, 1.0f,  // 3. right-top-front
        1.0f, 1.0f, -1.0f,  // 7. right-top-back
        // TOP
        -1.0f, 1.0f, 1.0f,  // 2. left-top-front
        1.0f, 1.0f, 1.0f,  // 3. right-top-front
        -1.0f, 1.0f, -1.0f,  // 5. left-top-back
        1.0f, 1.0f, -1.0f,  // 7. right-top-back
        // BOTTOM
        -1.0f, -1.0f, -1.0f,  // 4. left-bottom-back
        1.0f, -1.0f, -1.0f,  // 6. right-bottom-back
        -1.0f, -1.0f, 1.0f,  // 0. left-bottom-front
        1.0f, -1.0f, 1.0f // 1. right-bottom-front
    )


    var texCoords = floatArrayOf( // Texture coords for the above face
        0.0f, 1.0f,  // A. left-bottom
        1.0f, 1.0f,  // B. right-bottom
        0.0f, 0.0f,  // C. left-top
        1.0f, 0.0f // D. right-top
    )
    var textureIDs = IntArray(1) // Array for 1 texture-ID

    // Constructor - Set up the buffers
    init {
        // Setup vertex-array buffer. Vertices in float. An float has 4 bytes
        val vbb: ByteBuffer = ByteBuffer.allocateDirect(vertices.size * 4)
        vbb.order(ByteOrder.nativeOrder()) // Use native byte order
        vertexBuffer = vbb.asFloatBuffer() // Convert from byte to float
        vertexBuffer?.put(vertices) // Copy data into buffer
        vertexBuffer?.position(0) // Rewind

        // Setup texture-coords-array buffer, in float. An float has 4 bytes
        val tbb = ByteBuffer.allocateDirect(texCoords.size * 4)
        tbb.order(ByteOrder.nativeOrder())
        texBuffer = tbb.asFloatBuffer()
        texBuffer?.put(texCoords)
        texBuffer?.position(0)
    }

    // Draw the shape
    fun draw(gl: GL10) {
        gl.glFrontFace(GL10.GL_CCW) // Front face in counter-clockwise orientation
        gl.glEnable(GL10.GL_CULL_FACE) // Enable cull face
        gl.glCullFace(GL10.GL_BACK) // Cull the back face (don't display)
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer)

        gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY)  // Enable texture-coords-array
        gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, texBuffer) // Define texture-coords buffer

        // Render all the faces
        for (face in 0 until numFaces) {
            // Set the color for each of the faces
            gl.glColor4f(colors[face][0], colors[face][1], colors[face][2], colors[face][3])
            // Draw the primitive from the vertex-array directly
            gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, face * 4, 4)
        }

        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glDisable(GL10.GL_CULL_FACE)
    }

    // Load images into 6 GL textures
    fun loadTexture(gl: GL10, context: Context) {
        gl.glGenTextures(1, textureIDs, 0) // Generate texture-ID array
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[0]) // Bind to texture ID
        // Set up texture filters
        gl.glTexParameterf(
            GL10.GL_TEXTURE_2D,
            GL10.GL_TEXTURE_MIN_FILTER,
            GL10.GL_NEAREST.toFloat()
        )
        gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR.toFloat())

        // Construct an input stream to texture image
        val bitmap: Bitmap = drawableToBitmap(ResourcesCompat.getDrawable(context.resources, R.drawable.img, null)!!)

        // Build Texture from loaded bitmap for the currently-bind texture ID
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0)
        bitmap.recycle()
    }

    fun drawableToBitmap(drawable: Drawable): Bitmap {
        if (drawable is BitmapDrawable) {
            if (drawable.bitmap != null) {
                return drawable.bitmap
            }
        }
        val bitmap = if (drawable.intrinsicWidth <= 0 || drawable.intrinsicHeight <= 0) {
            Bitmap.createBitmap(1, 1, Bitmap.Config.ARGB_8888)
        } else {
            Bitmap.createBitmap(
                drawable.intrinsicWidth,
                drawable.intrinsicHeight,
                Bitmap.Config.ARGB_8888
            )
        }
        val canvas = Canvas(bitmap)
        drawable.setBounds(0, 0, canvas.width, canvas.height)
        drawable.draw(canvas)
        return bitmap
    }
}