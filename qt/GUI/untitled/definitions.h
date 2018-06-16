#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PACKET_SIZE 64

#define RH_VID 1024
#define RH_PID 22140

#define DG_VID 1023
#define DG_PID 22140

#define PACKET_HEADER 0x1A
#define PACKET_FOOTER 0x1A
#define DATA_SIZE_MAX 0x55

//#define UNIX_IP "172.20.10.4"
#define UNIX_IP "127.0.0.1"
#define UNIX_PORT 6777

//For chartview
#define DG_RANGE 500
#define RH_RANGE 1000
#define RH_RESOLUTION 4
#define DG_RESOLUTION 4
#define DG_POINTS_NUM 22
#define DG_UPPER_FORCE 10000

//For control
#define FORCE_THREASHOLD 2000
#define RH_ENCOUNTER_MAX 32000

//For calibration
#define DG_CALIB_TIMES 1000

typedef unsigned char * packetType;
typedef unsigned short int uint16_t;
typedef short int int16_t;
typedef enum
{
    PC   	= 0,
    RH 		= 1,
    UNITY	= 2,
    DG		= 3,
} PacketDestination;

typedef enum
{
    CMD				= 0,

    RH_Status		= 1,
    MATLAB_LOG  	= 2,
    DG_LOG			= 3,
    UNITY_LOG   	= 4,

    RH_TRACE_FORCE_MSG	= 5,

    DG_EMG_FORCE_BENCH_MSG	= 6,

} PacketType;

typedef enum
{
    MotorStatusCTRL = 0,
    PIDParaCTRL  	= 1,
    MotionCTRL		= 2,
    IntervalCTRL	= 3,
    InteLimitCTRL   = 4,
    RH_StatusRET    = 5,
} CMDType;

typedef enum
{
    Motion_Err		= 0,
    MotorStatus  	= 1,
} LogIndex;

#endif // DEFINITIONS_H
