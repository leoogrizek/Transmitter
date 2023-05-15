/*
 * uart.cpp
 *
 * Created: 9. 05. 2023 18:38:14
 *  Author: Leo
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL

// Calculate the baud rate prescaler
#define BAUD_PRESCALE(baud_rate) (((F_CPU / (baud_rate * 16UL))) - 1)

void uart_init(uint32_t baud_rate) {
	// Set baud rate
	uint16_t prescaler = BAUD_PRESCALE(baud_rate);
	UBRR0H = (uint8_t)(prescaler >> 8);
	UBRR0L = (uint8_t)(prescaler);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(uint8_t data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));

	// Put data into buffer, send the data
	UDR0 = data;
}

uint8_t uart_receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));

	// Get and return received data from buffer
	return UDR0;
}

uint8_t uart_data_available(void) {
	// Check if data is available in the receive buffer
	return (UCSR0A & (1 << RXC0));
}

void uart_println(const char *str) {
	// Send each character in the string
	while (*str) {
		uart_transmit(*str++);
	}

	// Send newline and carriage return characters
	uart_transmit('\r');
	uart_transmit('\n');
}

void uart_println(uint8_t *str) {
	// Send each character in the string
	while (*str) {
		uart_transmit(*str++);
	}

	// Send newline and carriage return characters
	uart_transmit('\r');
	uart_transmit('\n');
}

void uart_print_binary(uint8_t value) {
	for (uint8_t i = 8; i > 0; i--) {
		uint8_t bit = (value >> (i - 1)) & 0x01;
		uart_transmit(bit ? '1' : '0');
	}
}

void uart_newline(void) {
	uart_transmit('\r');
	uart_transmit('\n');
}