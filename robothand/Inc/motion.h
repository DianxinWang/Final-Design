/* ========================================
 * 
 * ========================================
*/


#ifndef __MOTION_H__
#define __MOTION_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <math.h>
	 
#include "motor.h"

typedef struct
{
	double PositionExpect;
  double PositionCurrent;
	
  double VelocityExpect;
  double VelocityCurrent;
	
  double PWM;
} PIDVARIABLE;

typedef struct 
{
    float Kp;
    float Ki;
    float Kd;
    
    float Error;
    float LastError;
    float Integral;
    float Differ;
    
} PIDPARA;		

void DetectVelocity(Motor* motor, PIDVARIABLE* velocity);
void CalcSpeedPID(Motor* motor, PIDPARA* MotorPID, PIDVARIABLE *Velocity);
void CalcPositionPID(Motor* motor, PIDPARA *PositionPID, PIDVARIABLE *Position);

#ifdef __cplusplus
}
#endif
#endif



/* [] END OF FILE */
