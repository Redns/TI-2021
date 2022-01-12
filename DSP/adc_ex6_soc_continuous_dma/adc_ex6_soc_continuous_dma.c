#include "driverlib.h"
#include "device.h"
#include "board.h"


//
// Function Prototypes
//
__interrupt void dmach1ISR(void);

void configureEPWM(uint32_t epwmBase);
void initializeDMA(void);
void configureDMAChannels(void);

#define RESULTS_BUFFER_SIZE     1024    // 缓冲区大小（必须是16的整数倍）
uint32_t sampleWindow = 800U;

#pragma DATA_SECTION(myADC0DataBuffer, "ramgs0");
#pragma DATA_SECTION(myADC1DataBuffer, "ramgs0");
uint16_t myADC0DataBuffer[RESULTS_BUFFER_SIZE];
uint16_t myADC1DataBuffer[RESULTS_BUFFER_SIZE];
volatile uint16_t done;

void main(void)
{
    uint16_t resultsIndex;

    // 配置系统时钟
    // sysclk:100MHz
    Device_init();

    // 解锁GPIOA、GPIOB、GPIOH
    Device_initGPIO();

    // 清除所有中断使能和中断标志
    Interrupt_initModule();

    // 链接PIE向量表，使其指向默认中断服务函数
    Interrupt_initVectorTable();

    // Board Initializatrion
    // - Configure the ADCA & ADCC and power it up
    // - Setup the ADC for continuous conversions on channels A3 and C3
    // - Set up ISR for ADCA INT1 - occurs after first conversion
    // - Enable specific PIE & CPU interrupts: ADCA INT1 - Group 1, interrupt 1
    Board_init();
    ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER6, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER7, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER8, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER9, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER10, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER11, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER12, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER13, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER14, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);
    ADC_setupSOC(myADC1_BASE, ADC_SOC_NUMBER15, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, sampleWindow);


    // Set up ISRs used by this example
    // ISR for DMA ch1 - occurs when DMA transfer is complete
    Interrupt_register(INT_DMA_CH1, &dmach1ISR);


    // Enable specific PIE & CPU interrupts:
    // DMA interrupt - Group 7, interrupt 1
    Interrupt_enable(INT_DMA_CH1);


    // Stop the ePWM clock
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    // Call the set up function for ePWM 2
    configureEPWM(EPWM2_BASE);


    // Start the ePWM clock
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    // Initialize the DMA & configure DMA channels 1 & 2
    initializeDMA();
    configureDMAChannels();

    // Initialize results buffer
    for(resultsIndex = 0; resultsIndex < RESULTS_BUFFER_SIZE; resultsIndex++)
    {
        myADC0DataBuffer[resultsIndex] = 0;
        myADC1DataBuffer[resultsIndex] = 0;
    }


    // Clearing all pending interrupt flags
    DMA_clearTriggerFlag(DMA_CH1_BASE);   // DMA channel 1
    DMA_clearTriggerFlag(DMA_CH2_BASE);   // DMA channel 2
    HWREGH(myADC0_BASE + ADC_O_INTFLGCLR) = 0x3U; // ADCA
    HWREGH(myADC1_BASE + ADC_O_INTFLGCLR) = 0x3U; // ADCC
    EPWM_forceADCTriggerEventCountInit(EPWM2_BASE, EPWM_SOC_A);     // EPWM2 SOCA
    EPWM_clearADCTriggerFlag(EPWM2_BASE, EPWM_SOC_A);               // EPWM2 SOCA


    // Enable continuous operation by setting the last SOC to re-trigger the first
    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER0,    // ADCA
                               ADC_INT_SOC_TRIGGER_ADCINT2);
    ADC_setInterruptSOCTrigger(myADC1_BASE, ADC_SOC_NUMBER0,    // ADCC
                               ADC_INT_SOC_TRIGGER_ADCINT2);

    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Start DMA
    //
    done = 0;
    DMA_startChannel(DMA_CH1_BASE);
    DMA_startChannel(DMA_CH2_BASE);

    //
    // Finally, enable the SOCA trigger from ePWM. This will kick off
    // conversions at the next ePWM event.
    //
    EPWM_enableADCTrigger(EPWM2_BASE, EPWM_SOC_A);

    //
    // Loop until the ISR signals the transfer is complete
    //
//    while(done == 0)
//    {
//        __asm(" NOP");
//    }
//    ESTOP0;
}

//
// adcA1ISR - This is called after the very first conversion and will disable
//                      the ePWM SOC to avoid re-triggering problems.
//
#pragma CODE_SECTION(adcA1ISR, ".TI.ramfunc");
__interrupt void adcA1ISR(void)
{
    //
    // Remove ePWM trigger
    //
    EPWM_disableADCTrigger(EPWM2_BASE, EPWM_SOC_A);

    //
    // Disable this interrupt from happening again
    //
    //Interrupt_disable(INT_ADCA1);

    //
    // Acknowledge interrupt
    //
    Interrupt_clearACKGroup(INT_myADC0_1_INTERRUPT_ACK_GROUP);
}

