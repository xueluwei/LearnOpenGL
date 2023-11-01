/**
 * lesson2我们使用了单个颜色来渲染一个三角，对于简单的程序来说这样还行，但是如果在一个小的地方绘制非常多颜色的情况下，
 * 还用这种方法就要绘制非常多的三角了，这样就很不合理。所以我们就要使用纹理来解决这个问题。
 *
 * 纹理就像一个图片，贴到三角形上来显示，这样的话一个三角形就能显示很多不同的颜色。图片的格式没有限制，
 * 你需要做的只是以合适的方式加载你想要的图片，这个例子中，我们用最简单的RAW格式。
*/

#include <GLES2/gl2.h>



/**
 * 用于加载纹理，具体流程如下：
 *    1.指定id并加载图片
 *    2.将图片打包加载到图像内存中，并生成纹理对象id
 *    3.激活纹理单位并绑定一个指定的单位类型，纹理单位的数量由硬件决定（至少8位），
 *    对于纹理单位来说，我们至少有两种选择，GL_TEXTURE_2D或GL_TEXTURE_CUBE_MAP，我们常选择GL_TEXTURE_2D
 *    4.
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
    /* 加载纹理 */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    /* 设置过滤模型 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureId;
}

