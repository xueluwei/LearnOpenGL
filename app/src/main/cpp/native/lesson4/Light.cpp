/**
 * 之前我们已经用简单的颜色来渲染我们的立方体了，从灯光的角度来看，每个面都是被照亮的，所以颜色就跟我们指定的完全一致，
 * 为了让场景更加真实，我们需要介绍向场景里添加光照的概念，这节课我们介绍顶点法线、漫反射、镜面反射、环境光的概念。
 *
 * OpenGL ES 2.0之前没有灯光的概念，这个概念源于开发者设计并实现了一个场景中模拟光照的方法。就像计算机图像中的其他
 * 实现一样，光照也是假的，这意味着我们可以实现一个足够接近的方法来模拟光照，而不需要尝试去实现光子的真实物理模型（光追）。
 *
 * 我们要通过算法处理每个顶点的值来实现一个定向的光照，在处理中我们也会考虑一下这个实现方法的一些局限性。我们会尽可能简单
 * 的使用一些关于场景、灯光、材料的假设，处理时我们会一一指出。
 *
 * 我们会使用Phong反射模型，简单的解释：环境量+漫反射+镜面反射=Phong反射模型
 *
 * 环境量：环境光常量乘环境光强度，是个固定的量，类似于光照进物体之后会被吸收一部分，这部分是固定的，不会随着角度或视角变化
 * 漫反射：根据光的入射角的反向向量和法线的反向向量点积计算夹角，然后乘漫反射光照常量计算光强
 * 镜面反射：根据光的入射角和我们的视角反向的向量点积计算夹角，然后乘镜面反射光照常量计算光强
*/

#include <GLES2/gl2.h>
#include "../include/CameraUtil.h"
#include "../include/LoadUtil.h"
#include "../include/LogUtil.h"

// 顶点坐标，我们每个面加一个特殊的点，这样我们就可以计算出每个面的法线了
GLfloat vertices[] = { 1.0f,  1.0f, -1.0f, /* 后面 */
                        -1.0f,  1.0f, -1.0f,
                        1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f, -1.0f,
                        0.0f,  0.0f, -1.0f,
                        -1.0f,  1.0f,  1.0f, /* 前面 */
                        1.0f,  1.0f,  1.0f,
                        -1.0f, -1.0f,  1.0f,
                        1.0f, -1.0f,  1.0f,
                        0.0f,  0.0f,  1.0f,
                        -1.0f,  1.0f, -1.0f, /* 左面 */
                        -1.0f,  1.0f,  1.0f,
                        -1.0f, -1.0f, -1.0f,
                        -1.0f, -1.0f,  1.0f,
                        -1.0f,  0.0f,  0.0f,
                        1.0f,  1.0f,  1.0f, /* 右面 */
                        1.0f,  1.0f, -1.0f,
                        1.0f, -1.0f,  1.0f,
                        1.0f, -1.0f, -1.0f,
                        1.0f,  0.0f,  0.0f,
                        -1.0f, -1.0f,  1.0f, /* 下面 */
                        1.0f, -1.0f,  1.0f,
                        -1.0f, -1.0f, -1.0f,
                        1.0f, -1.0f, -1.0f,
                        0.0f, -1.0f,  0.0f,
                        -1.0f,  1.0f, -1.0f, /* 上面 */
                        1.0f,  1.0f, -1.0f,
                        -1.0f,  1.0f,  1.0f,
                        1.0f,  1.0f,  1.0f,
                        0.0f,  1.0f,  0.0f
};

