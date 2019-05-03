#include <msp430.h>

/*
 * main.c
 */
#define Do  1000000 / 523
#define Re  1000000 / 587
#define Mi  1000000 / 659
#define Fa  1000000 / 698
#define So  1000000 / 784
#define La  1000000 / 880
#define Si  1000000 / 900
int i = 0;

struct sheet {
    float m;
    float p;
};


struct sheet s[] = {
                    {Do, 1}, {Re, 1}, {Mi, 1}, {Do, 1}, {Do, 0.5}, {Do, 1}, {Re, 1}, {Mi, 1}, {Do, 1}, {Do, 0.5},
                    {Mi, 1}, {Fa, 1}, {So, 1}, {So, 0.5}, {Mi, 1}, {Fa, 1}, {So, 1},{So, 0.5},
                    {So, 0.5}, {La, 0.5}, {So, 0.5}, {La, 0.5}, {Mi, 1}, {Do, 1},
                    {So, 0.5}, {La, 0.5}, {So, 0.5}, {La, 0.5}, {Mi, 1}, {Do, 1},
                    {Do, 1}, {So, 1}, {Do, 1}, {Do, 0.5}, {Do, 1}, {So, 1}, {Do, 1}, {Do, 0.5}
                   };


int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P5DIR |= BIT4 + BIT5;
    P8DIR |= 0x60;
    P8SEL |= 0x60;
    // Timer0
    TA0CCTL0 = CCIE;
    TA0CCR0 = 1000000 / 8 / 2;
    TA0CTL = TASSEL_2 + ID_3 + MC_1;
    // Timer1
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1;
    _BIS_SR(LPM3_bits + GIE);

    return 0;
}
// Timer0 interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    if(++i > 37) i = 0;
    //TA0CCR0 = s[i].p;
    TA0CCR0 = 1000000 / 8 / 2 * s[i].p;
    TA1CCR0 = s[i].m;
    TA1CCR1 = TA1CCR0 / 2;
    P5OUT ^= BIT4 + BIT5;
}
/*// Timer1 interrupt
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_B(void) {
    P5OUT ^= BIT5;
}*/
