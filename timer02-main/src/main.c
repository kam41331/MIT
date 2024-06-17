#include "main.h"
#include "milis.h"
#include <stdbool.h>
#include <stdio.h>
#include <stm8s.h>
//#include "delay.h"
#include "daughterboard.h"
#include "uart1.h"

#define LEDR_PORT LED1_PORT
#define LEDR_PIN LED1_PIN
#define LEDG_PORT LED4_PORT
#define LEDG_PIN LED4_PIN
#define LEDB_PORT LED2_PORT
#define LEDB_PIN LED2_PIN

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    init_milis();
    init_uart1();

    GPIO_Init(LEDR_PORT, LEDR_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDG_PORT, LEDG_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDB_PORT, LEDB_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000 - 1);

    TIM2_OC1Init(                // inicializujeme kanál 1 (TM2_CH1)
        TIM2_OCMODE_PWM1,        // režim PWM1
        TIM2_OUTPUTSTATE_ENABLE, // Výstup povolen (TIMer ovládá pin)
        3000, // výchozí hodnota šířky pulzu (střídy) 1056/1600 = 66%
        TIM2_OCPOLARITY_HIGH // Polarita LOW protože LED rozsvěcím 0 (spol.
                             // anoda)
    );
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 7000,
                 TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 2000,
                 TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE); // povolí TIM2
}

int main(void) {

    uint32_t time = 0;
    uint16_t r = 1, g = 1, b = 1;
    uint8_t color = 3;
    uint16_t *colors[] = {0, &r, &g, &b};
    bool S1memory = false;
    bool S2memory = false;
    bool S3memory = false;

    init();

    while (1) {
        if (milis() - time > 10) {
            time = milis();

            // S1  přepínáná barvy
            if (!PUSH(S1) && S1memory) {
                if (++color > 3)
                    color = 1;
            }
            S1memory = PUSH(S1);
            LOW(LEDR);
            LOW(LEDG);
            LOW(LEDB);
            switch (color) {
            case 1:
                HIGH(LEDR);
                break;
            case 2:
                HIGH(LEDG);
                break;
            case 3:
                HIGH(LEDB);
                break;
            }

            // S3 zvyšuje jas
            if (!PUSH(S3) && S3memory) {
                *colors[color] += 1000;
                if (*colors[color] > 10000)
                    *colors[color] = 10001;
                printf("%d: %d %d %d\n", color, r, g, b);
            }
            S3memory = PUSH(S3);
            // S2 snižuje jas
            if (!PUSH(S2) && S2memory) {
                *colors[color] -= 1000;
                if (*colors[color] > 10001)
                    *colors[color] = 1;
                printf("%d: %d %d %d\n", color, r, g, b);
            }
            S2memory = PUSH(S2);

            TIM2_SetCompare1(b - 1);
            TIM2_SetCompare2(g - 1);
            TIM2_SetCompare3(r - 1);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
