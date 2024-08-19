#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMainWindow>
#define pi  3.141592
cv::Point mPos(0,0);
bool LClick = false;
double ErrorYaw = 0;
double ErrorX = 0;
double yawPos = 0;
using Eigen::MatrixXd;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    mPos = cv::Point(x,y);

    if  ( event == EVENT_LBUTTONDOWN )
    {
        LClick = true;
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_LBUTTONUP )
    {
        LClick = false;
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myTimer = new QTimer(this);
    //Socket Connection
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1",14551);
    //socket->connectToHost("192.168.43.51" , 14551);

    connect(socket, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
    connect(myTimer, SIGNAL(timeout()),this,SLOT(timmer_loop()));
    connect(myTimer, SIGNAL(timeout()),this,SLOT(onReadyRead()));
    myTimer->start(50);

    namedWindow("test",1); //anan
    setMouseCallback("test",CallBackFunc);
    Mat myFrame(Size(640,480),CV_8UC3,Scalar(0,0,0));//anan
    int cx = 320;//anan
    int cy = 240;//anan
    line(myFrame,cv::Point(0,cy),cv::Point(2*cx,cy),Scalar(0,255,0));//anan
    line(myFrame,cv::Point(cx,0),cv::Point(cx,2*cy),Scalar(0,255,0));//anan
    //myFrame= imread("C:/Users/moudp/Documents/Basemap/basemap3.tif");
    imshow("test",myFrame); //anan

    _maparcgis.setMinimumWidth(1080);
    _maparcgis.setMinimumHeight(960);
    _maparcgis.show();

    if(!socket->waitForConnected(5000))
    {
        qDebug() << "port " << "14551" << " CONNECTION Error: " << socket->errorString();

    }
    //_yolothread.start();
    //connect(sTime,SIGNAL(timeout()),this,SLOT(onReadyRead()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::controller()
{

}

void MainWindow::timmer_loop()
{

    if(LClick == true)
    {
        ErrorYaw = mPos.x - 320;
        ErrorX = 240 - mPos.y;

        Px= ErrorYaw;
        Py = ErrorX;
        //qDebug()<<"X="<<  Px<<",Y="<<  Py;
        ////////////////// Geolocation Formura ///////////////////////////

        Xmav = X_NED;
        Ymav = Y_NED;
        //qDebug()<<"X="<<  Xmav<<",Y="<< Ymav;
        double dist = sqrt(Xmav*Xmav + Ymav*Ymav);
        double rollxx = Deg2rad*roll;
        double pitchxx = Deg2rad*pitch;
        double yawxx = Deg2rad*(heading);
        double elx = Deg2rad*el;
        double alx = Deg2rad*al;

        // inertial to vehicle frame
        Eigen::Matrix4f Tiv;
        Tiv << 1, 0, 0,Xmav,
                0, 1, 0,Ymav,
                0, 0, 1,Hmav,
                0, 0, 0,    1;

        // vehicle to body frame
        Eigen::Matrix4f Tvb;
        Tvb <<                cos(pitchxx)*cos(yawxx)                              , cos(pitchxx)*sin(yawxx)                                          , -sin(pitchxx)        ,0,
                (sin(rollxx)*sin(pitchxx)*cos(yawxx))-(cos(rollxx)*sin(yawxx)) ,(sin(rollxx)*sin(pitchxx)*sin(yawxx))+(cos(rollxx)*cos(yawxx)), sin(rollxx)*cos(pitchxx) ,0,
                (cos(rollxx)*sin(pitchxx)*cos(yawxx))+(sin(rollxx)*sin(yawxx)) ,(cos(rollxx)*sin(pitchxx)*sin(yawxx))-(sin(rollxx)*cos(yawxx)), cos(rollxx)*cos(pitchxx) ,0,
                0                                                             , 0                                                                 ,0                    ,1;
        // the body to the gimbal

        Eigen::Matrix4f Tbg;
        Tbg <<   cos(elx)*cos(alx), cos(elx)*sin(alx) ,sin(elx), 0,
                -sin(elx)         , cos(alx)          , 0      , 0,
                -sin(elx)*cos(alx),-sin(elx)*sin(alx) ,cos(elx), 0,
                0 ,                  0,       0, 1;

        //the gimbal to the camera
        Eigen::Matrix4f Tgc ;
        Tgc <<   0 ,0 ,-1, 0,
                0 ,1 ,0 , 0,
                1 ,0 ,0 , 0,
                0 ,0 ,0 , 1;

        //camera
        Eigen::Vector4f q(Px,Py,1,1);
        Eigen::Matrix4f AA  = Tgc*Tbg*Tvb*Tiv;
        Eigen::Vector4f Pcci;
        Eigen::Vector4f B(0,0,0,1);
        Pcci = AA.inverse()*B;
        double Zcci =Pcci(2,0);
        Eigen::Matrix4f C ;
        C <<  0, fx ,ox ,0,
                -fy,  0 ,oy ,0,
                0,  0 , 1 ,0,
                0,  0 , 0 ,1;

        // The Target location
        Eigen::Matrix4f A = C*Tgc*Tbg*Tvb*Tiv;
        Eigen::Vector4f Qobji;
        Qobji = A.inverse()*q;
        double Zobji = Qobji(2,0);
        double YY = Zcci/(Zcci-Zobji);
        Eigen::Matrix4f AAA;
        AAA << YY,0 ,0 ,0,
                0 ,YY, 0,0,
                0 ,0 ,YY,0,
                0 ,0 ,0 ,1;
        Eigen::Vector4f Pobji;
        Pobji = A.inverse()*AAA*q;

        objX =  Pobji(0,0);
        objY =  Pobji(1,0);

        qDebug()<<"X="<<objX<<",Y="<<objY;

        mavlink_set_position_target_local_ned_t gps = {0};
        mavlink_message_t               msg;
        uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
        gps.target_system = 0;
        gps.target_component = 0;
        gps.time_boot_ms = time_stamp;
        gps.coordinate_frame = 1;
        gps.type_mask =  0b100111000000;
        gps.x = objX;
        gps.y = objY;
        gps.z = -Altitude;


        mavlink_msg_set_position_target_local_ned_encode(1, 255, &msg, &gps);
        uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
        socket->write((const char*)data_buff_send,len);
    }

    //        if(abs(ErrorYaw) < 50 && ErrorX >50)
    //        {
    //            // go
    //            double _tempMag = 0.5*ErrorX ;
    //            if(_tempMag > 10) _tempMag = 10;
    //            else if(_tempMag < -10) _tempMag = -10;
    //            double Deg2rad = 3.14/180.0;
    //            mavlink_set_position_target_local_ned_t gps = {0};
    //            mavlink_message_t               msg;
    //            uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    //            gps.target_system = 0;
    //            gps.target_component = 0;
    //            gps.time_boot_ms = time_stamp;
    //            gps.coordinate_frame = 1;
    //            gps.type_mask =  0b100111000111;
    //            //gps.x = _tempMag*cos(Deg2rad*heading);
    //            //gps.y = _tempMag*sin(Deg2rad*heading);

    //            gps.vx =_tempMag*cos(Deg2rad*heading);
    //            gps.vy = _tempMag*sin(Deg2rad*heading);


    //            mavlink_msg_set_position_target_local_ned_encode(1, 255, &msg, &gps);
    //            uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //            socket->write((const char*)data_buff_send,len);
    //            qDebug()<<"("<<gps.vx<<","<<gps.vy<<")";
    //        }else{

    //            //yaw
    //            yawPos += 0.00001*ErrorYaw;

    //            if(yawPos >= 360){
    //                yawPos = 0;
    //            }else if(yawPos < 0 ){
    //                yawPos = 360-yawPos;
    //            }

    //            if(ErrorYaw >= 0){

    //                mavlink_message_t msg;
    //                uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    //                mavlink_command_long_t arm_mode = { 0 };
    //                arm_mode.target_system = 1;
    //                arm_mode.target_component = 1;
    //                arm_mode.command = MAV_CMD_CONDITION_YAW;
    //                //arm_mode.confirmation = true;
    //                arm_mode.param1 = yawPos; //degree
    //                arm_mode.param2 = 10; //rate deg/sec
    //                arm_mode.param3 = 1; //direction
    //                arm_mode.param4 = 1;
    //                mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    //                uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //                //qint64 len_send = socket->write((const char*)data_buff_send,len);
    //                socket->write((const char*)data_buff_send,len);
    //                qDebug()<<"020";
    //            }else if(ErrorYaw < 0){
    //                mavlink_message_t msg;
    //                uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    //                mavlink_command_long_t arm_mode = { 0 };
    //                arm_mode.target_system = 1;
    //                arm_mode.target_component = 0;
    //                arm_mode.command = 115;
    //                //arm_mode.confirmation = true;
    //                arm_mode.param1 = yawPos; //degree
    //                arm_mode.param2 = 10; //rate deg/sec
    //                arm_mode.param3 = -1; //direction
    //                arm_mode.param4 = 1;
    //                mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    //                uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //                //qint64 len_send = socket->write((const char*)data_buff_send,len);
    //                socket->write((const char*)data_buff_send,len);
    //            }

    //        }
    //    }else{
    //        yawPos  = heading;

    //    }

    //    if(_yolothread.frameSend.cols > 0 && _yolothread.frameSend.rows > 0){

    //        QImage qimgOriginal2((uchar*)_yolothread.frameSend.data,_yolothread.frameSend.cols,_yolothread.frameSend.rows,_yolothread.frameSend.step,QImage::Format_RGB888);
    //        ui->ShowImage->setPixmap(QPixmap::fromImage(qimgOriginal2));

    //    }

    ui->label_lat->setText(QString::number(Lat,'g',10));
    ui->label_lon->setText(QString::number(Lon,'g',10));
    ui->label_al->setText(QString::number(Altitude));
    ui->label_hd->setText(QString::number(heading));
    _maparcgis.setLonLat(Lon,Lat,heading);
}

void MainWindow::onReadyRead()
{
    QByteArray data;
    mavlink_message_t msg;
    mavlink_status_t status;

    float  show_roll ;
    data.clear();
    data = socket->readAll();

    if(data.size() < 0){
        return;
    }


    //////GPS DISTANCE///////
    for(int i = 0; i < data.size(); i++)
    {

        if( mavlink_parse_char(MAVLINK_COMM_0, (uint8_t)data[i], &msg, &status)){
            msg_global = msg;
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_GPS_RAW_INT:
                mavlink_gps_raw_int_t gps_t;
                mavlink_msg_gps_raw_int_decode(&msg,&gps_t);
                sat_vis = gps_t.satellites_visible;

                //qDebug()<< "SAT : " << sat_vis;
                break;

            case MAVLINK_MSG_ID_RC_CHANNELS:
                mavlink_radio_status_t rd;
                mavlink_msg_radio_status_decode(&msg,&rd);
                break;


            case MAVLINK_MSG_ID_ATTITUDE:
                mavlink_attitude_t att;
                mavlink_msg_attitude_decode(&msg,&att);
                roll = att.roll*Rad2Deg;
                pitch = att.pitch*Rad2Deg;
                if(roll >= 1.06908e+14){
                    roll = prev_roll;
                }
                break;

            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t hr;
                mavlink_msg_heartbeat_decode(&msg,&hr);
                flight_mode = hr.custom_mode;
                arm_disarm = hr.base_mode;
                break;

            case MAVLINK_MSG_ID_VFR_HUD:
                mavlink_vfr_hud_t vfr;
                mavlink_msg_vfr_hud_decode(&msg,&vfr);
                air_speed = vfr.airspeed;
                gnd_speed = vfr.groundspeed;
                throttle = vfr.throttle;

                break;

            case MAVLINK_MSG_ID_SYS_STATUS:
                mavlink_sys_status_t sys;
                mavlink_msg_sys_status_decode(&msg,&sys);
                voltage = sys.voltage_battery;
                safety_sw = sys.onboard_control_sensors_enabled ;
                //int temp = safety_sw - prev_sw ;

                if (safety_sw == 39911487 || safety_sw == 56691775 || safety_sw == 39943231 || safety_sw == 39914559 || safety_sw == 56688703){
                    //ui->safety_sw_button->setStyleSheet("QLabel {background-color: rgb(0, 255, 0); }");
                    indicate = true;

                }
                else if(safety_sw == 39944255 || safety_sw == 39947327 || safety_sw == 39975999 || safety_sw == 56721471 || safety_sw == 56724543 ){
                    //ui->safety_sw_button->setStyleSheet("QLabel {background-color: rgb(255, 0, 0); }");
                    //if(temp_safety != safety_sw && safety_sw != 56663087 && temp_safety != 0){
                    //    safety_sw_sound->play();
                    if(indicate == true){
                        //safety_sw_sound->play();
                        indicate = false;
                    }
                }
                if(safety_sw == 39911487 || safety_sw == 39944255 || safety_sw == 56721471){ //mannual
                    //ui->flt_mode->setText("MANUAL");
                    shooo = true;
                }
                else if(safety_sw == 39914559 || safety_sw == 39947327  || safety_sw == 56724543){ //fly
                    //ui->flt_mode->setText("FLY-BY-WIRE");
                    shooo = true;
                }
                else if(safety_sw == 39943231 || safety_sw == 39975999  || safety_sw == 56753215){ //auto
                    //ui->flt_mode->setText("AUTO");
                    if(shooo){
                        //safety_sw2_sound->play();
                        shooo = false;
                    }
                }
                if(prev_sw > safety_sw && prev_sw != 39914559 && prev_sw != 39947327 && prev_sw != 56724543 ){
                    //auto_pilot_dis_sound->play();
                }
                temp_safety = safety_sw;
                //temp = aqFabs
                prev_sw = safety_sw;
                //amp_batt = sys.current_battery;
                voltage = voltage/1000.0;
                //amp_batt = amp_batt*100;
                //ui->volt->setText(QString::number(voltage,'f',1));
                //qDebug()<< safety_sw;
                break;

            case MAVLINK_MSG_ID_BATTERY_STATUS:
                mavlink_battery_status_t bat;
                mavlink_msg_battery_status_decode(&msg,&bat);
                amp_batt = bat.current_battery;
                //qDebug()<< amp_batt;
                //ui->amp->setText(QString::number(amp_batt));
                break;

            case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: // sensor fussion GPS+ACC
                mavlink_global_position_int_t pos;
                mavlink_msg_global_position_int_decode(&msg,&pos);
                Altitude = pos.relative_alt/1000.0;
                if(Altitude == -0.001){
                    Altitude = prev_alt;
                }

                heading = pos.hdg/100.0;
                //qDebug()<< heading;
                if(heading >= 361){
                    heading = prev_heading;
                }
                //ui->graphicsView_HSI->setText(QString::number(heading));
                prev_heading = heading;
                prev_alt = Altitude;
                /////////////////// Initial data for geolocation //////////////////
                Lat = pos.lat/10000000.0;
                Lon = pos.lon/10000000.0;


                if(Lat == 0 || Lon == 0){
                    Lat = prev_lat;
                    Lon = prev_lon;
                }

                prev_lat = Lat;
                prev_lon = Lon;
                break;

            case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
                mavlink_local_position_ned_t ned;
                mavlink_msg_local_position_ned_decode(&msg,&ned);
                X_NED = ned.x;
                Y_NED = ned.y;
                //qDebug()<< amp_batt;
                //ui->amp->setText(QString::number(amp_batt));
                break;

            case MAVLINK_MSG_ID_SYSTEM_TIME:
                mavlink_system_time_t time;
                mavlink_msg_system_time_decode(&msg,&time);
                time_stamp = time.time_boot_ms;
                //qDebug()<< time_stamp;
            }
        }
    }


}

void MainWindow::on_pushButton_clicked()
{
    mavlink_message_t               msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];

    mavlink_command_long_t arm_mode = { 0 };
    arm_mode.target_system = 1;
    arm_mode.target_component = 0;
    arm_mode.command = 22;
    arm_mode.confirmation = true;
    arm_mode.param1 = 0;
    arm_mode.param7 = 50;
    mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //qint64 len_send = socket->write((const char*)data_buff_send,len);
    socket->write((const char*)data_buff_send,len);
    //socket54444.writeData((const char*)data_buff_send,len);
    //qDebug()<< "hlhlhhl";
    // socket->write((const char*)data,MAVLINK_MAX_PACKET_LEN);
    //socket->writeDatagram((const char*)data_buff_send,len, QHostAddress::LocalHost, 14551);
    qDebug()<< "Check 2" ;
}

