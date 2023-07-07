//
// Created by 薛鲁委 on 2023/3/29.
//

#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_learncpp_MainActivity_stringFromMainJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "233";
    return env->NewStringUTF(hello.c_str());
}