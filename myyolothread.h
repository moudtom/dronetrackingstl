#ifndef MYYOLOTHREAD_H
#define MYYOLOTHREAD_H


#include <QObject>
#include <QDebug>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/dnn.hpp>
#include <QString>
#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#include <QThread>
#include <QTimer>
#define OPENCV
using namespace cv;
using namespace dnn;
using namespace std;



class myYoloThread : public QThread
{
    Q_OBJECT

public:

    std::vector<char> getResultImage();
    std::mutex data_lock;
      void run();
      void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
      void postprocess(Mat& frame, const vector<Mat>& outs);
      vector<String> getOutputsNames(const Net& net);

    struct targetPosition
    {
        double x,y;
        bool isDetect;
         bool faceRecog;
         int xx;
         int yy ;
        targetPosition() {
            x = 0;
            y = 0;
            xx=0;
            yy=0;
            isDetect = false;
            faceRecog = false;
        }
    };

    targetPosition myTarget;

    struct tempMeasurementYolo
    {

        float CTempYolo= 0.0f;

    };


    tempMeasurementYolo myTempYolo;

    struct yoloqrRead
    {

        bool isCaptureyolo;
        std::string yoloName;
       yoloqrRead() {

           isCaptureyolo = false;
           yoloName = "";
        }
    };

    yoloqrRead myyoloQrcode;

    cv::Mat frame, frameSend;
    bool bStart,bHaveCam;

    // Initialize the parameters
    float confThreshold = 0.1; // Confidence threshold
    float nmsThreshold = 0.7;  // Non-maximum suppression threshold
    int inpWidth = 416;  // Width of network's input image
    int inpHeight = 416; // Height of network's input image
     Net net;

    vector<string> classes;
     vector<String> classNamesVec;
     Mat input, blob, DisFace,DisOriginal,ProFilePic,inputF,FrameResize;
     std::string box_text;
     bool found = false;
     bool Capture = false;

     int Xm=0;
     int Ym=0;




       struct yoloSaveData
       {

           QString stFile6;
           QString stFile7;
           QString stFileCreateFolder;
           QString stDirectory;
           QString  stFileAddPicture ;
           QString stFileCreateFolderSick;
           QString stFileCreateFolderNormal;
           QString stFileCreateFolder2;
          yoloSaveData() {

              stFile6="";
              stFile7="";
              stFileCreateFolder="";
              stDirectory="";
              stFileAddPicture="" ;
              stFileCreateFolderSick="";
              stFileCreateFolderNormal="";
              stFileCreateFolder2="";
           }
       } myYoloSave;


private:
 
};




#endif // MYYOLOTHREAD_H
