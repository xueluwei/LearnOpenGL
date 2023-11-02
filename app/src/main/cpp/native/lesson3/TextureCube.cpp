/**
 * lesson2我们使用了单个颜色来渲染一个三角，对于简单的程序来说这样还行，但是如果在一个小的地方绘制非常多颜色的情况下，
 * 还用这种方法就要绘制非常多的三角了，这样就很不合理。所以我们就要使用纹理来解决这个问题。
 *
 * 纹理就像一个图片，贴到三角形上来显示，这样的话一个三角形就能显示很多不同的颜色。图片的格式没有限制，
 * 你需要做的只是以合适的方式加载你想要的图片，这个例子中，我们用最简单的RAW格式。
 *
 * 加载纹理大概流程：
 *    - 从系统中加载一个文件，然后转化为OpenGL ES能够理解的格式
 *    - 生成纹理对象并激活需要的纹理单元
 *    - 使用glTexImage2D把纹理对象加载到纹理单元中
 *    - 使用glTexParameteri方法设置采样
 *    - 把纹理坐标加载到对应的系统顶点上
 *    - 把采样对象添加到块着色器上
*/

#include <GLES2/gl2.h>
#include "../include/LoadUtil.h"
#include "../include/LogUtil.h"
#include "../include/CameraUtil.h"

// 顶点着色器
static const char glVertexShader[] =
        "attribute vec4 vertexPosition;\n" // 顶点坐标
        "attribute vec2 vertexTextureCord;\n" // 顶点纹理坐标
        "varying vec2 textureCord;\n" // 用于传递给片段着色器纹理坐标（坐标是由左下（0，0）到右上（1，1））
        "uniform mat4 projection;\n" // 投影矩阵
        "uniform mat4 modelView;\n" // 模型矩阵
        "void main()\n"
        "{\n"
        "    gl_Position = projection * modelView * vertexPosition;\n" // GL坐标设置成投影矩阵和模型视图矩阵的乘积
        "    textureCord = vertexTextureCord;\n" // 传递纹理坐标给片段着色器
        "}\n";

// 块着色器
static const char glFragmentShader[] =
        "precision mediump float;\n" // 设置精度
        "uniform sampler2D texture;\n" // 用来展示我们需要使用的纹理单元，在我们只有一个纹理时不是很重要
        "varying vec2 textureCord;\n" // 从顶点着色器传递过来的纹理坐标
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, textureCord);\n" // 设置颜色
        "}\n";

/**
 * 用于加载纹理，具体流程如下：
 *    1.指定id并加载图片
 *    2.将图片打包加载到图像内存中，并生成纹理对象id
 *    3.激活纹理单位并绑定一个指定的单位类型，纹理单位的数量由硬件决定（至少8种），
 *    对于纹理单位来说，我们至少有两种选择，GL_TEXTURE_2D或GL_TEXTURE_CUBE_MAP，我们常选择GL_TEXTURE_2D，
 *    后续操作都是对这个纹理单位进行的，在这个对象上，我们可以上传真正的纹理数据。
 *    4.使用glTexImage2D上传纹理数据
 *    5.设置过滤模型
 * @return 纹理对象id
 */
