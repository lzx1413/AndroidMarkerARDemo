//
// Created by Huang on 2017/3/4.
//

#include <jni.h>
#include <add.h>

extern "C" {
jint Java_com_huang_opencvtest_MainActivity_addFromCpp(JNIEnv *env, jobject thiz, jint a, jint b){
    return add(a,b);
}
}