#include "adc.h"

// 初始化
void ADC1_Init(void)
{
    // 1. 时钟配置
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // CFGR：ADCPRE - 10，6分频，得到 12MHz
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 2. GPIO工作模式，PB0，模拟输入，00 00
    GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    // GPIO工作模式，PB1，模拟输入，00 00
    GPIOB->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);

    // 3. ADC配置
    // 3.1 工作模式：禁用扫描模式
    ADC1->CR1 |= ADC_CR1_SCAN;

    // 3.2 启用连续转换模式(单曲循环)、
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 数据右对齐（默认）
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 设置通道10的采样时间，001 - 7.5个时钟周期
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP8;
    ADC1->SMPR2 |= ADC_SMPR2_SMP8_0;
    //设置通道9的采样时间，001 - 7.5个时钟周期
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP9;
    ADC1->SMPR2 |= ADC_SMPR2_SMP9_0;

    // 3.5 规则组通道序列配置
    // 3.5.1 规则组中的通道个数 L
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 3.5.2 将通道号 10 保存到序列中的第一位
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 8 << 0;

    // 将通道号 12 保存到序列中的第二位
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 9 << 5;

    // 3.5 选择软件触发AD转换
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;    // 选择的就是SWSTART控制AD转换
}
void ADC1_DMA_Init(void)
{
    // 1. 开启DMA模块时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. 设置通道1的传输方向, DIR = 0
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // 3. 数据宽度, SIZE = 01, 16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    // 4. 地址自增，外设不增，内存地址要增
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 5. 开启循环模式
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 6. 开启DMA模式
    ADC1->CR2 |= ADC_CR2_DMA;

}

// 开启转换(带DMA)
void ADC1_DMA_StartConvert(uint32_t destAddr, uint8_t len)
{
    // 0. DMA配置源地址和目的地址，以及数据长度
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = destAddr;
    DMA1_Channel1->CNDTR = len;

    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 1. 上电唤醒
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. 执行校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 等待校准完成
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. 启动转换
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. 等待全部转换完成
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}

// 开启转换
void ADC1_StartConvert(void)
{
    // 1. 上电唤醒
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. 执行校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 等待校准完成
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. 启动转换
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. 等待转换完成
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}

// 返回转换后的模拟电压值
double ADC1_ReadV(void)
{
    return ADC1->DR * 100 / 4095;
}
