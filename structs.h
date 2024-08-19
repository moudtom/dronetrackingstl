#ifndef STRUCTS_H
#define STRUCTS_H

struct UavRequest{
    char   flight_mode;
    double orbit_lat;
    double orbit_lon;
    unsigned short orbit_rad;
    unsigned char airspeed_cmd;
    unsigned short altitude_cmd;
    unsigned short heading_cmd;
    unsigned short pan_cmd;
    unsigned short tilt_cmd;
    unsigned char  camera_cmd;
    unsigned char  gimbal_mode;
    unsigned char next_wp;
    unsigned char ack_id;
    bool resetRTH;
    bool navLightOn;
    //not in protocol
    unsigned short orbit_altitude;
    unsigned char orbit_airspeed_cmd;
    unsigned char chute_deploy_by;
};

struct UavResponse{
    float airspeed;
    float groundspeed;
    float p;
    float q;
    float r;
    double roll;
    double pitch;
    double heading;
    double altitude;
    float gpsAltitude;
    float accx;
    float accy;
    float accz;
    double lat;
    double lon;
    double x[3];
    double v[3];
    double x_c[3];
    double v_c[3];
    short cross_track;
    short along_track;
    unsigned short pwm_aileron;
    unsigned short pwm_elevator;
    unsigned short pwm_throttle;
    unsigned short pwm_flap;
    unsigned short pwm_auto;
    float desired_airspeed;
    float desired_roll;
    float desired_pitch;
    float desired_heading;
    float desired_altitude;
    unsigned short distance_to_wp;
    unsigned char throttle_percent;
    unsigned char wp_number;
    unsigned char total_wp;
    unsigned char auto_manual_mode;
    unsigned char flight_mode;
    unsigned char sat_in_view;
    float   fcs_batt_volt;
    float   esc_batt_volt;
    short current_amp;
    unsigned short pwr_used;
    unsigned char fcs_status;
    unsigned char ack;
    double mag_heading;
    double gimbal_pan_angle;
    double gimbal_tilt_angle;
    unsigned short zoom_position;
    char modem_rssi;
    unsigned char modem_1s_cnt;
    unsigned char modem_temperature;
    unsigned char vdl_temperature;
    unsigned char fcs_temperature;
    unsigned char ack2;
    unsigned short rpm;
    unsigned char vdl_auto_manual;
    unsigned char vdl_power;

    bool gyro_status;
    bool acc_status;
    bool mag_status;
    bool baro_status;
    bool airspeed_status;
    bool gps_status;
    bool dma_status;
    bool looptime_status;

    int batt_percent;
    float hdop;
    float vdop;

    double home_lat; //mavlink only
    double home_lon;
    //---------------------
    //not exist in current packet
    float climb_rate;
};

 struct WaypointRequest{
    unsigned char total_wp;
    unsigned char wp_number;
    double lat;
    double lon;
    unsigned int altitude;
    unsigned int airspeed;
};

struct GeolocateOutput{
    double lat;
    double lon;
    bool valid;
};

struct SensorZeroRequest{
    unsigned char type;
};

struct SensorZeroResponse{
    unsigned char type;
    float offset;
    float airspeed_offset;
    float altitude_offset;
    bool airspeedIsSet;
    bool altitudeIsSet;
};

struct ReturnHomeSetRequest{
    unsigned short fly_altitude;
    unsigned short home_altitude;
    unsigned char speed;
    unsigned short timeout;
};

struct ReturnHomeReadRequest{
    unsigned char dummy;
};

struct ReturnHomeResponse{
    unsigned short fly_altitude;
    unsigned short home_altitude;
    unsigned char speed;
    unsigned short timeout;
};

struct TrackingAntRequest{
    unsigned char mode;
    unsigned char antenna_select;
    float heading_cmd;
    float tilt_cmd;
};

struct TrackingAntResponse{
    double pitch;
    unsigned short heading;
    double lat;
    double lon;
    double gpsAltitude;
    double dist_to_uav;
    unsigned char sat_in_view;
    char   rssi;
    float  batt;
    char   omni_rssi;
    char   dir_rssi;
    char   vdl_rssi;
    unsigned char mode;
    unsigned char selected_antenna;
    float temperature;
    float gcs_box_batt;
    char status2;
};

struct WaypointLink{
    unsigned char wp_num;
    double lat;
    double lon;
    unsigned short altitude;
    unsigned short airspeed;
    float gpsAltitude;
};

struct Datum{
    double lat0;
    double lon0;
    double lat0_rad;
    double lat_to_m;
    double lon_to_m;
    double m_to_lat;
    double m_to_lon;
    double gpsAltitude0;
    bool exist;
};

struct MagCalibRequest{
    unsigned char trigger;
};

struct MagCalibResponse{
    float Xsf;
    float Ysf;
    float Xoff;
    float Yoff;
};

#endif
