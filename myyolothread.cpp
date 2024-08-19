#include "myyolothread.h"
#include "QDateTime"
#include <QDir>
#include <QUrl>


void myYoloThread::run()
{
    qDebug() << "start objectdetect";
    int width = 4096;
    int height = 2160;
    cv::VideoCapture cap(0);
    bHaveCam = false;
    bStart = false;
    if(cap.isOpened()){
        qDebug() << "open camera";
        bHaveCam = true;
        cap.set(cv::CAP_PROP_FRAME_WIDTH,width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT,height);
    }else{
        qDebug() << "camera not open";
        qDebug() << "stop objectdetect";
        bHaveCam = false;
        return;
    }


    ///////////////  Yolo Initial Setup //////////////////////////////////
    // Load names of classes
     if (net.empty()) {
         string classesFile = "D:/YOLOV4/darknet/build/darknet/x64/data/coco.names";
         //string classesFile = "coco.names";
         ifstream ifs(classesFile.c_str());
         string line;
         while (getline(ifs, line)) classes.push_back(line);
         // Give the configuration and weight files for the model
         String modelConfiguration ="D:/wolf pack/azFace-master/net_cfg/tiny-yolo-azface-fddb.cfg";
         String modelWeights = "D:/wolf pack/azFace-master/weights/tiny-yolo-azface-fddb_82000.weights";
         //String modelWeights = " D:/azFace/weights/tiny-yolo-azface-fddb_82000.weights";

         // Load the network
         net = readNetFromDarknet(modelConfiguration, modelWeights);
         net.setPreferableBackend(DNN_BACKEND_OPENCV);
         net.setPreferableTarget(DNN_TARGET_OPENCL);
     }

     bStart = true;
      myYoloSave.stFileCreateFolder2 = QString("%1%2").arg(myYoloSave.stDirectory).arg( myYoloSave.stFileCreateFolder);
     myYoloSave.stFileCreateFolderSick =QString("%1/%2").arg( myYoloSave.stFileCreateFolder2).arg( "Sick");
     myYoloSave.stFileCreateFolderNormal=QString("%1/%2").arg( myYoloSave.stFileCreateFolder2).arg( "Normal");
      QDir().mkdir( myYoloSave.stFileCreateFolder2);
      QDir().mkdir( myYoloSave.stFileCreateFolderSick );
      QDir().mkdir( myYoloSave.stFileCreateFolderNormal);

    //qDebug() << "start yolo thread";
    while(bStart)
    {


        if(cap.isOpened()){


            cap >> frame;
            int W  =  frame.cols;
            int H =  frame.rows;
            int xx = W/2;
            int yy = H/2;
            // Create a 4D blob from a frame.
           blobFromImage(frame, blob, 1/255.0, Size(inpWidth, inpHeight), Scalar(0,0,0), true, false);
           //Sets the input to the network
           net.setInput(blob);
           // Runs the forward pass to get output of the output layers
           vector<Mat> outs;
           net.forward(outs, getOutputsNames(net));
           postprocess( frame, outs);

            //std::string str2 = std::to_string(boxNumber);

            //qDebug() << "end_draw box";


            data_lock.lock();
            cvtColor(  frame,   frame,COLOR_BGR2RGB);
            frame.copyTo(frameSend);
            data_lock.unlock();


           myyoloQrcode.isCaptureyolo= false;
       }
        // myyoloQrcode.isCaptureyolo=false;
    }
    qDebug() << "stop objectdetect";
}

