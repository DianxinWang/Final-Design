#ifndef __USR_INIT_H__
#define __USR_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "motor.h"
#include "main.h"
Motor_InitTypeDef motor_init[4] = 
{
{
	&htim5,
	TIM_CHANNEL_2,
	TIM_CHANNEL_1,
	MOTOR_EN_1_GPIO_Port,
	MOTOR_EN_1_Pin,
	&htim1,
	ENCODER_EN_1_GPIO_Port,
	ENCODER_EN_1_Pin,
	false
},

{
	&htim8,
	TIM_CHANNEL_3,
	TIM_CHANNEL_4,
	MOTOR_EN_4_GPIO_Port,
	MOTOR_EN_4_Pin,
	&htim4,
	ENCODER_EN_3_GPIO_Port,
	ENCODER_EN_3_Pin,
	false
}
,

{
	&htim8,
	TIM_CHANNEL_2,
	TIM_CHANNEL_1,
	MOTOR_EN_3_GPIO_Port,
	MOTOR_EN_3_Pin,
	&htim3,
	ENCODER_EN_3_GPIO_Port,
	ENCODER_EN_3_Pin,
	true
},

{
	&htim5,
	TIM_CHANNEL_3,
	TIM_CHANNEL_4,
	MOTOR_EN_2_GPIO_Port,
	MOTOR_EN_2_Pin,
	&htim2,
	ENCODER_EN_2_GPIO_Port,
	ENCODER_EN_2_Pin,
	true
}
};

#ifdef __cplusplus
}
#endif
#endif
