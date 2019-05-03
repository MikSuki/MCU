#include <msp430.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char buffer[1];
int i, test, rxflag, j;
unsigned char rxbuff[32];
unsigned char *rxptr;
int rxcount;
char message[16];

typedef struct dic {
	char eng[10];
	char chi[10];
	struct dic *next;
} Dic;

Dic *head;
void sendMessage(char data[]) {
	i = 0;
	while (data[i] != '\0') {
		while (!(UCA1IFG & UCTXIFG))
			;
		UCA1TXBUF = data[i];
		i++;
	}
	wrap();
}

void newDic(Dic *cur, char eng[], char chi[]) {
	strcpy(cur->eng, eng);
	strcpy(cur->chi, chi);
	cur->next = 0;
}

void initDic() {
	char eng[5][10] = { "apple", "banana", "car", "dog", "ezreal" };
	char chi[5][10] = { "蘋果", "香蕉", "車", "狗", "伊澤" };
	int i = 0;

	head = malloc(sizeof(Dic));
	newDic(head, eng[0], chi[0]);

	for (i = 1; i < 5; ++i)
		addDic(eng[i], chi[i]);

}
void strip_char(char *str, char strip) {
	char *p, *q;
	for (q = p = str; *p; p++)
		if (*p != strip)
			*q++ = *p;
	*q = '\0';
}

void addDic(char eng[], char chi[]) {
	Dic *curr = head;

	while (curr->next) {
		if (strcmp(chi, curr->chi) == 0 || strcmp(eng, curr->eng) == 0) {
			sendMessage("已經有了");
			return;
		}
		curr = curr->next;
	}

	curr->next = malloc(sizeof(Dic));
	newDic(curr->next, eng, chi);
	sendMessage("加入");
}
void addNewDic(char str[]) {
	int i = 0, j = 0, t = 2;
	char eng[32], chi[32];

	strip_char(rxbuff, '+');

	while (i < (rxcount - 1)) {
		if (str[i] == ' ') {
			--t;
			j = 0;
		} else {
			if (t == 2)
				eng[j++] = str[i];
			else
				chi[j++] = str[i];
		}
		++i;
	}

	if (t != 1) {
		sendMessage("新增錯誤");
		return;
	}

	addDic(eng, chi);
}

void delDic(char str[]) {
	Dic *curr = head, *last;
	strip_char(str, '-');
	while (curr) {
		if (strcmp(str, curr->chi) == 0 || strcmp(str, curr->eng) == 0) {
			if (curr != head) {
				free(last->next);
				last->next = curr->next;
			} else {
				head = head->next;
			}
			sendMessage("刪除");
			return;
		}

		last = curr;
		curr = curr->next;
	}
	// if not found
	sendMessage("查無此字");
}

void searchDic(char str[]) {
	Dic *curr = head;
	while (curr) {
		if (strcmp(str, curr->chi) == 0) {
			sendMessage(curr->eng);
			return;
		} else if (strcmp(str, curr->eng) == 0) {
			sendMessage(curr->chi);
			return;
		}
		curr = curr->next;
	}
	// if not found
	sendMessage("查無此字");
}

void showDic() {
	Dic *curr = head;
	sendMessage("-------------");
	while (curr) {
		sendMessage(curr->eng);
		//sendMessage(curr->chi);
		curr = curr->next;
	}
	sendMessage("-------------");
}

void freeChar(void) {
	while (rxcount > 0) {
		//todo :try to use pointer
		rxbuff[rxcount - 1] = '\0';
		rxcount--;
	}
}
void wrap(void) {
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = '\r';
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = '\n';
}
// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
	switch (__even_in_range(UCA1IV, 4)) {
	case 0:
		break;                             // Vector 0 - no interrupt
	case 2:                                // Vector 2 - RXIFG
		//while (!(UCA1IFG & UCTXIFG));    // USCI_A0 TX buffer ready?
		//UCA1TXBUF =UCA1RXBUF;            // TX -> RXed character

		UCA1TXBUF = UCA1RXBUF;
		if (UCA1RXBUF == '\r') {
			while (!(UCA1IFG & UCTXIFG))
				;
			UCA1TXBUF = '\n';
		}

		;
		if (UCA1RXBUF != 0x0D) {
			rxbuff[rxcount] = UCA1RXBUF;
			rxcount++;
		} else {
			rxflag = 1;
		}
		break;
	case 4:
		break;                             // Vector 4 - TXIFG
	default:
		break;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
	int b_f;
	switch (b_f = _even_in_range(P2IV, 16))
	//P2IV 420頁中斷向量表
	{
	case 2:
	case 4:
	case 6:
	case 8:
	case 10:
		showDic();
		break;
	default:
		break;
	}
}

void main(void) {

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P5SEL = BIT6 + BIT7;                        // P5.6,7 = USCI_A0 TXD/RXD
	UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA1CTL1 |= UCSSEL_1;                     // CLK = ACLK
	UCA1BR0 = 0x03;                        // 32kHz/9600=3.41 (see User's Guide)
	UCA1BR1 = 0x00;                           //
	UCA1MCTL = UCBRS_3 + UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
	UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

	UCA1CTL1 &= ~UCSWRST;                     // *Initialize USCI state machine*
	UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
	P2DIR &= ~0x1f;                           // 當輸入使用
	P2REN |= 0x1f;                            // 致能
	P2OUT |= 0x1f;                            // 設定為上拉電阻
	P2IE |= 0x1f;                             // 致能中斷
	P2IES |= 0x1f;                            // Falling-edge 與 Pullup有關

	__bis_SR_register(/*LPM3_bits + */GIE);    // Enter LPM3, interrupts enabled
	__no_operation();                        // For debugger

	initDic();
	while (1) {
		if (rxflag) {

			rxflag = 0;
			if (rxbuff[0] == '+' /*&& rxbuff[1] == 'd' && rxbuff[2] == 'd'*/) {
				addNewDic(rxbuff);
			} else if (rxbuff[0]
					== '-' /*&& rxbuff[1] == 'e' && rxbuff[2] == 'l'*/) {
				delDic(rxbuff);
			} else {
				searchDic(rxbuff);
			}

			freeChar();
		}
	}
}