// Draw the predicted bounding box
void myYoloThread::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
{
    //Draw a rectangle displaying the bounding box
   // rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 255, 255), 10);

    line( frame, cv::Point( left, top  ), cv::Point(  left, top +80), Scalar(255, 255, 255),  14, 1 );
    line( frame, cv::Point( left, top  ), cv::Point( left+80, top ),Scalar(255, 255, 255),  14, 1 );

    line( frame, cv::Point(right, top  ), cv::Point( right,  top  +80), Scalar(255, 255, 255),  14, 1);
    line( frame, cv::Point( right, top  ), cv::Point( right-80, top ), Scalar(255, 255, 255),  14, 1 );

    line( frame, cv::Point(right, bottom  ), cv::Point( right,  bottom  -80), Scalar(255, 255, 255),  14, 1 );
    line( frame, cv::Point( right, bottom  ), cv::Point( right-80, bottom ), Scalar(255, 255, 255),  14, 1 );

    line( frame, cv::Point(  left, bottom ), cv::Point( left,  bottom -80),Scalar(255, 255, 255),  14, 1 );
    line( frame, cv::Point(  left, bottom  ), cv::Point(left+80, bottom ), Scalar(255, 255, 255),  14, 1 );

    //Get the label for the class name and its confidence
    string conf_label = format("%.2f", conf);
    string label="";
    if (!classes.empty())
    {
        label = classes[classId] + ":" + conf_label;
    }


    if(myyoloQrcode.isCaptureyolo==true )
    {
    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 2, 1, &baseLine);
    top = max(top, labelSize.height);

    box_text  =  format(" %0.2f", myTempYolo.CTempYolo) + " C";

    if( myTempYolo.CTempYolo >37.3){
        rectangle(frame, Point(left+2, top +40), Point(left + labelSize.width, top - baseLine), Scalar(0, 0, 255), FILLED);
    }else {
         rectangle(frame, Point(left+2, top +40), Point(left + labelSize.width, top - baseLine), Scalar(0, 255, 0), FILLED);
    }

    putText(frame, box_text, Point(left, top+40), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0,0,0),1,LINE_AA);

    }

    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 2, 1, &baseLine);
    top = max(top, labelSize.height);

}


// Remove the bounding boxes with low confidence using non-maxima suppression
void myYoloThread::postprocess(Mat& frame, const vector<Mat>& outs)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;
    int Bleft = 0;
    int BRight = 0;
    int BTop = 0;
    int BBottom = 0;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols)*1.2;
                int height = (int)(data[3] * frame.rows)*1.5;
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    vector<int> indices;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        Bleft = (int)box.x;
        BRight = (int)(box.x+box.width);
        BTop = (int)box.y;
        BBottom = (int)(box.y+box.height);

        if( Bleft > 20 && BRight<(int)(frame.cols-20) && BTop > 20 && BBottom < (int)(frame.rows-20))
       {
                myTarget.faceRecog = true;

               drawPred(classIds[idx], confidences[idx], box.x, box.y,
                         box.x + box.width, box.y + box.height, frame);



                           if(myyoloQrcode.isCaptureyolo==true )
                           {


                               if(myTempYolo.CTempYolo>37.3){
                                   myYoloSave.stFileAddPicture= QString("%1/%2").arg(myYoloSave.stFileCreateFolderSick).arg(myYoloSave.stFile7);
                                   //                                            QMediaPlayer * welcom_sound = new QMediaPlayer();
                                   //                                            //welcom_sound->setMedia(QUrl("C:/Users/moudp/Documents/NKRAFA_TEMP_FACE_RECORD_V1/camera-shutter.wav"));
                                   //                                            welcom_sound->setMedia(QUrl("qrc:/camera-shutter.wav"));
                                   //                                            welcom_sound->play();
                               }else {
                                   myYoloSave.stFileAddPicture= QString("%1/%2").arg(myYoloSave.stFileCreateFolderNormal).arg(myYoloSave.stFile7);
                                   //                                            QMediaPlayer * welcom_sound = new QMediaPlayer();
                                   //                                            //welcom_sound->setMedia(QUrl("C:/Users/moudp/Documents/NKRAFA_TEMP_FACE_RECORD_V1/camera-shutter.wav"));
                                   //                                            welcom_sound->setMedia(QUrl("qrc:/pass_ka.mp3"));
                                   //                                            welcom_sound->play();
                                   //qDebug()<< "kuyyy";
                               }

                               cv::resize(frame(box),FrameResize, Size(300, 400));
                               imwrite(myYoloSave.stFileAddPicture.toStdString(),  FrameResize);


                           }

        }
    }

}

// Get the names of the output layers
vector<String> myYoloThread:: getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        vector<String> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i){
            names[i] = layersNames[outLayers[i] - 1];
        }
    }
    return names;
}





