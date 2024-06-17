#include "main.h"
#include "daughterboard.h"
#include "milis.h"
#include <stm8s.h>
#include <stdio.h>
#include <stdbool.h>

uint32_t time = 0;
extern char string[20];           
void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  // taktováni MCU na 16MHz

    GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);

    init_milis();

    // UART

    UART1_DeInit(); // zrušení předchozího nastavení

    UART1_Init(9600, // komunikační rychlost
        UART1_WORDLENGTH_8D, // délka slova
        UART1_STOPBITS_1, // počet stop bitů
        UART1_PARITY_NO, // nechceme paritní bit
        UART1_SYNCMODE_CLOCK_DISABLE, // chceme, aby to bylo asynchroní
        UART1_MODE_TXRX_ENABLE); // chceme, aby to byl přijímač i vysílač

    enableInterrupts(); // globálně povolí přerušovací systém
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE); // povolí přerušení od UART1_Rx

}

int putchar (int c)
{
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)  // čeká se, až bude prázdný
        ;
    UART1_SendData8(c);
    return c;

}


               //
int main (void) {



    init();
    while(1) {

        //rx_action();
        
        if (milis() - time > 1000) {
            time = milis();

            //UART1_SendData8('Q'); // jeden znak, píšeme do apostrofů, znaková konstanta

            // na funkci printf musíme nadeklarovat funkci putchar()
            printf("Hola hola\r\n"); // \r - vracení na začátek žádku \n - nový řádek
            printf("%s\n", string); // vytiskne prázdný řetězec
        }
    }
}



/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
