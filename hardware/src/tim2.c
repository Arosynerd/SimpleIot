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
    // 1. ����ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. GPIO����ģʽ��PA1������������� CNF-10��MODE-11
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. ��ʱ������
    // 3.1 Ԥ��Ƶֵ 7199���õ�10000Hz
    TIM2->PSC = 7199;

    // 3.2 ��װ��ֵ��99��ÿ�� 10ms ���һ��
    TIM2->ARR = 99;

    // 3.3 ��������
    TIM2->CR1 &= ~TIM_CR1_DIR;

    // 3.4 ����ͨ��2��CCRֵ
    TIM2->CCR2 = 100;

    // 3.5 ����ͨ��2Ϊ���ģʽ
    TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 3.6 ����ͨ��2ΪPWM1ģʽ��OC2M-110
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    // 3.7 ʹ�����ͨ��
    TIM2->CCER |= TIM_CCER_CC2E;
}

// ����ʱ���Ŀ��ذ�װ�ɺ���
void TIM2_Start(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_Stop(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

// ����ռ�ձȵĺ���
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
