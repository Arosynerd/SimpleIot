/*
 * @Author: wushengran
 * @Date: 2024-06-11 15:02:52
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

// 初始化
void ADC1_Init(void);

void ADC1_DMA_Init(void);

// 开启转换
void ADC1_StartConvert(void);

void ADC1_DMA_Init(void);

// 返回转换后的模拟电压值
double ADC1_ReadV(void);

#endif
