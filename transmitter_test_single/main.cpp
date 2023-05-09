#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"
#include "uart.h"




int main(void) {
	uart_init(9600);
	
	while (1) {
		uart_println("amogus"); //uart test
		_delay_ms(1000);
	}
	
	return 0;
}
