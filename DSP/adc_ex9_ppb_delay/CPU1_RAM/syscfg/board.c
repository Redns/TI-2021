/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "board.h"

void Board_init()
{
	EALLOW;

	PinMux_init();
	ASYSCTL_init();
	ADC_init();
	INTERRUPT_init();

	EDIS;
}

void PinMux_init()
{

}

void ADC_init(){
	//myADC0 initialization

	// ADC Initialization: Write ADC configurations and power up the ADC
	// Configures the ADC module's offset trim
	ADC_setOffsetTrimAll(ADC_REFERENCE_INTERNAL,ADC_REFERENCE_3_3V);
	// Configures the analog-to-digital converter module prescaler.
	ADC_setPrescaler(myADC0_BASE, ADC_CLK_DIV_2_0);
	// Sets the timing of the end-of-conversion pulse
	ADC_setInterruptPulseMode(myADC0_BASE, ADC_PULSE_END_OF_CONV);
	// Powers up the analog-to-digital converter core.
	ADC_enableConverter(myADC0_BASE);
	// Delay for 1ms to allow ADC time to power up
	DEVICE_DELAY_US(5000);

	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	// Disables SOC burst mode.
	ADC_disableBurstMode(myADC0_BASE);
	// Sets the priority mode of the SOCs.
	ADC_setSOCPriority(myADC0_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	// Start of Conversion 0 Configuration
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM1_SOCA
	//	  	Channel			: ADC_CH_ADCIN0
	//	 	Sample Window	: 8 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 8U);
	ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	// Start of Conversion 1 Configuration
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM2_SOCA
	//	  	Channel			: ADC_CH_ADCIN2
	//	 	Sample Window	: 8 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM2_SOCA, ADC_CH_ADCIN2, 8U);
	ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
	// ADC Interrupt 1 Configuration
	// 		SOC/EOC number	: 0
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	ADC_setInterruptSource(myADC0_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER1);
	ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(myADC0_BASE, ADC_INT_NUMBER1);
	// ADC Interrupt 2 Configuration
	// 		SOC/EOC number	: 1
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	ADC_setInterruptSource(myADC0_BASE, ADC_INT_NUMBER2, ADC_SOC_NUMBER1);
	ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER2);
	ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER2);
	ADC_disableContinuousMode(myADC0_BASE, ADC_INT_NUMBER2);
			
	// PPB Configuration: Configure high and low limits detection for ADCPPB
	// Post Processing Block 1 Configuration
	// 		Configures a post-processing block (PPB) in the ADC.
	// 		PPB Number				: 1
	// 		SOC/EOC number			: 0
	// 		Calibration Offset		: 0
	// 		Reference Offset		: 0
	// 		Two's Complement		: Disabled
	// 		Trip High Limit			: 0
	// 		Trip Low Limit			: 0
	// 		Clear PPB Event Flags	: Disabled
	ADC_setupPPB(myADC0_BASE, ADC_PPB_NUMBER1, ADC_SOC_NUMBER0);
	ADC_disablePPBEvent(myADC0_BASE, ADC_PPB_NUMBER1, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO | ADC_EVT_ZERO));
	ADC_disablePPBEventInterrupt(myADC0_BASE, ADC_PPB_NUMBER1, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO | ADC_EVT_ZERO));
	ADC_setPPBCalibrationOffset(myADC0_BASE, ADC_PPB_NUMBER1, 0);
	ADC_setPPBReferenceOffset(myADC0_BASE, ADC_PPB_NUMBER1, 0);
	ADC_disablePPBTwosComplement(myADC0_BASE, ADC_PPB_NUMBER1);
	ADC_setPPBTripLimits(myADC0_BASE, ADC_PPB_NUMBER1, 0, 0);
	ADC_disablePPBEventCBCClear(myADC0_BASE, ADC_PPB_NUMBER1);

}
void ASYSCTL_init(){
	// asysctl initialization
	// Disables the temperature sensor output to the ADC.
	ASysCtl_disableTemperatureSensor();
	// Set the analog voltage reference selection to internal.
	ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
	// Set the internal analog voltage reference selection to 1.65V.
	ASysCtl_setAnalogReference1P65( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
}
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_myADC0_1
	Interrupt_register(INT_myADC0_1, &adcA1ISR);
	Interrupt_enable(INT_myADC0_1);
	
	// Interrupt Setings for INT_myADC0_2
	Interrupt_register(INT_myADC0_2, &adcA2ISR);
	Interrupt_enable(INT_myADC0_2);
}