// 法线坐标
GLfloat normals[] = { 1.0f,  1.0f, -1.0f, /* 后面 */
                      -1.0f,  1.0f, -1.0f,
                      1.0f, -1.0f, -1.0f,
                      -1.0f, -1.0f, -1.0f,
                      0.0f,  0.0f, -1.0f,
                      -1.0f,  1.0f,  1.0f, /* 前面 */
                      1.0f,  1.0f,  1.0f,
                      -1.0f, -1.0f,  1.0f,
                      1.0f, -1.0f,  1.0f,
                      0.0f,  0.0f,  1.0f,
                      -1.0f,  1.0f, -1.0f, /* 左面 */
                      -1.0f,  1.0f,  1.0f,
                      -1.0f, -1.0f, -1.0f,
                      -1.0f, -1.0f,  1.0f,
                      -1.0f,  0.0f,  0.0f,
                      1.0f,  1.0f,  1.0f, /* 右面 */
                      1.0f,  1.0f, -1.0f,
                      1.0f, -1.0f,  1.0f,
                      1.0f, -1.0f, -1.0f,
                      1.0f,  0.0f,  0.0f,
                      -1.0f, -1.0f,  1.0f, /* 下面 */
                      1.0f, -1.0f,  1.0f,
                      -1.0f, -1.0f, -1.0f,
                      1.0f, -1.0f, -1.0f,
                      0.0f, -1.0f,  0.0f,
                      -1.0f,  1.0f, -1.0f, /* 上面 */
                      1.0f,  1.0f, -1.0f,
                      -1.0f,  1.0f,  1.0f,
                      1.0f,  1.0f,  1.0f,
                      0.0f,  1.0f,  0.0f
};

// 顶点颜色
GLfloat colour[] = {1.0f, 0.0f, 0.0f, /* 后面 */
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, /* 前面 */
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, /* 左面 */
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    1.0f, 1.0f, 0.0f, /* 右面 */
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 1.0f, /* 下面 */
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    0.0f, 1.0f, 1.0f,
                    1.0f, 0.0f, 1.0f, /* 上面 */
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f,
                    1.0f, 0.0f, 1.0f
};

// 每个面绘制的三角形（本来一个面2个三角，但是加上法线后的点，每个面就要画4个三角形）
GLushort indices[] = {0,  2,  4,/* 后面 */
                      0,  4,  1,
                      1,  4,  3,
                      2,  3,  4,
                      5,  7,  9, /* 前面 */
                      5,  9,  6,
                      6,  9,  8,
                      7,  8,  9,
                      10, 12, 14,/* 左面 */
                      10, 14, 11,
                      11, 14, 13,
                      12, 13, 14,
                      15, 17, 19, /* 右面 */
                      15, 19, 16,
                      16, 19, 18,
                      17, 18, 19,
                      20, 22, 24, /* 下面 */
                      20, 24, 21,
                      21, 24, 23,
                      22, 23, 24,
                      25, 27, 29, /* 上面 */
                      25, 29, 26,
                      26, 29, 28,
                      27, 28, 29
};


// 下面是cube绘制
// 顶点着色器
static const char  glVertexShader[] =
        "attribute vec3 vertexNormal;\n" // 顶点法线
        "attribute vec4 vertexPosition;\n" // 顶点坐标
        "attribute vec3 vertexColour;\n" // 顶点颜色
        "varying vec3 fragColour;\n" // 用于传递给片段着色器颜色（varying用于传递属性给片段着色器）
        "uniform mat4 projection;\n" // 投影矩阵（uniform类似全局变量，可在顶点着色器和块着色器中被访问，但在其中不能被修改）
        "uniform mat4 modelView;\n" // 模型矩阵
        "void main()\n"
        "{\n"
        "    vec3 transformedVertexNormal = normalize((modelView * vec4(vertexNormal, 0.0)).xyz);" // 用模型矩阵来转换顶点法线
        "    vec3 inverseLightDirection = normalize(vec3(0.0, 1.0, 1.0));\n" // 入射光反转（我们需要一个往外发散的向量表示反射出的光的向量）
        "    fragColour = vec3(0.0);\n" // 初始化颜色，后续根据光照各种反射模型来累加
        "    vec3 diffuseLightIntensity = vec3(1.0, 1.0, 1.0);\n" // 漫反射光强度初始化（这个例子中用的白光）
        "    vec3 vertexDiffuseReflectionConstant = vertexColour;\n" // 表示漫反射光的颜色常量
        "    float normalDotLight = max(0.0, dot(transformedVertexNormal, inverseLightDirection));\n" // 点乘计算出法线和光线的夹角，cos值，和0.0做max方法过滤掉负值（反射光在背面的值）
        "    fragColour += normalDotLight * vertexDiffuseReflectionConstant * diffuseLightIntensity;\n" // 角度*颜色常量*强度，得到漫反射光的颜色，累加到块颜色中
        "    vec3 ambientLightIntensity = vec3(0.1, 0.1, 0.1);\n" // 环境光强度初始化
        "    vec3 vertexAmbientReflectionConstant = vertexColour;\n" // 表示环境光的颜色常量，这个例子中用vertexColour简单表示，通常这个值需要单独指定颜色
        "    fragColour += vertexAmbientReflectionConstant * ambientLightIntensity;\n" // 环境光颜色*环境光强度，得到环境光的颜色，累加到块颜色中，这个颜色是统一的，不会随着角度或视角变化
        "    vec3 inverseEyeDirection = normalize(vec3(0.0, 0.0, 1.0));\n" // 反转后的视角方向，这个例子简单使用了一个固定的视角，通常这个值需要根据相机矩阵计算
        "    vec3 specularLightIntensity = vec3(1.0, 1.0, 1.0);\n" // 镜面反射光强度
        "    vec3 vertexSpecularReflectionConstant = vec3(1.0, 1.0, 1.0);\n" // 镜面反射光的颜色常量，这里不像上面环境光和漫反射光一样用顶点颜色，而是用白光，因为反射到视角中的光是白光
        "    float shininess = 2.0;\n" // 指数，用于计算镜面反射光的强度，因为我们点积的值都是0-1之间的，所以这个值越大（反射的向量与视角的夹角越大），镜面反射光的结果越小
        "    vec3 lightReflectionDirection = reflect(vec3(0) - inverseLightDirection, transformedVertexNormal);\n" // 使用reflect接口计算反射光的方向
        "    float normalDotReflection = max(0.0, dot(inverseEyeDirection, lightReflectionDirection));\n" // 点积计算反射光和视角的夹角
        "    fragColour += pow(normalDotReflection, shininess) * vertexSpecularReflectionConstant * specularLightIntensity;\n" // 角度*颜色常量*强度，得到镜面反射光的颜色，累加到块颜色中
        "    clamp(fragColour, 0.0, 1.0);\n" // 确保颜色在0-1之间
        "    gl_Position = projection * modelView * vertexPosition;\n" // GL坐标设置成投影矩阵和模型视图矩阵的乘积
        "}\n";