void MainWindow::on_pushButton_arm_clicked()
{
    mavlink_message_t               msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];

    mavlink_command_long_t arm_mode = { 0 };
    arm_mode.target_system = 1;
    arm_mode.target_component = 0;
    arm_mode.command = MAV_CMD_COMPONENT_ARM_DISARM;
    arm_mode.confirmation = true;
    arm_mode.param1 = 1;
    arm_mode.param2 = 21196;
    mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //qint64 len_send = socket->write((const char*)data_buff_send,len);
    socket->write((const char*)data_buff_send,len);
    //socket54444.writeData((const char*)data_buff_send,len);
    //qDebug()<< "hlhlhhl";
    // socket->write((const char*)data,MAVLINK_MAX_PACKET_LEN);
    //socket->writeDatagram((const char*)data_buff_send,len, QHostAddress::LocalHost, 14551);
    qDebug()<< "Check 1" ;
}


void MainWindow::on_pushButton_land_clicked()
{
    mavlink_message_t               msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    mavlink_command_long_t arm_mode = { 0 };
    arm_mode.target_system = 1;
    arm_mode.target_component = 0;
    arm_mode.command = 21;
    arm_mode.confirmation = true;
    arm_mode.param1 = 0;
    arm_mode.param2 = 0;
    mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //qint64 len_send = socket->write((const char*)data_buff_send,len);
    socket->write((const char*)data_buff_send,len);
    //socket54444.writeData((const char*)data_buff_send,len);
    //qDebug()<< "hlhlhhl";
    // socket->write((const char*)data,MAVLINK_MAX_PACKET_LEN);
    //socket->writeDatagram((const char*)data_buff_send,len, QHostAddress::LocalHost, 14551);
    qDebug()<< "Check 2" ;
}


