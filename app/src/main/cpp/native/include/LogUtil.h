#ifndef LEARNOPENGL_LOGUTIL_H
#define LEARNOPENGL_LOGUTIL_H

#include <android/log.h>
#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //LEARNOPENGL_LOGUTIL_H
