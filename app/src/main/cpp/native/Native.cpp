#include <jni.h>
#include "include/TextureCube.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengl_nativecode_NativeRender_init(JNIEnv *env, jobject thiz, jint width, jint height) {
    setupGraphics(width, height); // 初始化OpenGL ES
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengl_nativecode_NativeRender_setup(JNIEnv *env, jobject thiz) {
    renderFrame(); // 渲染
}