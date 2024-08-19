#ifndef DATALINK_H
#define DATALINK_H

#include <QObject>
#include "port.h"
#include "parameters.h"
#include "structs.h"
#ifdef BUILD_GCS
#include "joystick.h"
#endif
#include "logger.h"

extern UavResponse uavResponse;
extern TrackingAntResponse trackingAntResponse;
extern Datum datum;
extern char custom_command;
extern mavlink_debug_vect_t custom_msg_1;

class Datalink : public QObject
{
    Q_OBJECT
public:
    explicit Datalink(QObject *parent = 0);
    ~Datalink();
    Port portGcs2Ob;
#ifdef BUILD_GCS
    Joystick joy;
#endif
    Logger logger;
    mavlink_global_position_int_t global_position_int;
    mavlink_local_position_ned_t  local_position_ned;
    mavlink_attitude_t            attitude;
    mavlink_attitude_quaternion_t attitude_quaternion;
    mavlink_simstate_t            simstate;
    mavlink_ahrs2_t               ahrs2;
    mavlink_timesync_t            timesync;
    mavlink_gps_global_origin_t   gps_global_origin;
    mavlink_home_position_t       home_position;
    mavlink_sys_status_t          sys_status;
    mavlink_statustext_t          statustext;
    mavlink_command_ack_t         command_ack;
    mavlink_param_value_t         param_value;
    mavlink_set_position_target_local_ned_t  set_position_target_local_ned;
    QTimer *uplinkTimer;
signals:
    void receivedTelemetryReady();

public slots:
    void updateRxMavlinkData(Port* p,unsigned short packet_size);
    void uplinkTimerCb();
    void stop();
private slots:

private:

};


#endif // DATALINK_H
