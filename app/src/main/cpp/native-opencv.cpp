//
// Created by Huang on 2017/3/4.
//

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

extern "C" {
void Java_com_huang_opencvtest_MainActivity_nativeProcessFrame(JNIEnv *env, jobject thiz, jlong addrGray, jlong addrRGBA){
    Mat& gray = *(Mat *) addrGray;
    Mat& rgba = *(Mat *) addrRGBA;
    vector<KeyPoint> v;

    Ptr<ORB> orb = ORB::create();
    orb->detect(gray, v, cv::Mat());

    for (int i = 0; i < v.size(); ++i) {
        const KeyPoint& kp = v[i];
        circle(rgba, Point(kp.pt.x, kp.pt.y), 10, Scalar(255,0,0,255));
    }
}
}