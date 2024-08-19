#ifndef MYMAVLINK_H
#define MYMAVLINK_H


//#include <ardupilotmega/mavlink.h>
#include <QDebug>

class myMavlink
{
public:
    myMavlink();

    double pi = 3.14159265359;
    double Rad2Deg = 180 / pi;
    double Deg2rad = pi / 180.0;
    double roll = 0;
    double pitch = 0;
    double yaw = 0;
    double voltage_battery = 0;
    double Altitude = 0.0;
    double prev_alt = 0.0;
    double Lat = 0.0;
    double Lon = 0.0;
    int heading = 0;
    bool bArm = false;
    bool bNewData = false;

};

#endif // MYMAVLINK_H
