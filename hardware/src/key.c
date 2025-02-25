
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "key.h"
#include "delay.h"
#include "led.h"

/*
************************************************************
*	�������ƣ�	Led_Init
*
*	�������ܣ�	��������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Key_Init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    EXTI_InitTypeDef exti_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    // ��GPIOB��AFIO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // ����Ϊ��������ģʽ
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    // ����ʼ����Pin�Ÿ�ΪPB13
    gpio_initstruct.GPIO_Pin = GPIO_Pin_13;
    // ����ģʽ�£��ٶ����������壬�ɺ��Դ�����
    // gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz; 
    // ��ʼ��GPIOB
    GPIO_Init(GPIOB, &gpio_initstruct);

    // ����EXTI�ߣ�������Դ��ΪGPIOB��Pin13
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    // ����EXTI
    exti_initstruct.EXTI_Line = EXTI_Line13;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

    // ����NVIC
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);
}

// �жϷ�����ҲҪ��Ӧ�޸�
void EXTI15_10_IRQHandler(void)
{
    DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
    {
        if(led_info.Led_Status==LED_ON) Led_Set(LED_OFF);
		else Led_Set(LED_ON);
    }
    // ����жϱ�־λ
    EXTI_ClearITPendingBit(EXTI_Line13);
}
