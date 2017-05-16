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

#define alog(...) __android_log_print(ANDROID_LOG_ERROR, "F8DEMO", __VA_ARGS__);
static MarkerDetector marker_detector;
using namespace cv;
using namespace std;
RNG rng(12345);
Mat * mCanny = NULL;


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
JNIEXPORT jboolean JNICALL
Java_com_huang_opencvtest_CameraPreview_ImageProcessing(JNIEnv *env, jobject instance, jint width,
                                                        jint height, jbyteArray NV21FrameData_,
                                                        jintArray pixels_) {
    jbyte * pNV21FrameData = env->GetByteArrayElements(NV21FrameData_, 0);
    jint * poutPixels = env->GetIntArrayElements(pixels_, 0);

    if ( mCanny == NULL )
    {
        mCanny = new Mat(height, width, CV_8UC1);
    }
    Mat yuv(height/2+height,width,CV_8UC1,(unsigned char *)pNV21FrameData);
    Mat mGray(height, width, CV_8UC1, (unsigned char *)pNV21FrameData);
    Mat mResult(height, width, CV_8UC4, (unsigned char *)poutPixels);
    IplImage srcImg = mGray;
    IplImage CannyImg = *mCanny;
    IplImage ResultImg = mResult;

    cvCanny(&srcImg, &CannyImg, 80, 100, 3);
    //cvCvtColor(&CannyImg, &ResultImg, CV_GRAY2BGRA);
    cvtColor(yuv, mResult, CV_YUV420sp2BGR, 4);
    env->ReleaseByteArrayElements(NV21FrameData_, pNV21FrameData, 0);
    env->ReleaseIntArrayElements(pixels_, poutPixels, 0);
    return true;
}
}
