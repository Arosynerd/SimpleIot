/*
 * @Author: wushengran
 * @Date: 2024-06-03 15:28:03
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __TIM2_H
#define __TIM2_H

#include "stm32f10x.h"



void TIM2_Init(void);

// ����ʱ���Ŀ��ذ�װ�ɺ���
void TIM2_Start(void);
void TIM2_Stop(void);

// ����ռ�ձȵĺ���
void TIM2_SetDutyCycle(uint8_t dutyCycle);

// �������ȵĺ���
void TIM2_SetBrightness(u8 light);
#endif
