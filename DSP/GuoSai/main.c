#include "driverlib.h"
#include "device.h"


uint16_t adcAResult0;
void initADCs(void);
void initADCSOCs(void);


void main(void){
    Device_init(); //100 MHz
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // ��ʼ��ADC����ʼ��SOCΪ���������
    initADCs();
    initADCSOCs();

    EINT;
    ERTM;

    while(1)
    {
        // �������ADCת����
        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);

        // �ȴ�ADCת����ɣ�������жϱ�־λ
        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
        {
        }
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

        // ������
        adcAResult0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

        // ���ڵ��Ե������ͣ��
        ESTOP0;
    }
}



// ��ʼ��ADC
void initADCs(void)
{
    // ADCʹ��Ƭ�ڲο�3.3V
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    // ADCʱ��4��Ƶ
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

    // ����ת������ʱ�����ж�
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // ����AD���ӳ�1ms
    ADC_enableConverter(ADCA_BASE);

    DEVICE_DELAY_US(1000);
}



// ��ʼ��ADCSOC
void initADCSOCs(void)
{
    // �������ڳ���10��ϵͳʱ��
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 10);

    // �����ж�ΪADCINT1��������
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}
