/*****************************************************************************
*   MarkerDetector.hpp
*   Example_MarkerBasedAR
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch2 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#ifndef MarkerDetector_hpp
#define MarkerDetector_hpp

////////////////////////////////////////////////////////////////////
// Standard includes:
#include <jni.h>
#include <vector>
#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////
// File includes:
#include "BGRAVideoFrame.h"
#include "CameraCalibration.hpp"
#include "MarkerDetectionFacade.hpp"

////////////////////////////////////////////////////////////////////
// Forward declaration:
class Marker;

/**
 * A top-level class incapsulating marker detector algorithm
 */
class MarkerDetector : public MarkerDetectionFacade
{
public:
  
  /**
   * Initialize a new instance of marker detector object
   * @calibration[in] - Camera calibration (intrinsic and distorsion components) necessary for pose estimation.
   */
  MarkerDetector(CameraCalibration calibration);
    MarkerDetector();
  MarkerDetector(const cv::Mat& cameraMatrix,const cv::Mat& disCoeff);
  void processFrame(const BGRAVideoFrame& frame);
    void  resetMatrix(const cv::Mat& cameraMatrix,const cv::Mat& disCoeff);
  
  
  const std::vector<Transformation>& getTransformations() const;
  
//protected:
  bool findMarkers(const BGRAVideoFrame& frame, std::vector<Marker>& detectedMarkers);
    bool findMarkers(const cv::Mat &grayimage, std::vector<Marker>& detectedMarkers);

  void prepareImage(const cv::Mat& bgraMat, cv::Mat& grayscale);//this function is not needed because we can get the grey image from camera directly
  void performThreshold(const cv::Mat& grayscale, cv::Mat& thresholdImg);
  void findContours(const cv::Mat& thresholdImg, std::vector<std::vector<cv::Point> >& contours,int minContourPointsAllowed);
  void findMarkerCandidates(const std::vector<std::vector<cv::Point> >& contours, std::vector<Marker>& detectedMarkers);
  void detectMarkers(const cv::Mat& grayscale, std::vector<Marker>& detectedMarkers);
  void estimatePosition(std::vector<Marker>& detectedMarkers);
    cv::Mat get_camMatrix()
    {
      return camMatrix;
    }

private:
  float m_minContourLengthAllowed;
  
  cv::Size markerSize;
  cv::Mat camMatrix;
  cv::Mat distCoeff;
  std::vector<Transformation> m_transformations;
  
  cv::Mat m_grayscaleImage;
  cv::Mat m_thresholdImg;  
  std::vector<std::vector<cv::Point> > m_contours;
  std::vector<cv::Point3f> m_markerCorners3d;
  std::vector<cv::Point2f> m_markerCorners2d;
};

#endif
