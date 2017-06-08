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
#include <ARDisplay.h>
#define LOG "ar-process-jni"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__) // 定义LOGF类型
using namespace cv;
using namespace std;
static MarkerDetector marker_detector;
static std::vector<Marker> detecedMarkers;
static ARDisplay arDisplay;

JNIEXPORT void JNICALL
Java_com_example_lzx1413_androidcalibration_MainActivity_nativeInit(JNIEnv *env, jobject instance) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_lzx1413_androidcalibration_MainActivity_nativeResize(JNIEnv *env, jobject instance,
                                                                      jint w, jint h) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_example_lzx1413_androidcalibration_MainActivity_nativeRender(JNIEnv *env,
                                                                      jobject instance) {

    // TODO

}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_lzx1413_androidcalibration_ARFramRender_addSurfaceImage(JNIEnv *env,
                                                                         jobject instance, jint w,
                                                                         jint h, jintArray pixls_) {
    jint *pixls = env->GetIntArrayElements(pixls_, NULL);

    // TODO
    if (pixls == NULL) {
        LOGF("input image is null");
        return false;
    }
    cv::Mat in_mat(h,w,CV_8UC4,(unsigned char*) pixls);
    cv::cvtColor(in_mat,in_mat,CV_BGRA2RGBA);
    auto long_side = 128;
    cv::resize(in_mat,in_mat,cv::Size(long_side,long_side));//according to the marker the image must be squared
    arDisplay.AppendSurfaceImages(in_mat);
    env->ReleaseIntArrayElements(pixls_, pixls, 0);
    return true;
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_lzx1413_androidcalibration_ARFramRender_findMarkers(JNIEnv *env, jobject instance,
                                                                     jlong grayaddr,jlong rgbaaddr) {
    detecedMarkers.clear();
    Mat grayImg = *(Mat*) grayaddr;
    Mat rgbaImg = *(Mat*) rgbaaddr;
    if(marker_detector.findMarkers(grayImg,detecedMarkers))
    {
        for(auto marker : detecedMarkers) {
            for (auto point :marker.points) {
                circle(rgbaImg, point, 10, Scalar(255, 0, 0, 255));

            }
            cv::Mat outterMatrix = marker.transformation.getMat34();
            cv::Mat camMatrix = marker_detector.get_camMatrix();
            arDisplay.PlotCube(rgbaImg, outterMatrix, camMatrix);
            arDisplay.MapImage(rgbaImg,outterMatrix,camMatrix);
        }
    }
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
