/*                MSP432E401Y
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST            PE3|<--AIN0
 *            |                  |
 *            |                  |
 *            |                  |
 *            |               PA0|<--U0RX
 *            |               PA1|-->U0TX
 * Author: Amit Ashara
*******************************************************************************/

#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pga.h"
#include "arm_math.h"
#include "uartstdio.h"
#include "Calculate.h"
#include "arm_const_structs.h"


/* Define for Samples to be captured and Sampling Frequency */
#define NUM_SAMPLES 1024


/* DMA Buffer declaration and buffer complet flag */
static uint32_t dstBufferA[NUM_SAMPLES];
static uint32_t dstBufferB[NUM_SAMPLES];
volatile bool setBufAReady = false;
volatile bool setBufBReady = false;
volatile uint32_t SAMP_FREQ = 200000;
/* Global variables and defines for FFT */
#define IFFTFLAG   0
#define BITREVERSE 1
float float_sampling_buf[NUM_SAMPLES*2]={0};
float *big_pointer_buf;
float fftfout[NUM_SAMPLES];
float fftvout[NUM_SAMPLES];
float temp_fftvout[NUM_SAMPLES*2];
float voutmax;
float fftwin[NUM_SAMPLES];
float fft_winout_sampling[NUM_SAMPLES];
uint32_t voutmax_index;
float voutmax_fre;
float fft_DC;
float fft_THD;
int button_value;
    float true_fftfout[NUM_SAMPLES/2];
    float true_fftvout[NUM_SAMPLES/2];
volatile int16_t fftOutput[NUM_SAMPLES*2];

/* Global variables for RMS and DC calculation */
volatile float32_t rmsBuff;
volatile float32_t dcBuff;
float32_t rmsCalculation;

/* The control table used by the uDMA controller.  This table must be aligned
 * to a 1024 byte boundary. */
#if defined(__ICCARM__)
#pragma data_alignment=1024
uint8_t pui8ControlTable[1024];
#elif defined(__TI_ARM__)
#pragma DATA_ALIGN(pui8ControlTable, 1024)
uint8_t pui8ControlTable[1024];
#else
uint8_t pui8ControlTable[1024] __attribute__ ((aligned(1024)));
#endif

