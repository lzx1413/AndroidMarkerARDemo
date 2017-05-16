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
#include <MarkerDetector.hpp>
#include <Marker.hpp>

#define alog(...) __android_log_print(ANDROID_LOG_ERROR, "F8DEMO", __VA_ARGS__);
static MarkerDetector marker_detector;
static std::vector<Marker> detecedMarkers;
using namespace cv;
using namespace std;


extern "C"
JNIEXPORT void JNICALL
Java_com_example_lzx1413_androidcalibration_ARFramRender_findMarkers(JNIEnv *env, jobject instance,
                                                                     jlong grayaddr) {
    detecedMarkers.clear();
    Mat grayImg = *(Mat*) grayaddr;

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lzx1413_androidcalibration_ARFramRender_initMarkerDetector(JNIEnv *env,
                                                                            jobject instance,
                                                                            jlong camMataddr,
                                                                            jlong distorCoefaddr) {

    Mat& camMat = *(Mat*) camMataddr;
    Mat& distorCoef = *(Mat*) distorCoefaddr;
    marker_detector.resetMatrix(camMat,distorCoef);
    return true;

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
    //std::vector<std::vector<cv::Point> > contours;
    //marker_detector.performThreshold(gray,gray);
    //marker_detector.findContours(gray,contours,100);
    /*
    for (int i = 0; i < contours.size(); ++i) {
        for(auto point:contours[i]) {
            circle(rgba, Point(point.x , point.y), 1, Scalar(255, 0, 0, 255));
        }
    }
     */
    for (auto point : v)
    {
        circle(rgba, Point(point.pt.x/scale , point.pt.y/scale), 10, Scalar(255, 0, 0, 255));
    }

}
}
