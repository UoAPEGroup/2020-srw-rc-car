/*
 * uart.h
 *
 * Created: 19/01/2021 12:27:59 pm
 *  Author: Earlene
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_init(uint16_t baudRate);
void uart_transmit(uint8_t data);
void send_data(char data[]);

#endif /* UART_H_ */