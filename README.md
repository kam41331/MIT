#include "main.h"
#include "delay.h"
#include "milis.h"
#include <stdio.h>
#include <stm8s.h>

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz
    GPIO_Init(GPIOB, GPIO_PIN_4,
              GPIO_MODE_IN_FL_NO_IT); // pir senzor -vstup bez interupt
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW); // bzucak push pull
}

int main(void) {

    init();

    while (1) {

        if (GPIO_ReadInputPin(GPIOB, GPIO_PIN_4) == SET) {

            GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
            delay_ms(1000);

        } else {
            GPIO_WriteLow(GPIOB, GPIO_PIN_5);
        }
    }
}
