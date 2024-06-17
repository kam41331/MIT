#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"
#include "adc_helper.h"
#include "daughterboard.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    init_milis();
    init_uart1();

    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL14, DISABLE);
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL15, DISABLE);
    // nastavíme clock pro ADC2a (16MHZ / 4  = 4MHz)
    ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
    // volíme zarovnání výsledku -- typicky do prava
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    // nastavíme multiplexer na **některý** kanál
    ADC2_Select_Channel(ADC2_CHANNEL_14);
    // rozběhnemen ADC
    ADC2_Cmd(ENABLE);
    // počkáme až se rozběhne ADC  (~7us)
    ADC2_Startup_Wait();

}


int main(void)
{
    uint32_t time = 0;
    uint16_t vref, vtemp, temp;

    init();

    while (1) {
        if (milis() - time > 1111 ) {
            time = milis();
            
            vref = ((long)ADC_get(CHANNEL_VREF) * 5000 + 512) / 1023;
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            vtemp = ((uint32_t)ADC_get(CHANNEL_VTEMP) * 5000L + 512) /1023;
            vtemp = vtemp * 2495L / vref;
            temp = (100L*vtemp - 40000L + 195/2) / 195 ;
            printf("%u mV, %u mV %u,%u ˚C\n", vref, vtemp, temp/10, temp%10);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
