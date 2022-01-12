#include "driverlib.h"
#include "device.h"

uint16_t count = 0;
uint16_t adcAResult[1000];
void initADCs(void);
void initADCSOCs(void);


void main(void)
{
    //100 MHz
    Device_init();
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
        if(count == 1000){
            count = 0;
        }
        adcAResult[count] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
        count++;


        // ���ڵ��Ե������ͣ��
        // Software breakpoint. At this point, conversion results are stored in adcAResult0,
        //ESTOP0;
    }
}



// ��ʼ��ADC
void initADCs(void)
{
    // ADCʹ��Ƭ�ڲο�3.3V
    // Setup VREF as internal
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    // ADCʱ��4��Ƶ
    // Set ADCCLK divider to /4
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_1_0);

    // ����ת������ʱ�����ж�
    // Set pulse positions to late
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // ����AD���ӳ�1ms
    // Power up the ADCs and then delay for 1 ms
    ADC_enableConverter(ADCA_BASE);

    DEVICE_DELAY_US(1000);
}



// ��ʼ��ADCSOC
void initADCSOCs(void)
{
    // �������ڳ���10��ϵͳʱ��
    // - SOC0 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 10);

    // �����ж�ΪADCINT1��������
    // Set SOC1 to set the interrupt 1 flag. Enable the interrupt and make sure its flag is cleared.
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}
