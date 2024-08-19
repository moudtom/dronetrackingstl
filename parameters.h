#ifndef PARAMETERS
#define PARAMETERS


#define DEG2RAD 0.01745329251
#define RAD2DEG  57.2957795
#define PI_2 1.570796326794897
#define KMHR2MPS 0.277778

#define EARTH_RADIUS  63781370

#define GEO_COORD_LATLON 0
#define GEO_COORD_UTM 1
#define GEO_COORD_MGRS 2

#define AIRSPEED_MIN 50
#define AIRSPEED_MAX 80

#define MSX_AIRSPEED_MIN_MPS 9
#define MSX_AIRSPEED_MAX_MPS 16

#define ALTITUDE_MIN 0
#define ALTITUDE_MAX 500

#define PARACHUTE_ALTITUDE_MIN 10

#define HEADING_MIN 0
#define HEADING_MAX 360

#define TIMEOUT_MIN 5
#define TIMEOUT_MAX 300

#define ORBIT_RADIUS_MIN 100
#define ORBIT_RADIUS_MAX 500

#define DEFAULT_ORBIT_RADIUS 200.0
#define DEFAULT_AIRSPEED 55
#define MSX_DEFAULT_AIRSPEED_MPS 11

#define AIRCRAFT_TEAGLEEYES 1
#define AIRCRAFT_M_SOLAR_X  2

#define DATALINK_TYPE_TEAGLEEYES 1
#define DATALINK_TYPE_MAVLINK    2

#define PACKET_NONE 0

// ardupilot modes
namespace Mode
{
    enum Number : uint8_t {
        MANUAL        = 0,
        CIRCLE        = 1,
        STABILIZE     = 2,
        TRAINING      = 3,
        ACRO          = 4,
        FLY_BY_WIRE_A = 5,
        FLY_BY_WIRE_B = 6,
        CRUISE        = 7,
        AUTOTUNE      = 8,
        AUTO          = 10,
        RTL           = 11,
        LOITER        = 12,
        TAKEOFF       = 13,
        AVOID_ADSB    = 14,
        GUIDED        = 15,
        INITIALISING  = 16,
        QSTABILIZE    = 17,
        QHOVER        = 18,
        QLOITER       = 19,
        QLAND         = 20,
        QRTL          = 21,
        QAUTOTUNE     = 22,
        QACRO         = 23,
        THERMAL       = 24,
        LOITER_ALT_QLAND = 25,
    };
}

#endif // PARAMETERS

