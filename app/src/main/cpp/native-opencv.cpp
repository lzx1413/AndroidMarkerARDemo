//
// Created by Huang on 2017/3/4.
//

#include <vector>
#include <jni.h>
#include <opencv2/opencv.hpp>
//caffe2
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define alog(...) __android_log_print(ANDROID_LOG_ERROR, "F8DEMO", __VA_ARGS__);
using namespace cv;
using namespace std;
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
