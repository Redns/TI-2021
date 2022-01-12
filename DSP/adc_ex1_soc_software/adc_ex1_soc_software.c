#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t data[1000];
uint16_t count = 0;
uint16_t myADC0Result0;
uint16_t myADC0Result1;
uint16_t myADC1Result0;
uint16_t myADC1Result1;

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Set up ADCs, initializing the SOCs to be triggered by software
    //
    Board_init();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop indefinitely
    //
    while(1)
    {
        //
        // Convert, wait for completion, and store results
        //
        ADC_forceMultipleSOC(myADC0_BASE, (ADC_FORCE_SOC0 | ADC_FORCE_SOC1));

        //
        // Wait for ADCA to complete, then acknowledge flag
        //
        while(ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1) == false)
        {
        }
        ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);

        ADC_forceMultipleSOC(myADC1_BASE, (ADC_FORCE_SOC0 | ADC_FORCE_SOC1));
        //
        // Wait for ADCC to complete, then acknowledge flag
        //
        while(ADC_getInterruptStatus(myADC1_BASE, ADC_INT_NUMBER1) == false)
        {
        }
        ADC_clearInterruptStatus(myADC1_BASE, ADC_INT_NUMBER1);

        //
        // Store results
        //
        myADC0Result0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
        myADC0Result1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
        myADC1Result0 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);
        myADC1Result1 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER1);

        //
        // Software breakpoint. At this point, conversion results are stored in
        // myADC0Result0, myADC0Result1, myADC1Result0, and myADC1Result1.
        //
        // Hit run again to get updated conversions.
        //
        ESTOP0;
    }
}
