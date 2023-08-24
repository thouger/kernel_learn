#include <jni.h>
#include <string>
#include <sys/prctl.h>
#include <unistd.h>

int trace_current_process() {
//    ALOGE("start trace_current_process ");
    prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
    int mainProcessPid = getpid();
    pid_t child = fork();
    if (child < 0) {
//        ALOGE("ptrace svc  fork() error ");
        return -errno;
    }
    //init first tracer
    Tracer *first = get_tracer(nullptr, mainProcessPid, true);
    if (child == 0) {
        // attch main pid
        int status = ptrace(PTRACE_ATTACH, mainProcessPid, NULL, NULL);
        if (status != 0) {
            //attch失败
            ALOGE(">>>>>>>>> error: attach target process %d ", status);
            return -errno;
        }
        first->wait_sigcont = true;
        //开始执行死循环代码,因为处于一直监听状态,理论上该进程不会退出
        exit(event_loop());
    } else {
        //init seccomp by main process
        //the seccomp filtering rule is intended only for the current process
        enable_syscall_filtering(first);
    }
    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_seccomp_1ptrace_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    trace_current_process();
    return env->NewStringUTF(hello.c_str());
}