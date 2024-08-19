#ifndef VIDEOCAPTUREWORKER_H
#define VIDEOCAPTUREWORKER_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

#ifdef WITH_IGNITION
#include <ignition/transport.hh>
#endif

#include "paramsloader.h"

#define CAP_SOURCE_IGNITION 1
#define CAP_SOURCE_OPENCV 2 
#define CAP_SOURCE_OPENCV_VIDEO_DEMO 3

struct CameraInfo {
     bool available  = false; 
     double fx = 0.0;
     double fy = 0.0;
     double offset_x = 0.0;
     double offset_y  = 0.0;
     double skew = 0.0;
     double k1 = 0.0;
     double k2 = 0.0;
     double k3 = 0.0;
     double p1 = 0.0;
     double p2 = 0.0;
     cv::Mat intrinsicMatrix;
     cv::Mat distCoeffs;
};

extern struct VideoCaptureWorkerSet {
    bool doUndistort = 0;
    bool showCapturedFrame = 0;
    int captureSource = CAP_SOURCE_IGNITION;
    QString ignitionCameraTopic = "/rgbd_camera/image";
    QString namedWindowText     = "Captured Frame";
    QString cameraInfoFile      = "settings/gazebo_camerainfo.ini";
    int cvCamID		 = 0;
    int cvFrameWidth		 = 640;
    int cvFrameHeight		 = 480; 
    CameraInfo cameraInfo;
} obVideoCaptureWorkerSet,gcsVideoCaptureWorkerSet;

class VideoCaptureWorker: public QObject{
    Q_OBJECT
public:
    explicit VideoCaptureWorker(VideoCaptureWorkerSet* s,QObject *parent = 0);
    void loadCameraParams();
    VideoCaptureWorkerSet* set;
    void init(QThread* vThread,QTimer* vTimer);
    std::string namedWindowStr;
signals:
     void reportCaptureTime(unsigned long process_time);
     void uFrameReady(cv::UMat uMat);
     void frameReady(cv::Mat mat,unsigned long frameCnt,double time);
private:

     QElapsedTimer timeObj;
     cv::Mat frame;
     cv::VideoCapture cap;
     QSize image_size;
     cv::Mat matOriginalBGR;
     cv::Mat matOriginal;
     cv::Mat matUndistorted;
     unsigned long frameCnt;
#ifdef WITH_IGNITION
     ignition::transport::Node ign_node;
     ignition::transport::v11::Node::Publisher pub;
     void ignCallback(const ignition::msgs::Image &img);
#endif
public slots:
    void onTimeout();
};

void delay( int millisecondsToWait );
#endif // VIDEOCAPTUREWORKER_H