void MainWindow::on_pushButton_WP1_clicked()
{
    mavlink_message_t               msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    mavlink_set_position_target_global_int_t gps = {0};
    gps.target_system = 1;
    gps.target_component = 0;
    gps.time_boot_ms = time_stamp;
    gps.coordinate_frame = 6;
    gps.type_mask = 65528;
    gps.lat_int = 13.9182647*10000000;
    gps.lon_int = 100.6307195*10000000;
    gps.alt = 50;
    mavlink_msg_set_position_target_global_int_encode(1, 255, &msg, &gps);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    socket->write((const char*)data_buff_send,len);
    //socket54444.writeData((const char*)data_buff_send,len);

}



void MainWindow::set_yaw() //SET HDG
{
    //float hdg_zz = ui->lineEdit->text().toFloat();
    mavlink_message_t msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    mavlink_command_long_t arm_mode = { 0 };
    arm_mode.target_system = 1;
    arm_mode.target_component = 0;
    arm_mode.command = MAV_CMD_CONDITION_YAW;
    arm_mode.confirmation = true;
    arm_mode.param1 = 0;
    arm_mode.param2 = 0.5;
    arm_mode.param3 = 1;
    arm_mode.param4 = 0;
    mavlink_msg_command_long_encode(1, 255, &msg, &arm_mode);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //qint64 len_send = socket->write((const char*)data_buff_send,len);
    socket->write((const char*)data_buff_send,len);
}

void MainWindow::on_pushButton_mode_clicked()
{
    // set guide mode
    mavlink_message_t               msg;
    uint8_t data_buff_send[MAVLINK_MAX_PACKET_LEN];
    mavlink_command_long_t mav_mode = { 0 };
    mav_mode.target_system = 1;
    mav_mode.target_component = 0;
    mav_mode.command = MAV_CMD_DO_SET_MODE;
    mav_mode.param1 = 1;
    mav_mode.param2 = 4;
    mavlink_msg_command_long_encode(1, 255, &msg, &mav_mode);
    uint16_t len = mavlink_msg_to_send_buffer(data_buff_send, &msg);
    //qint64 len_send = socket->write((const char*)data_buff_send,len);
    socket->write((const char*)data_buff_send,len);
    //qDebug()<< "hlhlhhl";
    // socket->write((const char*)data,MAVLINK_MAX_PACKET_LEN);
    //socket->writeDatagram((const char*)data_buff_send,len, QHostAddress::LocalHost, 14551);
    //qDebug()<< "Check 2" ;
}





