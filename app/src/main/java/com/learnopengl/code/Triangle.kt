package com.learnopengl.code

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import javax.microedition.khronos.opengles.GL10

class Triangle {
    private var vertexBuffer: FloatBuffer? = null // Buffer for vertex-array
    private var colorBuffer: FloatBuffer? = null // Buffer for color-array
    private var indexBuffer: ByteBuffer? = null // Buffer for index-array
    private val vertices = floatArrayOf( // Vertices of the triangle
        0.0f, 1.0f, 0.0f,  // 0. top
        -1.0f, -1.0f, 0.0f,  // 1. left-bottom
        1.0f, -1.0f, 0.0f // 2. right-bottom
    )
    private val indices = byteArrayOf(0, 1, 2) // Indices to above vertices (in CCW)
    private val colors = floatArrayOf( // Colors for the vertices (RGBA)
        1.0f, 0.0f, 0.0f, 1.0f, // Red
        0.0f, 1.0f, 0.0f, 1.0f, // Green
        0.0f, 0.0f, 1.0f, 1.0f  // Blue
    )

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

        // Setup color-array buffer. Colors in float. A float has 4 bytes
        val cbb = ByteBuffer.allocateDirect(colors.size * 4)
        cbb.order(ByteOrder.nativeOrder()) // Use native byte order
        colorBuffer = cbb.asFloatBuffer() // Convert byte buffer to float
        colorBuffer?.put(colors) // Copy data into buffer
        colorBuffer?.position(0) // Rewind

        // Setup index-array buffer. Indices in byte.
        indexBuffer = ByteBuffer.allocateDirect(indices.size)
        indexBuffer?.put(indices)
        indexBuffer?.position(0)
    }

    // Render this shape
    fun draw(gl: GL10) {
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY) // Enable vertex-array and define the buffers
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer) // Specify the location of the buffers
        // gl*Pointer(int size, int type, int stride, Buffer pointer)
        //   size: number of coordinates per vertex (must be 2, 3, or 4).
        //   type: data type of vertex coordinate, GL_BYTE, GL_SHORT, GL_FIXED, or GL_FLOAT
        //   stride: the byte offset between consecutive vertices. 0 for tightly packed.

        gl.glEnableClientState(GL10.GL_COLOR_ARRAY) // Enable color-array
        gl.glColorPointer(4, GL10.GL_FLOAT, 0, colorBuffer) // Define color-array buffer

        // Draw the primitives via index-array, uses the index array to reference the vertex and color arrays
        gl.glDrawElements(GL10.GL_TRIANGLES, indices.size, GL10.GL_UNSIGNED_BYTE, indexBuffer)
        // glDrawElements(int mode, int count, int type, Buffer indices)
        //   mode: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, or GL_TRIANGLES
        //   count: the number of elements to be rendered.
        //   type: data-type of indices (must be GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT).
        //   indices: pointer to the index array.

        gl.glDisableClientState(GL10.GL_VERTEX_ARRAY) // Disable vertex-array and define the buffers
        gl.glDisableClientState(GL10.GL_COLOR_ARRAY) // Disable color-array
    }
}