// 块着色器
static const char  glFragmentShader[] =
        "precision mediump float;\n" // 设置精度
        "varying vec3 fragColour;\n" // 从顶点着色器传递过来的颜色（vec3没有透明属性）
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(fragColour, 1.0);\n" // 设置颜色（1.0设置的是透明属性）
        "}\n";

GLuint lightProgram;
GLint vertexLocation;
GLint vertexNormalLocation; // 顶点法线缓存
GLint vertexColourLocation;
GLint projectionLocation;
GLint modelViewLocation;
float projectionMatrix[16];

// 顶点坐标
extern bool setupGraphics(int width, int height)
{
    lightProgram = createProgram(glVertexShader, glFragmentShader);
    if (lightProgram == 0)
    {
        LOGE ("Could not create program");
        return false;
    }
    vertexLocation = glGetAttribLocation(lightProgram, "vertexPosition"); // 获取顶点坐标
    vertexColourLocation = glGetAttribLocation(lightProgram, "vertexColour"); // 获取顶点颜色
    vertexNormalLocation = glGetAttribLocation(lightProgram, "vertexNormal"); // 获取顶点法线坐标
    projectionLocation = glGetUniformLocation(lightProgram, "projection"); // 获取投影矩阵
    modelViewLocation = glGetUniformLocation(lightProgram, "modelView"); // 获取模型视图矩阵
    matrixPerspective(projectionMatrix, 45, (float)width / (float)height, 0.1f, 100);
    glEnable(GL_DEPTH_TEST); // 开启深度测试，告知OpenGL ES显示时需要考虑深度
    glViewport(0, 0, width, height);
    return true;
}

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
    glUseProgram(lightProgram); // 使用程序
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, vertices); // 顶点坐标
    glEnableVertexAttribArray(vertexLocation); // 启用顶点坐标
    glVertexAttribPointer(vertexColourLocation, 3, GL_FLOAT, GL_FALSE, 0, colour); // 顶点颜色
    glEnableVertexAttribArray(vertexColourLocation); // 启用顶点颜色
    glVertexAttribPointer(vertexNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, normals); // 法线坐标
    glEnableVertexAttribArray(vertexNormalLocation); // 启用顶点法线坐标
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix); // 投影矩阵
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix); // 模型视图矩阵
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_SHORT, indices); // 绘制
    angle += 1; // 旋转角度
    if (angle > 360)
    {
        angle -= 360;
    }
}