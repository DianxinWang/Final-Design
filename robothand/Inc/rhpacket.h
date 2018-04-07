/*
Packet structure:
[PACHKET_HEADER][DESTINATION][TYPE][SIZE][DATA 0][DATA 1]...[DATA n-1][^TYPE][^DESTINATION][PACHKET_FOOTER]
0				 1			  2 	3 	  4		 5		   size+3 size+4 size+5		   size+6       
*/

#ifndef __RH_PACKET_H__
#define __RH_PACKET_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "motor.h"

#define PACKET_HEADER 0x1A
#define PACKET_FOOTER 0x1A
#define DATA_SIZE_MAX 0x55

typedef enum
{
	PC 	= 0,
	RH 		= 1,
	UNITY	= 2,
	DG		= 3,
} PacketDestination;

typedef enum
{
	CMD				= 0,
	
	RH_LOG			= 1,
	MATLAB_LOG  	= 2,
	DG_LOG			= 3,
	UNITY_LOG   	= 4,
	
	RH_TRACE_FORCE_MSG	= 5,
	
	DG_FORCE_BENCH_MSG	= 6,
	
} PacketType;

typedef enum
{
	MotorStatusCTRL = 0,
	PIDParaCTRL  	= 1,
	MotionCTRL		= 2,
	FrequencCTRL	= 3,
} CMDType;

typedef enum
{
	Motion_Err		= 0,
	MotorStatus  	= 1,
} LogIndex;

typedef struct
{
	void (* motorStatCtrlFunc)(Motor *,unsigned char *);
	void (* pidParaCTRL)(Motor *,unsigned char *);
	void (* motionCTRL)(Motor *,unsigned char *);
	void (* frequencCTRL)(Motor *,unsigned char *);
} RH_CMD_PROCESS_Itf;

int IsPacketValid(unsigned char *packet);
PacketType GetPacketInfo(unsigned char *packet, unsigned char **data, uint8_t *data_size);
static PacketType GetPacketType(unsigned char *packet);

void ProcessCMD(
	RH_CMD_PROCESS_Itf 	hCMDProcessfunc, 
	Motor 				*motor, 
	unsigned char 		*data, 
	uint8_t 			data_size);
				
void BuildMsg(	
	unsigned char 		*data_ptr,
	PacketDestination	dest,
	PacketType			type,
	uint8_t 			data_size, 
	unsigned char 		*packet);
				
void BuildRH_TraceForceMsg(
	Motor				*motor,
	uint16_t			*adc,
	unsigned char 		*packet);
/*void BuildLog(	PacketDestination	dest
				LogIndex 			index);
*/

#ifdef __cplusplus
}
#endif
#endif //__RH_PACKET_H__
