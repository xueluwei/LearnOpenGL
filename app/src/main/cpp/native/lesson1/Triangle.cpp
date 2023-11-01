/**
 * 在开始前，我们先讨论下OpenGL的简单绘制流程，这样有助于理解后面的代码。
 * 初始化流程：
 *    - 首先，在窗口初始化时，我们需要创建着色程序，着色程序是一组着色器的集合，着色器是运行在GPU上的程序，由相应着色器源码加载，用于渲染图形。
 *    - 然后，我们获取一系列变量，例如通过着色程序获取顶点着色器的vPosition变量，这些变量会在渲染时使用。
 *    - 最后，我们需要设置视口，视口类似于摄像机，它决定了渲染的图形在屏幕上的位置。
 * 绘制流程：
 *    - 首先，我们需要清除颜色缓冲区和深度缓冲区，这样才能绘制新的图形。
 *    - 然后，我们需要选择要使用的着色器程序，应用可以获取多个着色器程序。
 *    - 接着，我们需要通过预定义好的坐标来设置顶点坐标，这些坐标会在渲染时绘制图形。
 *    - 最后，我们需要启用顶点坐标，并绘制图形。
 */

#include <jni.h>

// EGL相应库
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <cstdlib>
#include "../include/LoadUtil.h"
#include "../include/LogUtil.h"

// 顶点着色器源码
static const char glVertexShader[] =
        "attribute vec4 vPosition;\n" // 顶点坐标，着色器的输入值，需要单独提供数据。对应每一个画到屏幕上的顶点。
        "void main()\n"
        "{\n"
        "  gl_Position = vPosition;\n" // 设置顶点坐标
        "}\n";

// 块着色器源码
static const char glFragmentShader[] =
        "precision mediump float;\n" // 必要的一行，用于设置精度，可选值：lowp, mediump, highp
        "void main()\n"
        "{\n"
        "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" // 设置颜色，4位代表RGBA，值域0.0~1.0，这个例子里表示红色
        "}\n";

GLuint simpleTriangleProgram; // 着色器程序，在setupGraphics中初始化，渲染时使用
GLuint vPosition; // 顶点着色器的vPosition变量，在setupGraphics中初始化，渲染时GPU的着色器需要使用

/**
 * 创建着色器程序并设置视口（类似架起摄像机），在渲染器初始化的时候调用
 * @param w
 * @param h
 * @return
 */
bool setupGraphics(int w, int h)
{
    simpleTriangleProgram = createProgram(glVertexShader, glFragmentShader); // 创建好着色器程序
    if (!simpleTriangleProgram) // 确保创建成功
    {
        LOGE ("Could not create program"); // 打印错误信息
        return false;
    }
    vPosition = glGetAttribLocation(simpleTriangleProgram, "vPosition"); // 通过OpenGL ES获取顶点着色器的vPosition变量
    glViewport(0, 0, w, h); // 设置视口
    return true;
}

// 三角形顶点坐标，范围是[-1.0，1.0],因为三角是2D图形，所以无需考虑z轴，每两个值代表一个顶点的x和y坐标，用于渲染时绘制三角形
const GLfloat triangleVertices[] = {
        0.0f, 1.0f, // 三角形顶部
        -1.0f, -1.0f, // 三角形左下角
        1.0f, -1.0f // 三角形右下角
};

/**
 * 用于绘制渲染三角形
 */
void renderFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置背景颜色为黑色
    glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区和深度缓冲区
    glUseProgram(simpleTriangleProgram); // 选择要使用的着色器程序，应用可以获取多个着色器程序。
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0 ,triangleVertices); // 设置顶点坐标
    glEnableVertexAttribArray(vPosition); // 启用顶点坐标
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
}