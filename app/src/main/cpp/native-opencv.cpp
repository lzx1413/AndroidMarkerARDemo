//
// Created by Huang on 2017/3/4.
//

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <vector>
//caffe2
#include <caffe2/core/predictor.h>
#include <caffe2/core/operator.h>
#include <caffe2/core/timer.h>
#include "caffe2/core/init.h"
//#include <caffe2/utils/proto_utils.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define alog(...) __android_log_print(ANDROID_LOG_ERROR, "F8DEMO", __VA_ARGS__);
using namespace cv;
using namespace std;
static caffe2::NetDef _initNet, _predictNet;
static caffe2::Predictor* _predictor;
static caffe2::Workspace ws;

void loadToNetDef(AAssetManager* mgr,caffe2::NetDef* net, const char* filename){
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
    assert(asset != nullptr);
    const void *data = AAsset_getBuffer(asset);
    assert(data != nullptr);
    off_t len = AAsset_getLength(asset);
    assert(len != 0);
    if (!net->ParseFromArray(data, len)) {
        alog("Couldn't parse net from data.\n");
    }
    AAsset_close(asset);
}
extern "C" {
void Java_com_huang_opencvtest_MainActivity_nativeProcessFrame(JNIEnv *env, jobject thiz, jlong addrGray, jlong addrRGBA){
    Mat& gray = *(Mat *) addrGray;
    Mat& rgba = *(Mat *) addrRGBA;
    vector<KeyPoint> v;

//    LOG("Width: %d, Height: %d", gray.cols, gray.rows);

    float scale = 0.4;
    Mat resized;
    resize(gray, resized, Size(gray.cols*scale,gray.rows*scale));
    Ptr<ORB> orb = ORB::create();
    orb->detect(resized, v, cv::Mat());

    for (int i = 0; i < v.size(); ++i) {
        const KeyPoint& kp = v[i];
        circle(rgba, Point(kp.pt.x/scale, kp.pt.y/scale), 10, Scalar(255,0,0,255));
    }
}
}
