#include "videocaptureworker.h"

VideoCaptureWorkerSet obVideoCaptureWorkerSet;
VideoCaptureWorkerSet gcsVideoCaptureWorkerSet;

VideoCaptureWorker::VideoCaptureWorker(VideoCaptureWorkerSet* s,QObject *parent) :QObject(parent){

    set = s;

    timeObj.start();
}

#ifdef WITH_IGNITION
void VideoCaptureWorker::ignCallback(const ignition::msgs::Image &img){
    CameraInfo* ci = &set->cameraInfo;

    const unsigned char *data = reinterpret_cast<const unsigned char*>(img.data().c_str());
    cv::Mat matImg(img.height(),img.width(), CV_8UC3,(unsigned char*) data);
    double captured_time = timeObj.elapsed()*1.0e-3;

    cvtColor(matImg,matImg,cv::COLOR_RGB2BGR);

    if (set->showCapturedFrame){
            cv::imshow(namedWindowStr, matImg);
    }

    //correct for lens distortion
    if (set->doUndistort && ci->available){
        undistort(matImg,matUndistorted,ci->intrinsicMatrix,ci->distCoeffs);
        emit frameReady(matUndistorted,frameCnt,captured_time);
    }else{
        emit frameReady(matImg,frameCnt,captured_time);
    }

    frameCnt++;
}
#endif

void VideoCaptureWorker::init(QThread* vThread,QTimer* vTimer){

    switch (set->captureSource ){
        case  CAP_SOURCE_IGNITION:
#ifdef WITH_IGNITION
            if (!ign_node.Subscribe(set->ignitionCameraTopic.toStdString(), &VideoCaptureWorker::ignCallback,this)){
              
                qDebug()  << "Error subscribing to topic " << set->ignitionCameraTopic;
                return ;
            }
#else
            qDebug() << "Ignition not compiled";
#endif
            break;

        case CAP_SOURCE_OPENCV:
        case CAP_SOURCE_OPENCV_VIDEO_DEMO:
        default:

            QObject::connect(vThread,SIGNAL(started()),vTimer,SLOT(start()));
            QObject::connect(vTimer,SIGNAL(timeout()),this,SLOT(onTimeout()));

            int captureRate = 30;
            vTimer->setInterval(1000.0/captureRate);
            vTimer->moveToThread(vThread);
          //  cv::ocl::setUseOpenCL(true);
            if (set->captureSource == CAP_SOURCE_OPENCV_VIDEO_DEMO){
                cap.open("demo.avi");
            }else{            
		cap.open(set->cvCamID);
            }
            
            if(cap.isOpened()){
          	cap.set(cv::CAP_PROP_FRAME_WIDTH, set->cvFrameWidth);
            	cap.set(cv::CAP_PROP_FRAME_HEIGHT,set->cvFrameHeight);
            }else{
                qDebug("can't open video");
                return;
	    }
	    
            break;
    }

    frameCnt = 0;
   
    if (set->showCapturedFrame){
        namedWindowStr = set->namedWindowText.toStdString();
        cv::namedWindow(namedWindowStr);
    }

    this->moveToThread(vThread);
    vThread->start(QThread::HighPriority);
}

void VideoCaptureWorker::onTimeout(){
  
    long t1,t2,dt;
    CameraInfo* ci = &set->cameraInfo;

    t1= timeObj.elapsed();

    if(cap.isOpened()==false){
        return;
    }

    if(cap.read(matOriginalBGR)==false){
           qDebug() << "no frame";
           //repeat the video
           if (frameCnt > 20 && set->captureSource == CAP_SOURCE_OPENCV_VIDEO_DEMO){
               qDebug() << "repeating demo video";
               frameCnt =0;
               cap.set(cv::CAP_PROP_POS_FRAMES, 0);
           }
           return;
    }
    double captured_time = timeObj.elapsed()*1.0e-3;

    if(matOriginalBGR.empty()==true){
           return;
     }
    if (set->showCapturedFrame){
        cv::imshow(namedWindowStr,matOriginalBGR);
    }

    cvtColor(matOriginalBGR,matOriginal,cv::COLOR_BGR2RGB);

    //correct for lens distortion
    if (set->doUndistort && ci->available){ 
        undistort(matOriginal,matUndistorted,ci->intrinsicMatrix,ci->distCoeffs);
        emit frameReady(matUndistorted,frameCnt,captured_time);
    }else{
        emit frameReady(matOriginal,frameCnt,captured_time);
    }

    frameCnt++;

    //if delay is too long reinitilize the video capture
    t2= timeObj.elapsed();
    dt = t2-t1;
    /*
    qDebug() << dt <<"dt";
    if (frameCnt > 1 && dt > 300){
        qDebug() << "reinitialize cap";
        cap.release();
        delay(6000);
            cv::ocl::setUseOpenCL(true);
            cap.open(0);
            if(cap.isOpened()==false){
                qDebug("can't open video");
                return;
            }
    }*/    
}


void delay( int millisecondsToWait ){
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime ){
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void VideoCaptureWorker::loadCameraParams(){

    CameraInfo* ci = &set->cameraInfo;

    //first check if there is a cameraInfo data available
    //I mean..it's kind of weird that I am reading fx and fy twice..but whatever
    ci->available = false;
    QSettings settings(set->cameraInfoFile,QSettings::IniFormat);
    settings.beginGroup("CameraInfo");
        if (settings.contains("fx") && settings.contains("fy")){
            if (settings.value("fx").toDouble() > 0 && settings.value("fy").toDouble() > 0){
                 ci->available = true;      
            }
        }
    settings.endGroup();

    ci->intrinsicMatrix = cv::Mat::zeros(3,3,CV_64FC1);
    ci->intrinsicMatrix.at<double>(0,0) = 1.0;
    ci->intrinsicMatrix.at<double>(1,1) = 1.0;
    ci->intrinsicMatrix.at<double>(2,2) = 1.0;
    ci->distCoeffs      = cv::Mat::zeros(4,1,CV_64FC1);

    if (ci->available){
        BEGIN_READ_SETTINGS(set->cameraInfoFile,CameraInfo,1)
            READ_SETTING(set->cameraInfo,fx,Double)
            READ_SETTING(set->cameraInfo,fy,Double)
            READ_SETTING(set->cameraInfo,offset_x,Double)
            READ_SETTING(set->cameraInfo,offset_y,Double)
            READ_SETTING(set->cameraInfo,skew,Double)
            READ_SETTING(set->cameraInfo,k1,Double)
            READ_SETTING(set->cameraInfo,k2,Double)
            READ_SETTING(set->cameraInfo,k3,Double)
            READ_SETTING(set->cameraInfo,p1,Double)
            READ_SETTING(set->cameraInfo,p2,Double)
        END_READ_SETTINGS

        ci->intrinsicMatrix.at<double>(0,0) = ci->fx;
        ci->intrinsicMatrix.at<double>(0,2) = ci->offset_x;
        //why didn't I include skew?
        ci->intrinsicMatrix.at<double>(1,1) = ci->fy;
        ci->intrinsicMatrix.at<double>(1,2) = ci->offset_y;

        ci->intrinsicMatrix.at<double>(2,2) = 1.0;

        ci->distCoeffs.at<double>(0,0) = ci->k1;
        ci->distCoeffs.at<double>(1,0) = ci->k2;
        ci->distCoeffs.at<double>(2,0) = ci->p1;
        ci->distCoeffs.at<double>(3,0) = ci->p2;
        qDebug() << "cameraInfo loaded";
        qDebug() << "intrinsicMatrix";
        std::cout << ci->intrinsicMatrix << std::endl << std::endl;
    }   

}
