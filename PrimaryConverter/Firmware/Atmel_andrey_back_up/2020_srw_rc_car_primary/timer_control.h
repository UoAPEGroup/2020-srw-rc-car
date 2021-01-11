/*
 * timer_control.h
 *
 * Created: 9/12/2020 1:24:39 pm
 *  Author: achu072
 */ 


#ifndef TIMER_CONTROL_H_
#define TIMER_CONTROL_H_

void timer_control_init();
void timer_control_set_duty_on_user(uint8_t duty_cycle);

//TEST 
uint8_t get_duty();



#endif /* TIMER_CONTROL_H_ */