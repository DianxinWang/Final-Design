/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "motion.h"

/*
 * Calculate the Motor PWMIN with PID
 * Para-----"*Motor" is the pointer to the Motor parameter structure
 * Para-----"*MotorPID" is the pointer to the Motor PID parameter struture
 * Return---status: 0 is success ;-1 is false
*/
void CalcSpeedPID(Motor* motor, PIDPARA* MotorPID, PIDVARIABLE *Velocity)
{
		MotorPID->LastError = MotorPID->Error;
		MotorPID->Error = Velocity->VelocityExpect - Velocity->VelocityCurrent;
		MotorPID->Integral += MotorPID->Error;
		MotorPID->Differ = (MotorPID->Error - MotorPID->LastError) * motor->m_SampleFrequence;
	
		Velocity->PWM = MotorPID->Error*MotorPID->Kp + MotorPID->Integral*MotorPID->Ki + MotorPID->Differ*MotorPID->Kd;
		Velocity->PWM = (Velocity->PWM <= 100) && (Velocity->PWM >= -100) ? \
										Velocity->PWM : \
									  (Velocity->PWM > 100 ? 100 : -100);
		motor->setDutyratio(Velocity->PWM);
		return;
										
}
/*
 * Calculate the Motor velocity expected with PID
 * Para-----"*Position" is the pointer to the Motor Position parameter structure
 * Para-----"*PositionPID" is the pointer to the Motor Position PID parameter struture
 * Return---status: 0 is success ;-1 is false
*/
void CalcPositionPID(Motor* motor, PIDPARA *PositionPID, PIDVARIABLE *Position)
{
		Position->PositionCurrent += Position->VelocityCurrent / motor->m_SampleFrequence;
		PositionPID->LastError = PositionPID->Error;
		PositionPID->Error = Position->PositionExpect - Position->PositionCurrent;
		PositionPID->Integral += PositionPID->Error;
		PositionPID->Differ = (PositionPID->Error - PositionPID->LastError) * motor->m_SampleFrequence;
	
		Position->VelocityExpect = PositionPID->Error*PositionPID->Kp \
														 + PositionPID->Integral*PositionPID->Ki \
														 + PositionPID->Differ*PositionPID->Kd;
	
		return;
		
}
/*
 * Detect the Motor velocity current
 * Para-----"*Motor" is the pointer to the Motor parameter structure,the results will be sent to 
 * Para-----"TIMx" indicate the Encoder Timer port
 * Para-----"time" is sample time 
 * Return---status: 0 is success ;-1 is false
*/
void DetectVelocity(Motor* motor, PIDVARIABLE* velocity)
{    
	
	int32_t EncoderDelta;
    motor->m_EncoderPrevious = motor->m_EncoderCurrent;
    motor->m_EncoderCurrent = motor->getEncoderValue();            //编码器的读数
    
		if( (motor->m_EncoderCurrent < 0x2000) && (motor->m_EncoderPrevious > 0xd000) )           // 超过上界//
			{
				EncoderDelta = motor->m_EncoderCurrent - motor->m_EncoderPrevious + 0x10000;   //正转//                                
	   	}
	  else if( (motor->m_EncoderCurrent > 0xd000) && (motor->m_EncoderPrevious < 0x2000) )    // 超过下界//
	   	{
				EncoderDelta= -(motor->m_EncoderPrevious - motor->m_EncoderCurrent + 0x10000); //正转//                                                                               
	   	}		
	  else if( motor->m_EncoderCurrent > motor->m_EncoderPrevious )
	   	{
				EncoderDelta = motor->m_EncoderCurrent - motor->m_EncoderPrevious;             //正转//
	   	}
	  else
	   	{
				EncoderDelta= -(motor->m_EncoderPrevious - motor->m_EncoderCurrent);           //反转//                                                     
	   	} 
		
		velocity->VelocityCurrent = EncoderDelta * motor->m_SampleFrequence;
			
		return;
	  
}




