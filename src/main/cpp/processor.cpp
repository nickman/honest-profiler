#include <thread>
#include <iostream>
#include "processor.h"

#ifdef WINDOWS
#include <windows.h>
#else

#include <unistd.h>

#endif

static jthread newThread(JNIEnv *jniEnv) {
    jclass thrClass;
    jmethodID cid;
    jthread res;

    thrClass = jniEnv->FindClass("java/lang/Thread");
    if (thrClass == NULL) {
        logError("Cannot find Thread class\n");
    }
    cid = jniEnv->GetMethodID(thrClass, "<init>", "()V");
    if (cid == NULL) {
        logError("Cannot find Thread constructor method\n");
    }
    res = jniEnv->NewObject(thrClass, cid);
    if (res == NULL) {
        logError("Cannot create new Thread object\n");
    } else {
       jmethodID mid = jniEnv->GetMethodID(thrClass, "setName","(Ljava/lang/String;)V");
       jniEnv->CallObjectMethod(res, mid, jniEnv->NewStringUTF("Honest Profiler Daemon Thread"));  
    }
    return res;
}

const uint MILLIS_IN_MICRO = 1000;

void sleep_for_millis(uint period) {
#ifdef WINDOWS
    Sleep(period);
#else
    //std::cout << "Period: " << period << "\n";
    usleep(period * MILLIS_IN_MICRO);
#endif
}

void Processor::run() {
    std::cout << "Running....\n";
    int popped = 0;

    while (true) {
        while (buffer_.pop()) {
            ++popped;
        }

        if (popped > 200) {
            if (!handler_.updateSigprofInterval()) {
                break;
            }
            popped = 0;
        }

        if (!isRunning_.load()) {
            std::cout << "Stop Flag Set. Stopping....\n";
            break;
        }

        sleep_for_millis(interval_);
    }

    handler_.stopSigprof();
}

void callbackToRunProcessor(jvmtiEnv *jvmti_env, JNIEnv *jni_env, void *arg) {
    IMPLICITLY_USE(jvmti_env);
    IMPLICITLY_USE(jni_env);
    Processor *processor = (Processor *) arg;    
    processor->run();
}

void Processor::start(JNIEnv *jniEnv) {
     bool actual = isRunning_.load();
     bool expected = false;
     bool setTo = true;
    if(!actual && isRunning_.compare_exchange_weak(expected, setTo, std::memory_order_release, std::memory_order_relaxed)) {
        std::cout << "Starting...  \n";
        jthread thread = newThread(jniEnv);
        jvmtiStartFunction callback = callbackToRunProcessor;
        jvmti_->RunAgentThread(thread, callback, this, JVMTI_THREAD_NORM_PRIORITY);
        std::cout << "Started\n";
    } else {
        std::cerr << "Already Running.\n";
    }

}

void Processor::stop() {
    isRunning_.store(false);
    std::cout << "Stop\n";
}

bool Processor::isRunning() const {
    return isRunning_.load();
}