float *find_fft_maxv(float *sampling_buf,unsigned int cir_times,unsigned int point_num)
{
    /*???????????????????????????????????????????????????????????*/
    //?????????????????

    int fft1_i1,fft1_i2;
    float temp_c_buf[sizeof(sampling_buf)*2]={0};
    float temp_in_c_buf[sizeof(sampling_buf)*2]={0};
    float temp_mag_buf[sizeof(sampling_buf)*2]={0};
    float *big_buf;
    arm_cfft_instance_f32 * S;
    switch(point_num)
    {
        case 16:
        {
            *S=arm_cfft_sR_f32_len16;
            break;
        }
        case 32:
        {
            *S=arm_cfft_sR_f32_len32;
            break;
        }
        case 64:
        {
            *S=arm_cfft_sR_f32_len64;
            break;
        }
        case 128:
        {
            *S=arm_cfft_sR_f32_len128;
            break;
        }
        case 256:
        {
            *S=arm_cfft_sR_f32_len256;
            break;
        }
        case 512:
        {
            *S=arm_cfft_sR_f32_len512;
            break;
        }
        case 1024:
        {
            *S=arm_cfft_sR_f32_len1024;
            break;
        }
        case 2048:
        {
            *S=arm_cfft_sR_f32_len2048;
            break;
        }
        case 4096:
        {
            *S=arm_cfft_sR_f32_len4096;
            break;
        }
        default:
        {
            UARTprintf("fft??????????????????????????????");
            return 0;
        }
    }

    big_buf=malloc(sizeof(sampling_buf)*cir_times);
    //????????????????????????????????
    for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
    {
        temp_c_buf[fft1_i1*2]=sampling_buf[fft1_i1];
    }

    //??????????????????
    arm_copy_f32(temp_c_buf,temp_in_c_buf,sizeof(sampling_buf)*2);

    //????????????????
    for(fft1_i2=0;fft1_i2<cir_times;fft1_i2++)
    {
        //????????????????
        for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
        {
            temp_c_buf[fft1_i1*2]=temp_in_c_buf[fft1_i1*2]*arm_cos_f32(-fft1_i1*(PI/(float)(point_num*cir_times)));
            temp_c_buf[fft1_i1*2+1]=temp_in_c_buf[fft1_i1*2]*arm_sin_f32(-fft1_i1*(PI/(float)(point_num*cir_times)));
        }

        //fft
        arm_cfft_f32(S,temp_c_buf,0,1);
        arm_cmplx_mag_f32(temp_c_buf,temp_mag_buf,point_num);
        //???????????????????????????????????????????????????
        for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
        {
            big_buf[fft1_i2+fft1_i1*cir_times]=temp_c_buf[fft1_i1];
        }
    }
    return big_buf;
}
float *my_fft_f(float *sampling_buf,unsigned int cir_times,unsigned int point_num)
{
    /*???????????????????????????????????????????????????????????*/
    //?????????????????



    int fft1_i1,fft1_i2;
    float temp_c_buf[sizeof(sampling_buf)*2]={0};
    float temp_in_c_buf[sizeof(sampling_buf)*2]={0};
    float temp_mag_buf[sizeof(sampling_buf)*2]={0};
    float *big_buf;
    arm_cfft_instance_f32 * S;
    switch(point_num)
    {
        case 16:
        {
            *S=arm_cfft_sR_f32_len16;
            break;
        }
        case 32:
        {
            *S=arm_cfft_sR_f32_len32;
            break;
        }
        case 64:
        {
            *S=arm_cfft_sR_f32_len64;
            break;
        }
        case 128:
        {
            *S=arm_cfft_sR_f32_len128;
            break;
        }
        case 256:
        {
            *S=arm_cfft_sR_f32_len256;
            break;
        }
        case 512:
        {
            *S=arm_cfft_sR_f32_len512;
            break;
        }
        case 1024:
        {
            *S=arm_cfft_sR_f32_len1024;
            break;
        }
        case 2048:
        {
            *S=arm_cfft_sR_f32_len2048;
            break;
        }
        case 4096:
        {
            *S=arm_cfft_sR_f32_len4096;
            break;
        }
        default:
        {
            UARTprintf("fft??????????????????????????????");
            return 0;
        }
    }

    big_buf=malloc(sizeof(sampling_buf)*cir_times);
    //????????????????????????????????
    for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
    {
        temp_c_buf[fft1_i1*2]=sampling_buf[fft1_i1];
    }

    //??????????????????
    arm_copy_f32(temp_c_buf,temp_in_c_buf,sizeof(sampling_buf)*2);

    //????????????????
    for(fft1_i2=0;fft1_i2<cir_times;fft1_i2++)
    {
        //????????????????
        for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
        {
            temp_c_buf[fft1_i1*2]=temp_in_c_buf[fft1_i1*2]*arm_cos_f32(-fft1_i1*(PI/(float)(point_num*cir_times)));
            temp_c_buf[fft1_i1*2+1]=temp_in_c_buf[fft1_i1*2]*arm_sin_f32(-fft1_i1*(PI/(float)(point_num*cir_times)));
        }

        //fft
        arm_cfft_f32(S,temp_c_buf,0,1);
        arm_cmplx_mag_f32(temp_c_buf,temp_mag_buf,point_num);
        //???????????????????????????????????????????????????
        for(fft1_i1=0;fft1_i1<sizeof(sampling_buf);fft1_i1++)
        {
            big_buf[fft1_i2+fft1_i1*cir_times]=temp_c_buf[fft1_i1];
        }
    }
    return big_buf;
}




void ADC0SS2_IRQHandler(void)
{
    uint32_t getIntStatus;
    uint32_t getDMAStatus;

    /* Get the interrupt status from the ADC */
    getIntStatus = MAP_ADCIntStatusEx(ADC0_BASE, true);

    /* Clear the ADC interrupt flag. */
    MAP_ADCIntClearEx(ADC0_BASE, getIntStatus);

    /* Read the primary and alternate control structures to find out which
     * of the structure has completed and generated the done interrupt. Then
     * re-initialize the appropriate structure */
    getDMAStatus = MAP_uDMAChannelModeGet(UDMA_CH16_ADC0_2 |
                                          UDMA_PRI_SELECT);

    /* Check if the primary or alternate channel has completed. On completion
     * re-initalize the channel control structure. If the Primary channel has
     * completed then set Buffer-A ready flag so that the main application
     * may perform the DSP computation. Similarly if the Alternate channel
     * has completed then set Buffer-B ready flag so that the main application
     * may perform the DSP computation. */
    if(getDMAStatus == UDMA_MODE_STOP)
    {
        MAP_uDMAChannelTransferSet(UDMA_CH16_ADC0_2 | UDMA_PRI_SELECT,
                                                   UDMA_MODE_PINGPONG,
                                                   (void *)&ADC0->SSFIFO2, (void *)&dstBufferA,
                                                   sizeof(dstBufferA)/4);
        setBufAReady = true;
        MAP_TimerDisable(TIMER0_BASE, TIMER_A);
    }

    getDMAStatus = MAP_uDMAChannelModeGet(UDMA_CH16_ADC0_2 |
                                          UDMA_ALT_SELECT);

    if(getDMAStatus == UDMA_MODE_STOP)
    {
        MAP_uDMAChannelTransferSet(UDMA_CH16_ADC0_2 | UDMA_ALT_SELECT,
                                   UDMA_MODE_PINGPONG,
                                   (void *)&ADC0->SSFIFO2, (void *)&dstBufferB,
                                   sizeof(dstBufferB)/4);
        setBufBReady = true;
    }
}

