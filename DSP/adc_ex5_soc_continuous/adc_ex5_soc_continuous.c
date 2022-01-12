#include "driverlib.h"
#include "device.h"
#include "board.h"

#define RESULTS_BUFFER_SIZE     256 //buffer for storing conversion results
                                //(size must be multiple of 16)

uint16_t adcAResults[RESULTS_BUFFER_SIZE];
uint16_t resultsIndex;

void main(void)
{

    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // 
    // Board Initialization
    // - Configure the ADC and power it up
    // - Setup the ADC for continuous conversions on channel 0
    // 
    Board_init();

    //
    // Initialize results buffer
    //
    for(resultsIndex = 0; resultsIndex < RESULTS_BUFFER_SIZE; resultsIndex++)
    {
        adcAResults[resultsIndex] = 0;
    }
    resultsIndex = 0;

    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Take conversions indefinitely in loop
    //
    do
    {
        //
        // Enable ADC interrupts
        //
        ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER1);
        ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER2);
        ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER3);
        ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER4);

        //
        // Clear all interrupts flags(INT1-4)
        //
        HWREGH(myADC0_BASE + ADC_O_INTFLGCLR) = 0x000F;

        //
        // Initialize results index
        //
        resultsIndex = 0;

        //
        // Software force start SOC0 to SOC7
        //
        HWREGH(myADC0_BASE + ADC_O_SOCFRC1) = 0x00FF;

        //
        // Keep taking samples until the results buffer is full
        //
        while(resultsIndex < RESULTS_BUFFER_SIZE)
        {
            //
            // Wait for first set of 8 conversions to complete
            //
            while(false == ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER3));

            //
            // Clear the interrupt flag
            //
            ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER3);

            //
            // Save results for first 8 conversions
            //
            // Note that during this time, the second 8 conversions have
            // already been triggered by EOC6->ADCIN1 and will be actively
            // converting while first 8 results are being saved
            //
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER0);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER1);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER2);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER3);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER4);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER5);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER6);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER7);

            //
            // Wait for the second set of 8 conversions to complete
            //
            while(false == ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER4));

            //
            // Clear the interrupt flag
            //
            ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER4);

            //
            // Save results for second 8 conversions
            //
            // Note that during this time, the first 8 conversions have
            // already been triggered by EOC14->ADCIN2 and will be actively
            // converting while second 8 results are being saved
            //
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER8);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER9);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER10);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER11);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER12);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER13);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER14);
            adcAResults[resultsIndex++] = ADC_readResult(ADCARESULT_BASE,
                                                         ADC_SOC_NUMBER15);
        }

        //
        // Disable all ADCINT flags to stop sampling
        //
        ADC_disableInterrupt(myADC0_BASE, ADC_INT_NUMBER1);
        ADC_disableInterrupt(myADC0_BASE, ADC_INT_NUMBER2);
        ADC_disableInterrupt(myADC0_BASE, ADC_INT_NUMBER3);
        ADC_disableInterrupt(myADC0_BASE, ADC_INT_NUMBER4);

        //
        // At this point, adcAResults[] contains a sequence of conversions
        // from the selected channel
        //

        //
        // Software breakpoint, hit run again to get updated conversions
        //
        asm("   ESTOP0");
    }
    while(1); // Loop forever
}

//
// End of file
//
