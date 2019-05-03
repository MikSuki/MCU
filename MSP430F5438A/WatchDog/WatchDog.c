#include "msp430x54x.h"

int main(void){
    WDTCTL = WDT_ARST_1000; // 1000 ms ­«±Ò¤@¦¸
    // LED
    P5DIR |= BIT4 + BIT5;
    P5OUT ^= BIT4 + BIT5;
    // Button
    P2REN |= BIT1;
    P2OUT |= BIT1;

    while(1){
        if(!(P2IN & BIT1))
            WDTCTL = WDT_ARST_1000;
    }

    return 0;
}
