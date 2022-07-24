#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>

#define TAG_CPP "native"
#define LOG_NATIVE(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG_CPP, __VA_ARGS__);

bool ifRunning = true;

struct my_param {
    JNIEnv *env = nullptr;
    jobject job = nullptr;
};

JavaVM *jvm;

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVM, void *) {
    ::jvm = javaVM;

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplicationcpp_MainActivity_stringFromJNI(JNIEnv *env, jobject job) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// 事实证明这里是ok的. 子线程
void *sub_thread_process(void *args) {

    my_param *context = static_cast<my_param *>(args);
    JNIEnv * jniEnv = nullptr;

    jint attachResult = ::jvm->AttachCurrentThread(&jniEnv, nullptr);
    if(attachResult!= JNI_OK){
        return nullptr;
    }

    jclass mainActivityCls = jniEnv->GetObjectClass(context->job);

    jmethodID  method= jniEnv->GetMethodID(mainActivityCls, "printTest", "()V");
    int counterMax = 5;

    while (ifRunning) {

        LOG_NATIVE("running\n");
        sleep(1);
        counterMax++;

        jniEnv->CallVoidMethod(context->job, method);
    }

    return nullptr;
}

// JNIEnv *env 不能跨越线程，否则奔溃， 他可以跨越函数 【解决方式：使用全局的Java
// jobject thiz 不能跨越线程，否则奔溃，不能跨越函数，否则奔溃 【解决方式：默认是局
// JavaVM 能够跨越线程，能够跨越函数
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplicationcpp_MainActivity_startSubThread(JNIEnv *env, jobject thiz) {

    pthread_t ptid;

    // my_param * myParam = new my_param;
    my_param *myParam = new my_param;

    // jvm->GetEnv(static_cast<void *> myParam->env, JNI_VERSION_1_6);

    // jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    myParam->env = env;
    myParam->job = env->NewGlobalRef(thiz); // 把局部成员 提升为 全局成员
    pthread_create(&ptid, NULL, sub_thread_process, myParam);

    // delete (myParam);

}