//
// Created by fotoable on 2017/5/17.
//

#ifndef OPENCVTEST_ARDISPLAY_H
#define OPENCVTEST_ARDISPLAY_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <vector>
#define DEBUG
class ARDisplay {
public:
    ARDisplay()
    {

         rec3dMat = (cv::Mat_<float>(4,4)<<   -0.5,-0.5,  0.5, 0.5,
                                              -0.5,0.5,0.5,   -0.5,
                                              -1,-1,  -1,   -1,
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
    void MapImage(cv::Mat&img, cv::Mat& outterMatrix, cv::Mat& cameraMatrix,int image_index = 0)
    {
        assert(image_index<surface_images.size());
        cv::Mat tmp;
        cv::Mat rec2dMat;
        cv::gemm(outterMatrix,rec3dMat,1,0,0,tmp);
        cv::gemm(cameraMatrix,tmp,1,0,0,rec2dMat);
        std::vector<cv::Point2f> per_points;
        for(int i = 0;i<rec2dMat.cols;++i)
        {
            auto point = rec2dMat.col(i);
            cv::Point per_point;
            per_point.x =point.at<float>(0,0)/point.at<float>(2,0);
            per_point.y =point.at<float>(1,0)/point.at<float>(2,0);
            per_points.push_back(per_point);
            circle(img,per_point, 10, cv::Scalar(0, 255, 0, 255),-1);
        }
        //img(cv::Rect(0,0,127,127)) = surface_images.at(0).clone();
        cv::Mat M = cv::getPerspectiveTransform(image_corners2d.at(image_index), per_points);
        cv::Mat per_image;
        cv::warpPerspective(surface_images.at(image_index), per_image,  M, img.size());
        cv::Mat mask;
        cv::inRange(per_image,cv::Scalar(0,0,0,255),cv::Scalar(255,255,255,255),mask);
        per_image.copyTo(img,mask);
#ifdef DEBUG
        cv::imwrite("/sdcard/mask.png",mask);
        cv::imwrite("/sdcard/surfaceimage.png",per_image);
        cv::imwrite("/sdcard/merge.png",img);
#endif
    }
    void AppendSurfaceImages(cv::Mat& img)
    {
        assert(img.data != NULL);
        surface_images.push_back(img);
        std::vector<cv::Point2f> single_img_corners;
        single_img_corners.push_back(cv::Point2f(0,0));
        single_img_corners.push_back(cv::Point2f(0,img.rows-1));
        single_img_corners.push_back(cv::Point2f(img.cols-1,img.rows-1));
        single_img_corners.push_back(cv::Point2f(img.cols-1,0));
        image_corners2d.push_back(single_img_corners);
    }

private:
    cv::Mat cube3dMat;
    cv::Mat rec3dMat;
    std::vector<cv::Mat> surface_images;
    std::vector<std::vector<cv::Point2f>> image_corners2d;
    void Mat2PointVec(const cv::Mat& mat,std::vector<cv::Point>& points)
    {

    }

};


#endif //OPENCVTEST_ARDISPLAY_H
