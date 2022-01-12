#include "driverlib.h"
#include "device.h"


uint16_t adcAResult0;
void initADCs(void);
void initADCSOCs(void);


void main(void){
    Device_init(); //100 MHz
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // 初始化ADC，初始化SOC为软件触发。
    initADCs();
    initADCSOCs();

    EINT;
    ERTM;

    while(1)
    {
        // 软件触发ADC转换。
        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);

        // 等待ADC转换完成，并清除中断标志位
        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
        {
        }
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

        // 储存结果
        adcAResult0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

        // 用于调试的软件暂停。
        ESTOP0;
    }
}



// 初始化ADC
void initADCs(void)
{
    // ADC使用片内参考3.3V
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    // ADC时钟4分频
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);

    // 设置转换结束时触发中断
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // 启动AD并延迟1ms
    ADC_enableConverter(ADCA_BASE);

    DEVICE_DELAY_US(1000);
}



// 初始化ADCSOC
void initADCSOCs(void)
{
    // 采样窗口持续10个系统时钟
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 10);

    // 设置中断为ADCINT1，并启用
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}
