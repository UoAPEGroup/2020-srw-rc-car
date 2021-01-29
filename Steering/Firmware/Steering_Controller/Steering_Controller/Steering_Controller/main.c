/*
 * Steering_Controller.c
 *
 * Created: 11/01/2021 10:04:20 am
 * Author : npat692
 */ 

#include "global.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"
#include "control.h"
#include "led.h"
#include "instructions_macros.h"
#include "steering.h"
#include "pi_controller.h"

ISR(USART0_RX_vect){
	input_data = UDR0;
	tx_debug(input_data);
	read_data();
}

int main(void)
{
	pin_init();
	led_init();
    uart_init();
	sei();
	system_state = ACTIVE_STATE;
    while (1) {
		/*while (system_state == STATIONARY);
		if(system_state == ACTIVE_STATE){
			turn_start();
		}*/
		led_toggle();
	}
}

