#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "rf.h"
#include "uart.h"
#include "spi.h"
#include "joystick.h"
#include <stdio.h>

uint8_t value = 0;


int main(void) {
	uart_init(9600);
	spi_init();
	ADC_init();
	
	uint8_t TxAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
	uint8_t TxData[32] = {0};
	
	char buffer[50];  // Buffer to hold the formatted string
	uart_print_binary(nrf24_read_register(CONFIG));
	
	nrf24_init();
	nrf24_set_tx_mode(TxAddress, 10);
	

	uart_println("Beginning ... "); 
	nrf24_send_cmd(FLUSH_TX);
	_delay_ms(10);

	while (1) {
		read_joystick_values(TxData);
		if(nrf24_transmit(TxData, 32)==1) {
			uart_println("transmitted");
			sprintf(buffer, "Yaw: %d, Thrust: %d, Pitch: %d, Roll: %d", TxData[0], TxData[1], TxData[2], TxData[3]);

			uart_println(buffer);
		}
		
		_delay_ms(50);
	}
	
	return 0;
}
