//
// Created by fotoable on 2017/5/17.
//

#ifndef OPENCVTEST_ARDISPLAY_H
#define OPENCVTEST_ARDISPLAY_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <vector>

class ARDisplay {
public:
    ARDisplay()
    {

         rec3dMat = (cv::Mat_<float>(4,4)<<   -0.5,-0.5,  0.5, 0.5,
                                              -0.5,0.5,-0.5,   0.5,
                                              0,0,  0,   0,
                                              1,1,  1,   1);
         cube3dMat = (cv::Mat_<float>(4,8)<<   -0.5,-0.5,  0.5, 0.5, -0.5,-0.5,  0.5, 0.5,
                                               -0.5,0.5,   0.5,   -0.5,-0.5,0.5,0.5,   -0.5,
                                                0,0,  0,   0,-1,-1,-1,-1,
                                                1,1,  1,   1,1,1,1,1);
    }
    void PlotCube(cv::Mat&img, cv::Mat& outterMatrix, cv::Mat& cameraMatrix)
    {
        cv::Mat tmp;
        cv::Mat cube2dMat;
        cv::gemm(outterMatrix,cube3dMat,1,0,0,tmp);
        cv::Mat camera = cv::Mat::ones(3,3,CV_32FC1);
        cv::gemm(cameraMatrix,tmp,1,0,0,cube2dMat);
        std::vector<cv::Point> per_points;
        for(int i = 0;i<cube2dMat.cols;++i)
        {
            auto point = cube2dMat.col(i);
            cv::Point per_point;
            per_point.x = static_cast<int>(point.at<float>(0,0)/point.at<float>(2,0));
            per_point.y = static_cast<int>(point.at<float>(1,0)/point.at<float>(2,0));
            per_points.push_back(per_point);
            circle(img,per_point, 10, cv::Scalar(0, 255, 0, 255),-1);
        }
        //plot cube
        cv::line(img,per_points[0],per_points[1],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[1],per_points[2],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[2],per_points[3],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[3],per_points[0],cv::Scalar(0,0,255,255),5);


        cv::line(img,per_points[4],per_points[5],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[5],per_points[6],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[6],per_points[7],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[7],per_points[4],cv::Scalar(0,0,255,255),5);


        cv::line(img,per_points[0],per_points[4],cv::Scalar(0,0,255,255),5);
        cv::line(img,per_points[1],per_points[5],cv::Scalar(0,0,0,255),5);
        cv::line(img,per_points[2],per_points[6],cv::Scalar(0,255,0,255),5);
        cv::line(img,per_points[3],per_points[7],cv::Scalar(255,0,0,255),5);


    }

private:
     cv::Mat cube3dMat;
    cv::Mat rec3dMat;
    void Mat2PointVec(const cv::Mat& mat,std::vector<cv::Point>& points)
    {

    }

};


#endif //OPENCVTEST_ARDISPLAY_H
