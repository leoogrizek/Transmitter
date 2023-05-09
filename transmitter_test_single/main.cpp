#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"
#include "uart.h"

uint8_t value = 0;


int main(void) {
	uart_init(9600);
	spi_init();
	
	while (1) {
		nrf24_write_register(CONFIG, 0b00001000);
		value = nrf24_read_register(CONFIG);
		uart_print_binary(value);
		uart_newline();
		_delay_ms(1000);
		

	}
	
	return 0;
}