void ConfigureUART(uint32_t systemClock)
{
    /* Enable the clock to GPIO port A and UART 0 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Configure the GPIO Port A for UART 0 */
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure the UART for 115200 bps 8-N-1 format */
    UARTStdioConfig(0, 115200, systemClock);
}

int main(void)
{
  char buf[32];
    uint32_t systemClock;
    uint32_t t;

    /* Configure the system clock for 120 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                          SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL |
                                          SYSCTL_CFG_VCO_480), 120000000);

    /* Initialize serial console */
    ConfigureUART(systemClock);

    /* Display the setup on the console. */
    UARTprintf("\033[2J\033[H");
    UARTprintf("\rCMSIS DSP Demo...\n\n");
    UARTprintf("\033[2GDC Average \033[31G\n");
    UARTprintf("\033[2GRMS \033[31G\n");
    UARTprintf("\033[2GFFT Amplitude \033[31G\n");
    UARTprintf("\033[2GFFT Frequency \033[31G\n");


    /* Enable the clock to GPIO Port E and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)))
    {
    }

    /* Configure PE3 as ADC input channel */
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOJ->DIR&=~BIT0;
    GPIOJ->PUR|=BIT0;
    GPIOJ->DEN|=BIT0;
    GPIOJ->DATA|=BIT0;
    /* Enable the clock to ADC-0 and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)))
    {
    }

    /* Configure Sequencer 2 to sample the analog channel : AIN0. The end of
     * conversion and interrupt generation is set for AIN0 */
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                                 ADC_CTL_END);

    /* Enable sample sequence 2 with a timer signal trigger.  Sequencer 2
     * will do a single sample when the timer generates a trigger on timeout*/
    MAP_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_TIMER, 2);

    /* Clear the interrupt status flag before enabling. This is done to make
     * sure the interrupt flag is cleared before we sample. */
    MAP_ADCIntClearEx(ADC0_BASE, ADC_INT_DMA_SS2);
    MAP_ADCIntEnableEx(ADC0_BASE, ADC_INT_DMA_SS2);

    /* Enable the DMA request from ADC0 Sequencer 2 */
    MAP_ADCSequenceDMAEnable(ADC0_BASE, 2);

    /* Since sample sequence 2 is now configured, it must be enabled. */
    MAP_ADCSequenceEnable(ADC0_BASE, 2);

    /* Enable the Interrupt generation from the ADC-0 Sequencer */
    MAP_IntEnable(INT_ADC0SS2);

    /* Enable the DMA and Configure Channel for TIMER0A for Ping Pong mode of
     * transfer */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_UDMA)))
    {
    }

    MAP_uDMAEnable();

    /* Point at the control table to use for channel control structures. */
    MAP_uDMAControlBaseSet(pui8ControlTable);

    /* Map the ADC0 Sequencer 2 DMA channel */
    MAP_uDMAChannelAssign(UDMA_CH16_ADC0_2);

    /* Put the attributes in a known state for the uDMA ADC0 Sequencer 2
     * channel. These should already be disabled by default. */
    MAP_uDMAChannelAttributeDisable(UDMA_CH16_ADC0_2,
                                    UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                    UDMA_ATTR_HIGH_PRIORITY |
                                    UDMA_ATTR_REQMASK);

    /* Configure the control parameters for the primary control structure for
     * the ADC0 Sequencer 2 channel. The primary control structure is used for
     * copying the data from ADC0 Sequencer 2 FIFO to dstBufferA. The transfer
     * data size is 32 bits and the source address is not incremented while
     * the destination address is incremented at 32-bit boundary.
     */
    MAP_uDMAChannelControlSet(UDMA_CH16_ADC0_2 | UDMA_PRI_SELECT,
                              UDMA_SIZE_32 | UDMA_SRC_INC_NONE |
                              UDMA_DST_INC_32 | UDMA_ARB_1);

    /* Set up the transfer parameters for the ADC0 Sequencer 2 primary control
     * structure. The mode is Basic mode so it will run to completion. */
    MAP_uDMAChannelTransferSet(UDMA_CH16_ADC0_2 | UDMA_PRI_SELECT,
                               UDMA_MODE_PINGPONG,
                               (void *)&ADC0->SSFIFO2, (void *)&dstBufferA,
                               sizeof(dstBufferA)/4);

    /* Configure the control parameters for the alternate control structure for
     * the ADC0 Sequencer 2 channel. The alternate control structure is used for
     * copying the data from ADC0 Sequencer 2 FIFO to dstBufferB. The transfer
     * data size is 32 bits and the source address is not incremented while
     * the destination address is incremented at 32-bit boundary.
     */
    MAP_uDMAChannelControlSet(UDMA_CH16_ADC0_2 | UDMA_ALT_SELECT,
                              UDMA_SIZE_32 | UDMA_SRC_INC_NONE |
                              UDMA_DST_INC_32 | UDMA_ARB_1);

    /* Set up the transfer parameters for the ADC0 Sequencer 2 alternate
     * control structure. The mode is Basic mode so it will run to
     * completion */
    MAP_uDMAChannelTransferSet(UDMA_CH16_ADC0_2 | UDMA_ALT_SELECT,
                               UDMA_MODE_PINGPONG,
                               (void *)&ADC0->SSFIFO2, (void *)&dstBufferB,
                               sizeof(dstBufferB)/4);

    /* The uDMA ADC0 Sequencer 2 channel is primed to start a transfer. As
     * soon as the channel is enabled and the Timer will issue an ADC trigger,
     * the ADC will perform the conversion and send a DMA Request. The data
     * transfers will begin. */
    MAP_uDMAChannelEnable(UDMA_CH16_ADC0_2);

    /* Enable Timer-0 clock and configure the timer in periodic mode with
     * a frequency of 1 KHz. Enable the ADC trigger generation from the
     * timer-0. */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)))
    {
    }

    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, (systemClock/SAMP_FREQ));
    MAP_TimerADCEventSet(TIMER0_BASE, TIMER_ADC_TIMEOUT_A);
    MAP_TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
    MAP_TimerEnable(TIMER0_BASE, TIMER_A);
        UARTprintf("code begin\r\n");
    /* While loop to process the data */
    while(1)
    {
        /* Wait for Primary channel to complete and then clear the flag and
         * initialize the variables */

        while((!setBufAReady)||(button_value=((GPIOJ->DATA)&BIT0)));
        while(!((GPIOJ->DATA)&BIT0));
        MAP_TimerEnable(TIMER0_BASE, TIMER_A);
        setBufAReady = false;

        get_window(WINDOW_RECTANG,NUM_SAMPLES, fftwin);
        /* First convert the sampled data to floating point format as the RMS
         * and DC average is being computed */

        for(t=0;t<NUM_SAMPLES;t++)
                    {
                        fft_winout_sampling[t]=fftwin[t]*(float)dstBufferA[t];

                    }

        for(t=0;t<NUM_SAMPLES;t++)
            {
                float_sampling_buf[t*2]=3.3f*fft_winout_sampling[t]/4096.0f;
                float_sampling_buf[t*2+1]=0;
            }


            arm_cfft_f32(&arm_cfft_sR_f32_len1024,float_sampling_buf,0,1);
            arm_cmplx_mag_f32(float_sampling_buf,temp_fftvout,NUM_SAMPLES);

            fftfout[0]=0;
            fftvout[0]=temp_fftvout[0]/(float)NUM_SAMPLES;
            fft_DC=fftvout[0];

            for(t=1;t<NUM_SAMPLES;t++)
            {
                fftfout[t]=(float)t*(float)SAMP_FREQ/(float)NUM_SAMPLES;

                fftvout[t]=temp_fftvout[t]/((float)NUM_SAMPLES/2.0f);
            }
//            for(t=0;t<NUM_SAMPLES/2;t++)
//            {
//               true_fftfout[t]=fftfout[t];
//
//               true_fftvout[t]=fftvout[t];
//            }
            for(t=0;t<3;t++)
            {
                fftvout[t]=0;

            }
            arm_max_f32(fftvout,NUM_SAMPLES/2,&voutmax,&voutmax_index);
            voutmax_fre=fftfout[voutmax_index];
            if(voutmax_index*5<NUM_SAMPLES/2)
                fft_THD=100.0f*sqrt(fftvout[voutmax_index*2]*fftvout[voutmax_index*2]+fftvout[voutmax_index*3]*fftvout[voutmax_index*3]+fftvout[voutmax_index*4]*fftvout[voutmax_index*4]+fftvout[voutmax_index*5]*fftvout[voutmax_index*5])/(fftvout[voutmax_index]);
            else
                fft_THD=-1;

            UARTprintf("Afftvmax ");
                    sprintf(buf,"index =  %d -- fre = %.3f Hz",voutmax_index,voutmax_fre);
                    UARTprintf("%s",buf);
                    sprintf(buf," -- vol: %.3f V\r\n",voutmax);
                    UARTprintf("%s",buf);



        /* Wait for Alternate channel to complete and then clear the flag and
         * initialize the variables */
        //free(big_pointer_buf);
//        while(!setBufBReady);
//        setBufBReady = false;
//        dcBuff       = 0.0f;
//        rmsBuff      = 0.0f;


    }
}
