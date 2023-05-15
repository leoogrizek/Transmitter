/*
 * uart.h
 *
 * Created: 9. 05. 2023 18:37:58
 *  Author: Leo
 */ 


#ifndef UART_H_
#define UART_H_


// Function prototypes
void uart_init(uint32_t baud_rate);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);
uint8_t uart_data_available(void);
void uart_println(const char *str);
void uart_println(uint8_t *str);
void uart_print_binary(uint8_t value);
void uart_newline(void);


#endif /* UART_H_ */