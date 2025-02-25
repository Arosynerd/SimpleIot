
//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "key.h"
#include "delay.h"
#include "led.h"

/*
************************************************************
*	函数名称：	Led_Init
*
*	函数功能：	蜂鸣器初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Key_Init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    EXTI_InitTypeDef exti_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    // 打开GPIOB和AFIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 设置为上拉输入模式
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    // 将初始化的Pin脚改为PB13
    gpio_initstruct.GPIO_Pin = GPIO_Pin_13;
    // 输入模式下，速度设置无意义，可忽略此设置
    // gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz; 
    // 初始化GPIOB
    GPIO_Init(GPIOB, &gpio_initstruct);

    // 配置EXTI线，将引脚源改为GPIOB和Pin13
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    // 配置EXTI
    exti_initstruct.EXTI_Line = EXTI_Line13;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

    // 配置NVIC
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);
}

// 中断服务函数也要相应修改
void EXTI15_10_IRQHandler(void)
{
    DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
    {
        if(led_info.Led_Status==LED_ON) Led_Set(LED_OFF);
		else Led_Set(LED_ON);
    }
    // 清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line13);
}
