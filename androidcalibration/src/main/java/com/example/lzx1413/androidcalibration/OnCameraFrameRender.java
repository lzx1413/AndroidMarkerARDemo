package com.example.lzx1413.androidcalibration;

/**
 * Created by lzx1413 on 2017/5/15.
 */

import java.util.ArrayList;
import java.util.List;

import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Range;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

abstract class FrameRender {
    protected CameraCalibrator mCalibrator;
    protected static final String TAG = "CameraFrameRender";

    public abstract Mat render(CvCameraViewFrame inputFrame);
}
class PreviewFrameRender extends FrameRender {
    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        return inputFrame.rgba();
    }
}

class CalibrationFrameRender extends FrameRender {
    public CalibrationFrameRender(CameraCalibrator calibrator) {
        mCalibrator = calibrator;
    }

    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        Mat rgbaFrame = inputFrame.rgba();
        Mat grayFrame = inputFrame.gray();
        mCalibrator.processFrame(grayFrame, rgbaFrame);

        return rgbaFrame;
    }
}

class UndistortionFrameRender extends FrameRender {
    public UndistortionFrameRender(CameraCalibrator calibrator) {
        mCalibrator = calibrator;
    }

    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        Mat renderedFrame = new Mat(inputFrame.rgba().size(), inputFrame.rgba().type());
        Imgproc.undistort(inputFrame.rgba(), renderedFrame,
                mCalibrator.getCameraMatrix(), mCalibrator.getDistortionCoefficients());

        return renderedFrame;
    }
}

class ComparisonFrameRender extends FrameRender {
    private int mWidth;
    private int mHeight;
    private Resources mResources;
    public ComparisonFrameRender(CameraCalibrator calibrator, int width, int height, Resources resources) {
        mCalibrator = calibrator;
        mWidth = width;
        mHeight = height;
        mResources = resources;
    }

    @Override
    public Mat render(CvCameraViewFrame inputFrame) {
        Mat undistortedFrame = new Mat(inputFrame.rgba().size(), inputFrame.rgba().type());
        Imgproc.undistort(inputFrame.rgba(), undistortedFrame,
                mCalibrator.getCameraMatrix(), mCalibrator.getDistortionCoefficients());

        Mat comparisonFrame = inputFrame.rgba();
        undistortedFrame.colRange(new Range(0, mWidth / 2)).copyTo(comparisonFrame.colRange(new Range(mWidth / 2, mWidth)));
        List<MatOfPoint> border = new ArrayList<MatOfPoint>();
        final int shift = (int)(mWidth * 0.005);
        border.add(new MatOfPoint(new Point(mWidth / 2 - shift, 0), new Point(mWidth / 2 + shift, 0),
                new Point(mWidth / 2 + shift, mHeight), new Point(mWidth / 2 - shift, mHeight)));
        Imgproc.fillPoly(comparisonFrame, border, new Scalar(255, 255, 255));

        Imgproc.putText(comparisonFrame, mResources.getString(R.string.original), new Point(mWidth * 0.1, mHeight * 0.1),
                Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 0));
        Imgproc.putText(comparisonFrame, mResources.getString(R.string.undistorted), new Point(mWidth * 0.6, mHeight * 0.1),
                Core.FONT_HERSHEY_SIMPLEX, 1.0, new Scalar(255, 255, 0));

        return comparisonFrame;
    }
}
class ARFramRender extends FrameRender{
    static
    {
        System.loadLibrary("native-opencv");
    }
    private int mWidth;
    private int mHeight;
    private Resources mResources;
    private int surface_image[];
    public native boolean initMarkerDetector(long camMat,long distorCoef);
    public native void  findMarkers(long grayaddr,long rgbaaddr);
    private native boolean addSurfaceImage(int w, int h, int[] pixls);
    public ARFramRender (CameraCalibrator calibrator,int width, int height,Resources resources)
    {
        mCalibrator = calibrator;
        mWidth = width;
        mHeight = height;
        mResources = resources;
        Mat camMat = mCalibrator.getCameraMatrix();
        Mat distortion = mCalibrator.getDistortionCoefficients();
        if(!initMarkerDetector(camMat.getNativeObjAddr(),distortion.getNativeObjAddr()))
        {
            Log.e(TAG,"init marker detector failed");
        }
        final Bitmap surfaceBitmap = BitmapFactory.decodeResource(resources,R.drawable.test);
        int w = surfaceBitmap.getWidth();
        int h = surfaceBitmap.getHeight();
        surface_image = new int[w*h];
        surfaceBitmap.getPixels(surface_image,0,w,0,0,w,h);
        addSurfaceImage(w,h,surface_image);
    }
    @Override
    public Mat render(CvCameraViewFrame inputFrame){
        Mat rgbaFrame = inputFrame.rgba();
        Mat grayFrame = inputFrame.gray();
         findMarkers(grayFrame.getNativeObjAddr(),rgbaFrame.getNativeObjAddr());
        return rgbaFrame;

    }
}

class OnCameraFrameRender {
    private FrameRender mFrameRender;
    public OnCameraFrameRender(FrameRender frameRender) {
        mFrameRender = frameRender;
    }
    public Mat render(CvCameraViewFrame inputFrame) {
        return mFrameRender.render(inputFrame);
    }
}