//
// dmach1ISR - This is called at the end of the DMA transfer, the conversions
//              are stopped by removing the trigger of the first SOC from
//              the last.
//
#pragma CODE_SECTION(dmach1ISR, ".TI.ramfunc");
__interrupt void dmach1ISR(void)
{
    //
    // Stop the ADC by removing the trigger for SOC0
    //
    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER0,
                               ADC_INT_SOC_TRIGGER_NONE);
    ADC_setInterruptSOCTrigger(myADC1_BASE, ADC_SOC_NUMBER0,
                               ADC_INT_SOC_TRIGGER_NONE);
    done = 1;

    //
    // Acknowledge interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
}

//
// configureEPWM - Set up the ePWM2 module so that the A output has a period
//                 of 40us with a 50% duty. The SOCA signal is coincident with
//                 the rising edge of this.
//
void configureEPWM(uint32_t epwmBase)
{
    //
    // Make the timer count up with a period of 40us
    //
    HWREGH(epwmBase + EPWM_O_TBCTL) = 0x0000U;
    EPWM_setTimeBasePeriod(epwmBase, 4000U);

    //
    // Set the A output on zero and reset on CMPA
    //
    EPWM_setActionQualifierAction(epwmBase, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(epwmBase, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    //
    // Set CMPA to 20us to get a 50% duty
    //
    EPWM_setCounterCompareValue(epwmBase, EPWM_COUNTER_COMPARE_A, 2000U);

    //
    // Start ADC when timer equals zero (note: don't enable yet)
    //
    EPWM_setADCTriggerSource(epwmBase, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(epwmBase, EPWM_SOC_A, 1U);

    //
    // Enable initialization of the SOCA event counter. Since we are
    // disabling the ETSEL.SOCAEN bit, we need a way to reset the SOCACNT.
    // Hence, enable the counter initialize control.
    //
    EPWM_enableADCTriggerEventCountInit(epwmBase, EPWM_SOC_A);
}

//
// initializeDMA - Initialize DMA through hard reset
//
void initializeDMA(void)
{
    //
    // Perform a hard reset on DMA
    //
    DMA_initController();

    //
    // Allow DMA to run free on emulation suspend
    //
    DMA_setEmulationMode(DMA_EMULATION_FREE_RUN);
}

//
// configureDMAChannels - Initialize DMA ch 1 to transfer ADCA results
//                        and DMA ch 2 to transfer ADCB results
//
void configureDMAChannels(void)
{
    //
    // DMA channel 1 set up for ADCA
    //
    DMA_configAddresses(DMA_CH1_BASE, (uint16_t *)&myADC0DataBuffer,
                        (uint16_t *)ADCARESULT_BASE);

    //
    // Perform enough 16-word bursts to fill the results buffer. Data will be
    // transferred 32 bits at a time hence the address steps below.
    //
    DMA_configBurst(DMA_CH1_BASE, 16, 2, 2);
    DMA_configTransfer(DMA_CH1_BASE, (RESULTS_BUFFER_SIZE >> 4), -14, 2);
    DMA_configMode(DMA_CH1_BASE, DMA_TRIGGER_ADCA2,
                   (DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_DISABLE |
                    DMA_CFG_SIZE_32BIT));

    DMA_enableTrigger(DMA_CH1_BASE);
    DMA_disableOverrunInterrupt(DMA_CH1_BASE);
    DMA_setInterruptMode(DMA_CH1_BASE, DMA_INT_AT_END);
    DMA_enableInterrupt(DMA_CH1_BASE);

    //
    // DMA channel 2 set up for ADCC
    //
    DMA_configAddresses(DMA_CH2_BASE, (uint16_t *)&myADC1DataBuffer,
                        (uint16_t *)ADCCRESULT_BASE);

    //
    // Perform enough 16-word bursts to fill the results buffer. Data will be
    // transferred 32 bits at a time hence the address steps below.
    //
    DMA_configBurst(DMA_CH2_BASE, 16, 2, 2);
    DMA_configTransfer(DMA_CH2_BASE, (RESULTS_BUFFER_SIZE >> 4), -14, 2);
    DMA_configMode(DMA_CH2_BASE, DMA_TRIGGER_ADCA2,
                   (DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_DISABLE |
                    DMA_CFG_SIZE_32BIT));

    DMA_enableTrigger(DMA_CH2_BASE);
    DMA_disableOverrunInterrupt(DMA_CH2_BASE);
    DMA_setInterruptMode(DMA_CH2_BASE, DMA_INT_AT_END);
    DMA_enableInterrupt(DMA_CH2_BASE);
}

//
// End of file
//
