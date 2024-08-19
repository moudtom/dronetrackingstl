#ifndef MAINWINDOW_H
#define MAINWINDOW_H
namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
class GraphicsOverlay;
}
}

#include <QMainWindow>
#include <QTimer>
#include "mytcpsocket.h"
#include "mavlink1/ardupilotmega/mavlink.h"
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QUdpSocket>
#include "myyolothread.h"
#include "mymaparcgis.h"
#include "Dense"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void controller();
public slots:
    void timmer_loop();
    void onReadyRead();
    void set_yaw();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_arm_clicked();

    void on_pushButton_land_clicked();

    void on_pushButton_WP1_clicked();

    void on_pushButton_mode_clicked();



private:
    Ui::MainWindow *ui;
    QTimer *myTimer;
    myTcpSocket socket54444;
    myTcpSocket socket58888;
    QTcpSocket *socket;
    myYoloThread _yolothread;
    myMapArcgis _maparcgis;

       QString _str_server_status;
       double Lat = 0.00f;
       double Lon = 0.00f;
       uint32_t time_stamp;

       bool control_loop = false;
       float roll = 0;
       float pitch = 0;
       int heading = 0;
       int prev_heading;
       int Sat = 0;
       double Altitude = 0.0f;
       double prev_alt = 0.0f;
       bool flyto = false;
       double home_lat ;
       double home_lon ;
       double prev_lat;
       double prev_lon;
       bool GeoStart;
       //////////// Geolocation ///////////////////////
       //define variable
       double lat0 = 13.879286;
       double lon0 = 100.586789;
       //double lat2 = 13.918021;
       //double lon2 = 100.630692;
       bool noInitial ;
       //double lat0 = 0.00f;
       //double lon0 = 0.00f;
       double lat2 = 0.00f;
       double lon2 = 0.00f;
       double lat3 = 0.00f;
       double lon3 = 0.00f;
       double distance = 0.00f;
       double latfill = 0.00f;
       double lonfill = 0.00f;

       double X_NED = 0.00f;
       double Y_NED = 0.00f;
        double Z_NED = 0.00f;

       //GPS location
       float Xmav = 0.0;
       float Ymav = 0.0;
       float Hmav = 0.0; // meter
       int safety_sw ;
       int prev_sw;
       //angle roll pitch yaw
       int rollx = 0;
       int pitchx = 0;
       int yawx = 0;
       QByteArray data;
       //gimbal angle
       int el = 90;
       int al = 0;
       QTimer         *tmrTimer; // timer
       QTimer         *sTime;
       float voltage;
       int16_t amp_batt;
       float air_speed;
       float gnd_speed;
       uint32_t flight_mode ;
       uint8_t arm_disarm;
       uint16_t throttle;
       quint16 senderPort;
       float prev_roll;
       int show_rssi;
       double temp_lat;
       double temp_lon;
       int temp_safety = 0;
       bool connect_click = false;
       int prev_arm = 0;
       int prev_mode = 0 ;
       bool indicate = false;
       uint8_t sat_vis;
       int altitude_feet = 0;
       //// RADIO /////
       int RSSI;
       bool shooo = false;
       bool GPS_distance = false;

       /////////WATCH///////////
       QTimer *ptimer;
       QTimer *ntimer;

       QString minStr;
       QString hrStr;
       QString secStr;
       QString Str;

       QHostAddress fromAddress;
       mavlink_message_t msg_global;

       ///////Counter/////
       int flt_number = 0 ;

       ///Control///
       float vxx = 0;
       float vyy = 0;

       int _heading = 0;
       double heading_ref, Vel_Ref_X, Vel_Ref_Y;
       double pre_err;

       //camera
       int Px = 0;
       int Py = 0;
       int Pz = 0;
       float fx =2.8;
       float fy =2.8;
       float ox = 320;
       float oy = 240;
       double Rad2Deg = 180/3.14;
       double Deg2rad = 3.14/180.0;

       double objX = 0.00f;
       double objY = 0.00f;


};
#endif // MAINWINDOW_H
