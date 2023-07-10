package com.learnopengl.code

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import javax.microedition.khronos.opengles.GL10

class Square {
    private var vertexBuffer: FloatBuffer? = null // Buffer for vertex-array
    private val vertices = floatArrayOf( // Vertices of the triangle
        -1.0f, -1.0f,  0.0f,  // 0. left-bottom
        1.0f, -1.0f,  0.0f,  // 1. right-bottom
        -1.0f,  1.0f,  0.0f,  // 2. left-top
        1.0f,  1.0f,  0.0f   // 3. right-top
    )
    private val indices = byteArrayOf(0, 1, 2) // Indices to above vertices (in CCW)

    init {
        // Setup vertex-array buffer. Vertices in float. A float has 4 bytes.
        // Allocate a raw byte buffer. A float has 4 bytes
        val vbb = ByteBuffer.allocateDirect(vertices.size * 4)
        // Need to use native byte order
        vbb.order(ByteOrder.nativeOrder())
        // Convert the byte buffer into float buffer
        vertexBuffer = vbb.asFloatBuffer()
        // Transfer the data into the buffer
        vertexBuffer?.put(vertices)
        // Rewind
        vertexBuffer?.position(0)
    }

    // Render this shape
    fun draw(gl: GL10) {
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY)
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer)

        gl.glColor4f(0.5f, 0.5f, 1.0f, 1.0f) // Set the current color

        // Draw the primitives from the vertex-array directly
        gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, vertices.size / 3)

        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY)
    }
}