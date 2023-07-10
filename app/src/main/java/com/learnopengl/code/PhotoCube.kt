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


class PhotoCube(context: Context) {
    private var vertexBuffer // Vertex Buffer
            : FloatBuffer? = null
    private var texBuffer // Texture Coords Buffer
            : FloatBuffer? = null

    private val numFaces = 6
    private val imageFileIDs = intArrayOf( // Image file IDs
        R.drawable.img,
        R.drawable.img1,
        R.drawable.img2,
        R.drawable.img3,
        R.drawable.img4,
        R.drawable.img5
    )
    private val textureIDs = IntArray(numFaces)
    private val bitmap = arrayOfNulls<Bitmap>(numFaces)
    private val cubeHalfSize = 1.2f

    // Constructor - Set up the vertex buffer
    init {
        // Allocate vertex buffer. An float has 4 bytes
        val vbb: ByteBuffer = ByteBuffer.allocateDirect(12 * 4 * numFaces)
        vbb.order(ByteOrder.nativeOrder())
        vertexBuffer = vbb.asFloatBuffer()

        // Read images. Find the aspect ratio and adjust the vertices accordingly.
        for (face in 0 until numFaces) {
            bitmap[face] = drawableToBitmap(ResourcesCompat.getDrawable(context.resources, imageFileIDs[face], null)!!)
            val imgWidth = bitmap[face]?.width ?: 256
            val imgHeight = bitmap[face]?.height ?: 256
            var faceWidth = 2.0f
            var faceHeight = 2.0f
            // Adjust for aspect ratio
            if (imgWidth > imgHeight) {
                faceHeight = faceHeight * imgHeight / imgWidth
            } else {
                faceWidth = faceWidth * imgWidth / imgHeight
            }
            val faceLeft = -faceWidth / 2
            val faceRight = -faceLeft
            val faceTop = faceHeight / 2
            val faceBottom = -faceTop

            // Define the vertices for this face
            val vertices = floatArrayOf(
                faceLeft, faceBottom, 0.0f,  // 0. left-bottom-front
                faceRight, faceBottom, 0.0f,  // 1. right-bottom-front
                faceLeft, faceTop, 0.0f,  // 2. left-top-front
                faceRight, faceTop, 0.0f
            )
            vertexBuffer?.put(vertices) // Populate
        }
        vertexBuffer?.position(0) // Rewind

        // Allocate texture buffer. An float has 4 bytes. Repeat for 6 faces.
        val texCoords = floatArrayOf(
            0.0f, 1.0f,  // A. left-bottom
            1.0f, 1.0f,  // B. right-bottom
            0.0f, 0.0f,  // C. left-top
            1.0f, 0.0f // D. right-top
        )
        val tbb: ByteBuffer = ByteBuffer.allocateDirect(texCoords.size * 4 * numFaces)
        tbb.order(ByteOrder.nativeOrder())
        texBuffer = tbb.asFloatBuffer()
        for (face in 0 until numFaces) {
            texBuffer?.put(texCoords)
        }
        texBuffer?.position(0) // Rewind
    }

    // Render the shape
    fun draw(gl: GL10) {
        gl.glFrontFace(GL10.GL_CCW)
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY)
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer)
        gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, texBuffer)

        // front
        gl.glPushMatrix()
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[0])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4)
        gl.glPopMatrix()

        // left
        gl.glPushMatrix()
        gl.glRotatef(270.0f, 0f, 1f, 0f)
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[1])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 4, 4)
        gl.glPopMatrix()

        // back
        gl.glPushMatrix()
        gl.glRotatef(180.0f, 0f, 1f, 0f)
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[2])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 8, 4)
        gl.glPopMatrix()

        // right
        gl.glPushMatrix()
        gl.glRotatef(90.0f, 0f, 1f, 0f)
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[3])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 12, 4)
        gl.glPopMatrix()

        // top
        gl.glPushMatrix()
        gl.glRotatef(270.0f, 1f, 0f, 0f)
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[4])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 16, 4)
        gl.glPopMatrix()

        // bottom
        gl.glPushMatrix()
        gl.glRotatef(90.0f, 1f, 0f, 0f)
        gl.glTranslatef(0f, 0f, cubeHalfSize)
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[5])
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 20, 4)
        gl.glPopMatrix()
        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY)
    }

    // Load images into 6 GL textures
    fun loadTexture(gl: GL10) {
        gl.glGenTextures(6, textureIDs, 0) // Generate texture-ID array for 6 IDs

        // Generate OpenGL texture images
        for (face in 0 until numFaces) {
            gl.glBindTexture(GL10.GL_TEXTURE_2D, textureIDs[face])
            gl.glTexParameterf(
                GL10.GL_TEXTURE_2D,
                GL10.GL_TEXTURE_MIN_FILTER,
                GL10.GL_NEAREST.toFloat()
            )
            gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR.toFloat())
            GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap[face], 0)
            bitmap[face]!!.recycle()
        }
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