#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_learnopengl_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "666";
    return env->NewStringUTF(hello.c_str());
}