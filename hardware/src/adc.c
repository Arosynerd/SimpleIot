#include "adc.h"

// ��ʼ��
void ADC1_Init(void)
{
    // 1. ʱ������
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // CFGR��ADCPRE - 10��6��Ƶ���õ� 12MHz
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 2. GPIO����ģʽ��PB0��ģ�����룬00 00
    GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    // GPIO����ģʽ��PB1��ģ�����룬00 00
    GPIOB->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);

    // 3. ADC����
    // 3.1 ����ģʽ������ɨ��ģʽ
    ADC1->CR1 |= ADC_CR1_SCAN;

    // 3.2 ��������ת��ģʽ(����ѭ��)��
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 �����Ҷ��루Ĭ�ϣ�
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 ����ͨ��10�Ĳ���ʱ�䣬001 - 7.5��ʱ������
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP8;
    ADC1->SMPR2 |= ADC_SMPR2_SMP8_0;
    //����ͨ��9�Ĳ���ʱ�䣬001 - 7.5��ʱ������
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP9;
    ADC1->SMPR2 |= ADC_SMPR2_SMP9_0;

    // 3.5 ������ͨ����������
    // 3.5.1 �������е�ͨ������ L
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 3.5.2 ��ͨ���� 10 ���浽�����еĵ�һλ
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 8 << 0;

    // ��ͨ���� 12 ���浽�����еĵڶ�λ
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 9 << 5;

    // 3.5 ѡ���������ADת��
    // ADC1->CR2 |= ADC_CR2_EXTTRIG;
    // ADC1->CR2 |= ADC_CR2_EXTSEL;    // ѡ��ľ���SWSTART����ADת��
}
void ADC1_DMA_Init(void)
{
    // 1. ����DMAģ��ʱ��
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. ����ͨ��1�Ĵ��䷽��, DIR = 0
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // 3. ���ݿ��, SIZE = 01, 16λ
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    // 4. ��ַ���������費�����ڴ��ַҪ��
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 5. ����ѭ��ģʽ
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 6. ����DMAģʽ
    ADC1->CR2 |= ADC_CR2_DMA;

}

// ����ת��(��DMA)
void ADC1_DMA_StartConvert(uint32_t destAddr, uint8_t len)
{
    // 0. DMA����Դ��ַ��Ŀ�ĵ�ַ���Լ����ݳ���
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = destAddr;
    DMA1_Channel1->CNDTR = len;

    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 1. �ϵ绽��
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. ִ��У׼
    ADC1->CR2 |= ADC_CR2_CAL;
    // �ȴ�У׼���
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. ����ת��
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. �ȴ�ȫ��ת�����
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}

// ����ת��
void ADC1_StartConvert(void)
{
    // 1. �ϵ绽��
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. ִ��У׼
    ADC1->CR2 |= ADC_CR2_CAL;
    // �ȴ�У׼���
    while (ADC1->CR2 & ADC_CR2_CAL)
    {}

    // 3. ����ת��
    // ADC1->CR2 |= ADC_CR2_SWSTART;
    ADC1->CR2 |= ADC_CR2_ADON;

    // 4. �ȴ�ת�����
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {}
}

// ����ת�����ģ���ѹֵ
double ADC1_ReadV(void)
{
    return ADC1->DR * 100 / 4095;
}
