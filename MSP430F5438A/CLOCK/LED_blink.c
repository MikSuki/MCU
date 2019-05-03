#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
	unsigned int i = 0;
	WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
	P5DIR |= BIT4 + BIT5;                            // P5.4 P5.5 output
	//P5OUT |= BIT4;
	//P5OUT &= ~BIT5;

	UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_4;                     // Select DCO range 16MHz operation
	UCSCTL2 = FLLD_1 + 49;                   // Set DCO Multiplier for 8MHz
											  // (N + 1) * FLLRef = Fdco
											  // (249 + 1) * 32768 = 8MHz
											  // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop


	// 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLKcycles for DCO to settle
	//__delay_cycles(250000);

	do{
		 UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);

		 SFRIFG1 &= ~OFIFG;
	}while(SFRIFG1 & OFIFG);


	while (1) {
		P5OUT ^= (BIT4 + BIT5);          // Toggle P5.4 P5.5
		//for (i = 0; i < 60000; i++);
		__delay_cycles(8000000);
	}

	return 0;
}