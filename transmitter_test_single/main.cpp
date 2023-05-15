#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"
#include "uart.h"
#include "spi.h"
#include <stdio.h>

uint8_t value = 0;


int main(void) {
	uart_init(9600);
	spi_init();
	
	uint8_t TxAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
	uint8_t TxData[32];
	sprintf((char*)TxData, "antena dela jebem ji mater");
	
	uart_print_binary(nrf24_read_register(CONFIG));
	
	nrf24_init();
	nrf24_set_tx_mode(TxAddress, 10);
	

	uart_println("Beginning ... "); 


	nrf24_send_cmd(FLUSH_TX);
	_delay_ms(10);
	uint8_t fifostatus = nrf24_read_register(FIFO_STATUS);
	uart_println("fifo");
	uart_print_binary(fifostatus);
	uart_println("--------------------------");
	uart_newline();
	
	while (1) {
		if(nrf24_transmit(TxData, 32)==1) {
			uart_println("transmitted");
		}
		
		uart_println("config");
		uart_print_binary(nrf24_read_register(CONFIG));
		uart_newline();
		uart_println("--------------------------");
		_delay_ms(5000);
	}
	
	return 0;
}
