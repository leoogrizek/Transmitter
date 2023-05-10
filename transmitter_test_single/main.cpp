#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"
#include "uart.h"
#include "spi.h"

uint8_t value = 0;


int main(void) {
	uart_init(9600);
	spi_init();
	
	uint8_t address[5]={0x01, 0x02, 0x03, 0x04, 0x05};


	nrf24_set_rx_tx_address(address, 0, 5);
	nrf24_write_register(RX_ADDR_P0, 0x01);
	
	uint8_t value=42;
	while (1) {
		spi_transfer(W_TX_PAYLOAD);
		spi_transfer(value);
		PORTB |= (1 << CE);
		_delay_us(15);
		PORTB &= ~(1 << CE);
		_delay_ms(1000);
	}
	
	return 0;
}
