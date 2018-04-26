#include "rhpacket.h"

int IsPacketValid(unsigned char *packet) 
{
	return (
		packet[0] == PACKET_HEADER &&
		packet[3] <= DATA_SIZE_MAX &&
		packet[1] == (packet[packet[3]+5] ^ 0xFF) &&
		packet[2] == (packet[packet[3]+4] ^ 0xFF) &&
		packet[packet[3]+6] == PACKET_FOOTER);
}

PacketType GetPacketInfo(unsigned char *packet, 
						unsigned char **data, 
						uint8_t *data_size)
{
	*data = &packet[4];
	*data_size = packet[3];
	return GetPacketType(packet);
}

static PacketType GetPacketType(unsigned char *packet)
{
	return (PacketType)packet[2];
}

/*
 * CMD inner data structure
 * [Inner Type][data 1][data 2]....[data n]
 */
void ProcessCMD(RH_CMD_PROCESS_Itf hCMDProcessfunc, 
				Motor *motor, 
				unsigned char *data, 
				uint8_t data_size)
{
	switch (data[0])
	{
		/*
		 * CMD MotorStatusCTRL data structure
		 * [MotorStatusCTRL][Motor1Enable][Motor2Enable][Motor3Enable][Motor4Enable]]
		 *	0				 1			   2 			 3 				4
		 */
		case MotorStatusCTRL:
			if(data_size != 5) 
			{
				//Todo Buildlog
				break;
			}
			hCMDProcessfunc.motorStatCtrlFunc(motor, data);
			break;
		/*
		 * CMD PIDParaCTRL data structure
		 * [PIDParaCTRL][Motor1 PID kp byte1]...[Motor1 PID kp byte4]...[Motor1 PID kd byte4]...[Motor4 PID kd byte4]
		 *	0						 1						 					 4					     				12						 					48
		 */
		case PIDParaCTRL:
			if(data_size != 49) 
			{
				//Todo Buildlog
				break;
			}
			hCMDProcessfunc.pidParaCTRL(motor, data);
			break;
		/*
		 * CMD MotionCTRL data structure
		 * [MotionCTRL][Motor1 motion byte 1][Motor1 motion byte 2]...[Motor4 motion byte 2]
		 *	0			1					  2						   8
		 */
		case MotionCTRL:
			if(data_size != 9) 
			{
				//Todo Buildlog
				break;
			}
			hCMDProcessfunc.motionCTRL(motor, data);
			break;
		/*
		 * CMD MsgRet data structure
		 * [MsgRet][INTERVAL BYTE1][INTERVAL BYTE2]
		 *	0		1				 2
		 */
		case IntervalCTRL:
			if(data_size != 3) 
			{
				//Todo Buildlog
				break;
			}
			hCMDProcessfunc.intervalCTRL(motor, data);
			break;
		/*
		 * CMD MsgRet data structure
		 * [MsgRet][INTELIMIT BYTE1][INTERVAL BYTE2][INTELIMIT BYTE3][INTERVAL BYTE4]
		 *	0		1				 2
		 */
		case InteLimitCTRL:
			if(data_size != 5) 
			{
				//Todo Buildlog
				break;
			}
			hCMDProcessfunc.inteLimitCTRL(motor, data);
			break;
		default:
			break;
	}
}

void BuildMsg(	unsigned char 		*data_ptr,
				PacketDestination	dest,
				PacketType			type,
				uint8_t 			data_size, 
				unsigned char 		*packet)
{
	memset(packet, 0, 64);
	if(data_size > DATA_SIZE_MAX)
	{
		//Todo return a log msg to PC
	}
	uint8_t pos = 0;
	packet[pos++] = PACKET_HEADER;
	packet[pos++] = dest;
	packet[pos++] = type;
	packet[pos++] = data_size;
	memcpy(packet+pos, data_ptr, data_size);
	pos += data_size;
	packet[pos++] = type ^ 0xFF;
	packet[pos++] = dest ^ 0xFF;
	packet[pos++]   = PACKET_FOOTER;	
}

/*
 * ForceTraceMsg data structure
 * [Motor 1 Trace byte 1][Motor 1 Trace byte 2][Motor 1 Force byte 1]
 *	0					  1						2
 * [Motor 1 Force byte 2]...[Motor 4 Force byte 1][Motor 4 Force byte 2]
 *  3                        14                    15
 */
void BuildRH_TraceForceMsg(
	Motor				*motor,
	uint16_t			*adc,
	unsigned char 		*packet)
{
	uint8_t pos = 0;
	uint8_t data[DATA_SIZE_MAX];
	for(int i = 0; i < 4; i++)
	{
		((uint16_t *)data)[pos++] = motor[i].m_EncValue;
		((uint16_t *)data)[pos++] = adc[i];
	}
	
	BuildMsg(data, PC, RH_TRACE_FORCE_MSG, 16, packet);
}	

/*
 * EMGBendForceMsg data structure
 * [EMG channel1 byte 1][EMG channel1 byte 2][EMG channel2 byte 1]...
    0                     1                    2 
   [EMG channel4 byte 2]|Same structure as before|[Bend channel byte1]...
    7                     31                       32                     
   [Bend channe5 byte2][Forece channel byte1]...[Force channel5 byte2]
 *  41                   42                      51
 */
void BuildDG_EMGBendForceMsg(
	uint16_t			*EMG,
	uint16_t			*Bend,
	uint16_t			*Force,
	unsigned char 		*packet)
{
	uint8_t pos = 0;
	uint8_t data[DATA_SIZE_MAX];
	memset(data, 0, DATA_SIZE_MAX);
	memcpy(data+pos, EMG, 32);
	pos += 32;
	memcpy(data+pos, Bend, 10);
	pos += 10;
	memcpy(data+pos, Force, 10);
//	BuildMsg(data, PC, DG_EMG_FORCE_BENCH_MSG, 52, packet);
}
