#include <GLES2/gl2.h>
#include <cstdlib>

#include "../include/LogUtil.h"

/**
 * 加载所有着色器编组并渲染到GPU上
 * @param shaderType
 * @param shaderSource
 * @return
 */
GLuint loadShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType); // 创建着色器
    if (shader) // 创建成功
    {
        glShaderSource(shader, 1, &shaderSource, NULL); // 选择着色器使用的源码（定点着色器或者块着色器）
        glCompileShader(shader); // 编译着色器
        GLint compiled = 0; // 用于检查编译是否成功
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); // 检查编译是否成功
        if (!compiled) // 编译失败
        {
            GLint infoLen = 0; // 用于获取错误信息长度
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen); // 获取错误信息长度
            if (infoLen) // 有错误信息
            {
                char * buf = (char*) malloc(infoLen); // 申请内存
                if (buf) // 申请成功
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf); // 获取错误信息
                    LOGE("Could not Compile Shader %d:\n%s\n", shaderType, buf); // 打印错误信息
                    free(buf); // 释放内存
                }
                glDeleteShader(shader); // 删除着色器
                shader = 0; // 置空
            }
        }
    }
    return shader; // 返回着色器
}

/**
 * 创建着色器程序（持有着色器的东西）
 * @param vertexSource
 * @param fragmentSource
 * @return
 */
GLuint createProgram(const char* vertexSource, const char * fragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource); // 加载顶点着色器
    if (!vertexShader) // 确保加载成功
    {
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource); // 加载块着色器
    if (!fragmentShader) // 确保加载成功
    {
        return 0;
    }
    GLuint program = glCreateProgram(); // 调用创建着色器程序
    if (program) // 创建成功
    {
        glAttachShader(program , vertexShader); // 将顶点着色器添加到着色器程序
        glAttachShader(program, fragmentShader); // 将块着色器添加到着色器程序
        glLinkProgram(program); // 链接着色器程序
        GLint linkStatus = GL_FALSE; // 用于检查链接是否成功
        glGetProgramiv(program , GL_LINK_STATUS, &linkStatus); // 检查链接是否成功
        if (linkStatus != GL_TRUE) // 链接失败
        {
            GLint bufLength = 0; // 用于获取错误信息长度
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength); // 获取错误信息长度
            if (bufLength) // 有错误信息
            {
                char* buf = (char*) malloc(bufLength); // 申请内存
                if (buf) // 申请成功
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf); // 获取错误信息
                    LOGE("Could not link program:\n%s\n", buf); // 打印错误信息
                    free(buf); // 释放内存
                }
            }
            glDeleteProgram(program); // 删除着色器程序
            program = 0; // 置空
        }
    }
    return program; // 返回着色器程序
}
