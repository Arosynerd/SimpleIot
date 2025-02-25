/*
 * @Author: wushengran
 * @Date: 2024-06-04 13:58:28
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim2.h"
#include "adc.h"

void TIM2_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. GPIO工作模式，PA1：复用推挽输出 CNF-10，MODE-11
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. 定时器配置
    // 3.1 预分频值 7199，得到10000Hz
    TIM2->PSC = 7199;

    // 3.2 重装载值，99，每隔 10ms 溢出一次
    TIM2->ARR = 99;

    // 3.3 计数方向
    TIM2->CR1 &= ~TIM_CR1_DIR;

    // 3.4 设置通道2的CCR值
    TIM2->CCR2 = 100;

    // 3.5 配置通道2为输出模式
    TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 3.6 配置通道2为PWM1模式，OC2M-110
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    // 3.7 使能输出通道
    TIM2->CCER |= TIM_CCER_CC2E;
}

// 将定时器的开关包装成函数
void TIM2_Start(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_Stop(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

// 设置占空比的函数
void TIM2_SetDutyCycle(uint8_t dutyCycle)
{
    TIM2->CCR2 = dutyCycle;
}
void TIM2_SetBrightness(u8 light){
    if(light < 60&&light>50){
        TIM2_SetDutyCycle(30); 
    }
    else if(light < 50&&light>30){
        TIM2_SetDutyCycle(50); 
    }
    else if(light < 30){
        TIM2_SetDutyCycle(100);
    }
}