GLuint loadSimpleTexture() {
    /* 渲染对象id */
    GLuint textureId;
    /* 简单制造一个3x3的RAW图片,每行代表RGBA值（范围0-255）*/
    GLubyte pixels[9 * 4] = {
            18, 140, 171, 255, /* 左下角 */
            143, 143, 143, 255, /* 下面中间 */
            255, 255, 255, 255, /* 右下角 */
            255, 255, 0, 255, /* 中间左边 */
            0, 255, 255, 255, /* 中间*/
            255, 0, 255, 255, /* 中间右边 */
            255, 0, 0, 255, /* 左上角 */
            0, 255, 0, 255, /* 上面中间 */
            0, 0, 255, 255, /* 右上角 */
    };
    /* 打包数据（缩减资源） */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    /* 生成纹理对象 */
    glGenTextures(1, &textureId);
    /* 激活纹理 */
    glActiveTexture(GL_TEXTURE0);
    /* 绑定纹理对象 */
    glBindTexture(GL_TEXTURE_2D, textureId);
    /* 加载纹理，
     * 第一个参数是我们要用的纹理单位，
     * 第二个参数是纹理贴图的等级，纹理贴图是个重要的技术，以后会讨论，当前设置为0即可
     * 第三个参数是我们需要用到的内部格式，OpenGL 2.0中内部格式要和传入图片的格式一致（没有转化方法），所以第七个参数和这个一样
     * 第四个和第五个参数表示图片的宽高，该例子中是3x3的图片
     * 第六个参数你想在图片周围加的边距，在OpenGL ES中需要被设置为0
     * 第八个参数是我们需要使用的数据的类型
     * 第九个参数就是我们传入的数据 */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    /* 设置过滤模型，拉伸或者收缩模式，比如一个面超过3x3，就使用拉伸来铺满这个面 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureId;
}

GLuint glProgram;
GLint vertexLocation;
GLint textureCordLocation;
GLint projectionLocation;
GLint modelViewLocation;
GLint samplerLocation;
GLuint textureId;
GLfloat projectionMatrix[16];

// 设置图像（类似lesson2，只在最后多了个加载纹理逻辑）
extern bool setupGraphics(int width, int height)
{
    glProgram = createProgram(glVertexShader, glFragmentShader);
    if (!glProgram)
    {
        LOGE ("Could not create program");
        return false;
    }
    vertexLocation = glGetAttribLocation(glProgram, "vertexPosition");
    textureCordLocation = glGetAttribLocation(glProgram, "vertexTextureCord");
    projectionLocation = glGetUniformLocation(glProgram, "projection");
    modelViewLocation = glGetUniformLocation(glProgram, "modelView");
    samplerLocation = glGetUniformLocation(glProgram, "texture");
    /* 设置视角 */
    matrixPerspective(projectionMatrix, 45, (float)width / (float)height, 0.1f, 100);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    /* 加载纹理 */
    textureId = loadSimpleTexture();
    if(textureId == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// 正方体顶点坐标
GLfloat cubeVertices[] = {-1.0f,  1.0f, -1.0f, /* 后面 */
                          1.0f,  1.0f, -1.0f,
                          -1.0f, -1.0f, -1.0f,
                          1.0f, -1.0f, -1.0f,
                          -1.0f,  1.0f,  1.0f, /* 前面 */
                          1.0f,  1.0f,  1.0f,
                          -1.0f, -1.0f,  1.0f,
                          1.0f, -1.0f,  1.0f,
                          -1.0f,  1.0f, -1.0f, /* 左面 */
                          -1.0f, -1.0f, -1.0f,
                          -1.0f, -1.0f,  1.0f,
                          -1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f, -1.0f, /* 右面 */
                          1.0f, -1.0f, -1.0f,
                          1.0f, -1.0f,  1.0f,
                          1.0f,  1.0f,  1.0f,
                          -1.0f, 1.0f, -1.0f, /* 上面 */
                          -1.0f, 1.0f,  1.0f,
                          1.0f, 1.0f,  1.0f,
                          1.0f, 1.0f, -1.0f,
                          -1.0f, - 1.0f, -1.0f, /* 下面 */
                          -1.0f,  -1.0f,  1.0f,
                          1.0f, - 1.0f,  1.0f,
                          1.0f,  -1.0f, -1.0f
};

// 纹理坐标（纹理是在一个平面上渲染的，所以坐标只用xy即可，对应定点矩阵的相应点，四个点组成一个面），
// 谨记我们的纹理坐标是从左下角（0，0）到右上角（1，1）的，
// 如果我们单取出前面，我们会看到定点矩阵左上角的点是（-1.0f，1.0f，1.0f）对应的是纹理矩阵的（0.0f, 1.0f），
// 这种情况适用于所有顶点
GLfloat textureCords[] = { 1.0f, 1.0f, /* 后面 */
                           0.0f, 1.0f,
                           1.0f, 0.0f,
                           0.0f, 0.0f,
                           0.0f, 1.0f, /* 前面 */
                           1.0f, 1.0f,
                           0.0f, 0.0f,
                           1.0f, 0.0f,
                           0.0f, 1.0f, /* 左面 */
                           0.0f, 0.0f,
                           1.0f, 0.0f,
                           1.0f, 1.0f,
                           1.0f, 1.0f, /* 右面 */
                           1.0f, 0.0f,
                           0.0f, 0.0f,
                           0.0f, 1.0f,
                           0.0f, 1.0f, /* 上面 */
                           0.0f, 0.0f,
                           1.0f, 0.0f,
                           1.0f, 1.0f,
                           0.0f, 0.0f, /* 下面 */
                           0.0f, 1.0f,
                           1.0f, 1.0f,
                           1.0f, 0.0f
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

float modelViewMatrix[16];
float angle = 0;


extern void renderFrame() {
    glVertexAttribPointer(textureCordLocation, 2, GL_FLOAT, GL_FALSE, 0, textureCords);
    glEnableVertexAttribArray(textureCordLocation);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE,projectionMatrix);
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix);
    /* 设置采样纹理为0，我们只有一个纹理（GL_TEXTURE0）就直接使用 */
    glUniform1i(samplerLocation, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置清屏颜色
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // 清除深度缓冲区和颜色缓冲区
    matrixIdentityFunction(modelViewMatrix); // 初始化模型视图矩阵
    matrixRotateX(modelViewMatrix, angle); // 沿X轴旋转
    matrixRotateY(modelViewMatrix, angle); // 沿Y轴旋转
    matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f); // 往Z轴负方向移动10个单位，防止画面太近看不到
    glUseProgram(glProgram); // 使用程序
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices); // 顶点坐标
    glEnableVertexAttribArray(vertexLocation); // 启用顶点坐标
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