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
import javax.microedition.khronos.opengles.GL11


class TextureCube {
    private var vertexBuffer // Buffer for vertex-array
            : FloatBuffer? = null
    private var texBuffer // Buffer for texture-coords-array
            : FloatBuffer? = null

    private val vertices = floatArrayOf( // Vertices for a face
        -1.0f, -1.0f, 0.0f,  // 0. left-bottom-front
        1.0f, -1.0f, 0.0f,  // 1. right-bottom-front
        -1.0f, 1.0f, 0.0f,  // 2. left-top-front
        1.0f, 1.0f, 0.0f // 3. right-top-front
    )

    var texCoords = floatArrayOf( // Texture coords for the above face
        0.0f, 1.0f,  // A. left-bottom
        1.0f, 1.0f,  // B. right-bottom
        0.0f, 0.0f,  // C. left-top
        1.0f, 0.0f // D. right-top
    )
    var textureIDs = IntArray(3) // Array for 3 texture-IDs (NEW)


    // Constructor - Set up the buffers
    init {
        // Setup vertex-array buffer. Vertices in float. An float has 4 bytes
        val vbb: ByteBuffer = ByteBuffer.allocateDirect(vertices.size * 4)
        vbb.order(ByteOrder.nativeOrder()) // Use native byte order
        vertexBuffer = vbb.asFloatBuffer() // Convert from byte to float
        vertexBuffer?.put(vertices) // Copy data into buffer
        vertexBuffer?.position(0) // Rewind

        // Setup texture-coords-array buffer, in float. An float has 4 bytes
        val tbb: ByteBuffer = ByteBuffer.allocateDirect(texCoords.size * 4)
        tbb.order(ByteOrder.nativeOrder())
        texBuffer = tbb.asFloatBuffer()
        texBuffer?.put(texCoords)
        texBuffer?.position(0)
    }

    // Draw the shape
    fun draw(gl: GL10, textureFilter: Int) {  // Select the filter (NEW)
        gl.glFrontFace(GL10.GL_CCW) // Front face in counter-clockwise orientation
        gl.glEnable(GL10.GL_CULL_FACE) // Enable cull face
        gl.glCullFace(GL10.GL_BACK) // Cull the back face (don't display)
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer)
        gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY) // Enable texture-coords-array
        gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, texBuffer) // Define texture-coords buffer

        // Select the texture filter to use via texture ID (NEW)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[textureFilter])

        // front
        gl.glPushMatrix()
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // left
        gl.glPushMatrix()
        gl.glRotatef(270.0f, 0.0f, 1.0f, 0.0f)
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // back
        gl.glPushMatrix()
        gl.glRotatef(180.0f, 0.0f, 1.0f, 0.0f)
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // right
        gl.glPushMatrix()
        gl.glRotatef(90.0f, 0.0f, 1.0f, 0.0f)
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // top
        gl.glPushMatrix()
        gl.glRotatef(270.0f, 1.0f, 0.0f, 0.0f)
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // bottom
        gl.glPushMatrix()
        gl.glRotatef(90.0f, 1.0f, 0.0f, 0.0f)
        gl.glTranslatef(0.0f, 0.0f, 1.0f)
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()
        gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY)
        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glDisable(GL10.GL_CULL_FACE)
    }

    // Load an image and create 3 textures with different filters (NEW)
    fun loadTexture(gl: GL10, context: Context) {
        val bitmap = drawableToBitmap(ResourcesCompat.getDrawable(context.resources, R.drawable.img, null)!!)
        gl.glGenTextures(3, textureIDs, 0) // Generate texture-ID array for 3 textures (NEW)

        // Create Nearest Filtered Texture and bind it to texture 0 (NEW)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[0])
        gl.glTexParameterf(
            GL10.GL_TEXTURE_2D,
            GL10.GL_TEXTURE_MAG_FILTER,
            GL10.GL_NEAREST.toFloat()
        )
        gl.glTexParameterf(
            GL10.GL_TEXTURE_2D,
            GL10.GL_TEXTURE_MIN_FILTER,
            GL10.GL_NEAREST.toFloat()
        )
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0)

        // Create Linear Filtered Texture and bind it to texture 1 (NEW)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[1])
        gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR.toFloat())
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0)

        // Create mipmapped textures and bind it to texture 2 (NEW)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[2])
        gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR.toFloat())
        gl.glTexParameterf(
            GL10.GL_TEXTURE_2D,
            GL10.GL_TEXTURE_MIN_FILTER,
            GL10.GL_LINEAR_MIPMAP_NEAREST.toFloat()
        )
        (gl as? GL11)?.glTexParameterf(
            GL11.GL_TEXTURE_2D,
            GL11.GL_GENERATE_MIPMAP,
            GL11.GL_TRUE.toFloat()
        )
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