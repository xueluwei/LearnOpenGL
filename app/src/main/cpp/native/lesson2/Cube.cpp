/**
 * --- 关于模型、视图、投影矩阵的讨论 ---
 *
 * 在考虑代码如何做动作之前，我们要理解究竟发生了什么。首先，所有转换是由矩阵完成的。矩阵是一些放到矩形表格中的数字。
 * 在三维坐标系下，我们使用四维矩阵来对三维坐标系下的物体进行动作操作。
 * 这样做的原因是因为高纬才能产生低维动作，低维相对于其坐标系本身是静止的。
 * 比如一条线，画好之后，如果坐标系不变，它不会自己动，除非你擦除重画他才会改变角度之类的。
 * 矩阵是个比较基础的数学概念，如果你不熟悉，建议看MIT的线性代数课程先了解一下。
 *
 * 在图形应用中我们使用三种主要的矩阵。它们被称作模型矩阵、视图矩阵和投影矩阵。以下是他们各自简介：
 *    - 模型矩阵：你可能还记得之前我们使用(0.0f, 1.0f)、(-1.0f, -1.0f)和(1.0f, -1.0f)的坐标画出的三角形，
 *      之后，我们把这些坐标画到屏幕的(0.0f, 0.0f, 0.0f)上，当你只有一个对象的时候，这样做没问题，但是如果
 *      你想创建另一个对象，它会立即覆盖掉之前的对象。模型矩阵就是这种情况的解决方法，而且模型矩阵简单解释了在屏幕的哪里
 *      画出对象。特别是当你对矩阵做移动、放大或旋转的时候，模型矩阵可以决定对象或模型的位置大小和旋转角度。
 *    - 视图矩阵：当你玩游戏时并不想让对象移动，比如，让游戏中的建筑物移动看起来不对劲，作为替换，我们需要让我们镜头
 *      相对游戏世界移动。视图矩阵是用于移动镜头的，也就是说控制我们如何看游戏场景的。
 *    - 投影矩阵：现在我们可以让对象移动到我们想要的地方，但问题是我们没有一种深度的场景，z轴没有被考虑到。我们需要一个
 *      矩阵来让对象达到近大远小的效果，这就是投影矩阵的作用。
*/

#include <cstddef>
#include <cmath>
#include <GLES2/gl2.h>
#include "../include/LoadUtil.h"
#include "../include/LogUtil.h"
#include "../include/CameraUtil.h"


// 顶点着色器
static const char  glVertexShader[] =
        "attribute vec4 vertexPosition;\n" // 顶点坐标
        "attribute vec3 vertexColour;\n" // 顶点颜色
        "varying vec3 fragColour;\n" // 用于传递给片段着色器颜色（varying用于传递属性给片段着色器）
        "uniform mat4 projection;\n" // 投影矩阵（uniform类似全局变量，可在顶点着色器和块着色器中被访问，但在其中不能被修改）
        "uniform mat4 modelView;\n" // 模型矩阵
        "void main()\n"
        "{\n"
        "    gl_Position = projection * modelView * vertexPosition;\n" // GL坐标设置成投影矩阵和模型视图矩阵的乘积
        "    fragColour = vertexColour;\n" // 传递颜色给片段着色器
        "}\n";

// 块着色器
static const char  glFragmentShader[] =
        "precision mediump float;\n" // 设置精度
        "varying vec3 fragColour;\n" // 从顶点着色器传递过来的颜色（vec3没有透明属性）
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(fragColour, 1.0);\n" // 设置颜色（1.0设置的是透明属性）
        "}\n";

GLuint simpleCubeProgram;
GLint vertexLocation;
GLint vertexColourLocation;
GLint projectionLocation;
GLint modelViewLocation;
float projectionMatrix[16];

// 顶点坐标
extern bool setupGraphics(int width, int height)
{
    simpleCubeProgram = createProgram(glVertexShader, glFragmentShader);
    if (simpleCubeProgram == 0)
    {
        LOGE ("Could not create program");
        return false;
    }
    vertexLocation = glGetAttribLocation(simpleCubeProgram, "vertexPosition"); // 获取顶点坐标
    vertexColourLocation = glGetAttribLocation(simpleCubeProgram, "vertexColour"); // 获取顶点颜色
    projectionLocation = glGetUniformLocation(simpleCubeProgram, "projection"); // 获取投影矩阵
    modelViewLocation = glGetUniformLocation(simpleCubeProgram, "modelView"); // 获取模型视图矩阵
    /* Setup the perspective */
    matrixPerspective(projectionMatrix, 45, (float)width / (float)height, 0.1f, 100);
    glEnable(GL_DEPTH_TEST); // 开启深度测试，告知OpenGL ES显示时需要考虑深度
    glViewport(0, 0, width, height);
    return true;
}

// 正方体矩阵，一个面由两个三角、六个点构成(范围0-1)
GLfloat cubeVertices[] = {-1.0f,  1.0f, -1.0f, /* Back. */
                          1.0f,  1.0f, -1.0f,
                          -1.0f, -1.0f, -1.0f,
                          1.0f, -1.0f, -1.0f,
                          -1.0f,  1.0f,  1.0f, /* Front. */
                          1.0f,  1.0f,  1.0f,
                          -1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f,  1.0f,
                          -1.0f,  1.0f, -1.0f, /* Left. */
                          -1.0f, -1.0f, -1.0f,
                          -1.0f, -1.0f,  1.0f,
                          -1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f, -1.0f, /* Right. */
                          1.0f, -1.0f, -1.0f,
                          1.0f, -1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,
                          -1.0f, -1.0f, -1.0f, /* Top. */
                          -1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f, -1.0f,
                          -1.0f,  1.0f, -1.0f, /* Bottom. */
                          -1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f, -1.0f
};

// 每个点的颜色，一行代表一组RGB值(范围0-1)
GLfloat colour[] = {1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f
};

// 绘制的点
GLushort indices[] = {0, 2, 3,
                      0, 1, 3,
                      4, 6, 7,
                      4, 5, 7,
                      8, 9, 10,
                      11, 8, 10,
                      12, 13, 14,
                      15, 12, 14,
                      16, 17, 18,
                      16, 19, 18,
                      20, 21, 22,
                      20, 23, 22
};


float angle = 0;
float modelViewMatrix[16];

// 渲染帧
extern void renderFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置清屏颜色
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // 清除深度缓冲区和颜色缓冲区
    matrixIdentityFunction(modelViewMatrix); // 初始化模型视图矩阵
    matrixRotateX(modelViewMatrix, angle); // 沿X轴旋转
    matrixRotateY(modelViewMatrix, angle); // 沿Y轴旋转
    matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f); // 往Z轴负方向移动10个单位，防止画面太近看不到
    glUseProgram(simpleCubeProgram); // 使用程序
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices); // 顶点坐标
    glEnableVertexAttribArray(vertexLocation); // 启用顶点坐标
    glVertexAttribPointer(vertexColourLocation, 3, GL_FLOAT, GL_FALSE, 0, colour); // 顶点颜色
    glEnableVertexAttribArray(vertexColourLocation); // 启用顶点颜色
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix); // 投影矩阵
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix); // 模型视图矩阵
    // 这里不能用glDrawArrays来画，因为它需要所有的点都被定义，而我们只定义了24个点，而不是36个点（只绘制了我们能看到的面）
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices); // 绘制
    angle += 1; // 旋转角度
    if (angle > 360)
    {
        angle -= 360;
    }
}
