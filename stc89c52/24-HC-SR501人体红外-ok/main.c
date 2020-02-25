#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit HC_SR501_out = P1 ^ 7;
sbit D7 = P2 ^ 6;
sbit beep = P1 ^ 5;


void delay(uint z)
{
	uint i, j;
	for (i = z; i > 0; i--)
		for (j = 110; j > 0; j--);
}

void main() {
	while (1) {
		if (HC_SR501_out == 1) {
			unsigned char m;
			for (m = 20; m > 0; m--) {
				beep = ~beep;
				D7 = ~D7;
				delay(500);
			}
		}
	}
}

