#include <jni.h>
#include <android/log.h>

extern "C" {
#include "demo/demo.h"
}

#define TAG "native"
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);

extern int counter;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplicationcpp_TestLog_stringFromTest(JNIEnv *env, jobject thiz) {
    // jstring str = env->NewStringUTF("From test, right?");
    char outputChar[15];
    sprintf(outputChar, "counter:%d", counter);
    jstring str = env->NewStringUTF(outputChar);
    return str;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplicationcpp_TestLog_createNewThread(JNIEnv *env, jobject thiz) {

    LOG("Start new thread");
    start_new_thread();
    LOG("End new thread");
}

