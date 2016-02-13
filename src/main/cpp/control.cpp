/*
 * Copyright (c) 2015 Richard Warburton (richard.warburton@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 */

#include <jvmti.h>
#include <stdio.h>
#include <syscall.h> 
 


#include "profiler.h"

extern "C"
JNIEXPORT jboolean JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_start(JNIEnv *env, jclass klass) {
    Profiler *prof = getProfiler();

    return prof->start(env);
}

extern "C"
JNIEXPORT void JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_stop(JNIEnv *env, jclass klass) {
    Profiler *prof = getProfiler();

    prof->stop();
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_isRunning(JNIEnv *env, jclass klass) {
    Profiler *prof = getProfiler();

    return prof->isRunning();
}

extern "C"
JNIEXPORT jlong JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_getStaticMethodId(JNIEnv *env, jclass klass, const jclass lookupKlazz, const jstring name, const jstring sig) {
	const char * pname = env->GetStringUTFChars(  name , NULL ) ;
	const char * psig = env->GetStringUTFChars(  sig , NULL ) ;
//	jmethodID mid_getName = env->GetMethodID(lookupKlazz, "getName", "()Ljava/lang/String;");
//	jstring cname = (jstring)env->CallObjectMethod(lookupKlazz, mid_getName);
//	const char * pcname = env->GetStringUTFChars(  cname , NULL ) ;
// 	std::cout << "Looking up class:" << lookupKlazz << ", mname:" << pname << ", sig:" << psig << std::endl;
    return (jlong)env->GetStaticMethodID(lookupKlazz, pname, psig);
}

extern "C"
JNIEXPORT jlong JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_getMethodId(JNIEnv *env, jclass klass, const jclass lookupKlazz, const jstring name, const jstring sig) {
	const char * pname = env->GetStringUTFChars(  name , NULL ) ;
	const char * psig = env->GetStringUTFChars(  sig , NULL ) ;
//	jmethodID mid_getName = env->GetMethodID(lookupKlazz, "getName", "()Ljava/lang/String;");
//	jstring cname = (jstring)env->CallObjectMethod(lookupKlazz, mid_getName);
//	const char * pcname = env->GetStringUTFChars(  cname , NULL ) ;
//	std::cout << "Looking up class:" << lookupKlazz << ", mname:" << pname << ", sig:" << psig << std::endl;
    return (jlong)env->GetMethodID(lookupKlazz, pname, psig);
}

extern "C"
JNIEXPORT jint JNICALL Java_com_insightfullogic_honest_1profiler_core_control_Agent_getThreadId(JNIEnv *env, jclass klass) {
	return syscall(__NR_gettid);
}
