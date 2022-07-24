#include <jni.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>

extern "C" {
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
}

#define TAG_CPP "native"
#define LOG_NATIVE(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG_CPP, __VA_ARGS__);

bool ifRunning = true;

struct my_param {
    JNIEnv *env = nullptr;
    jobject job = nullptr;
};

JavaVM *jvm;

void *rev_udp_data_thread(void *args);

int counter = 0;

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVM, void *) {
    ::jvm = javaVM;

    return JNI_VERSION_1_6;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplicationcpp_MainActivity_stringFromJNI(JNIEnv *env, jobject job) {
    char hello[20];
    sprintf(hello, "Hello from C++%d", counter);

    counter++;
    return env->NewStringUTF(hello);
}

// 事实证明这里是ok的. 子线程
void *sub_thread_process(void *args) {

    my_param *context = static_cast<my_param *>(args);
    JNIEnv *jniEnv = nullptr;

    jint attachResult = ::jvm->AttachCurrentThread(&jniEnv, nullptr);
    if (attachResult != JNI_OK) {
        return nullptr;
    }

    jclass mainActivityCls = jniEnv->GetObjectClass(context->job);

    // jmethodID method = jniEnv->GetMethodID(mainActivityCls, "printTest", "()V");
    jmethodID method = jniEnv->GetMethodID(mainActivityCls, "updateUI", "()V");

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

    // pthread_create(&ptid, NULL, sub_thread_process, myParam);
    // delete (myParam);

    pthread_create(&ptid, NULL, rev_udp_data_thread, NULL);

}

void *rev_udp_data_thread(void *args) {
    struct sockaddr_in addr;
    int sockfd, len = 0;
    int addr_len = sizeof(struct sockaddr_in);
    char buffer[256];

    bool ifRunning = true;
    /* 建立socket，注意必须是SOCK_DGRAM */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /* 填写sockaddr_in 结构 */
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(28000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);// 接收任意IP发来的数据

    /* 绑定socket */
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    while (ifRunning) {
        bzero(buffer, sizeof(buffer));
        // len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, &addr_len);
        if (len) {
            LOG_NATIVE("Received %d", len);
        }
        /* 显示client端的网络地址和收到的字符串消息 */

        // printf("Received a string from client %s, string is: %s\n", inet_ntoa(addr.sin_addr), buffer);
        /* 将收到的字符串消息返回给client端 */
        // sendto(sockfd, buffer, len, 0, (struct sockaddr *) &addr, addr_len);
    }

    return nullptr;
}