/*
 * timer1.c
 *
 * Created: 12/15/2020 1:10:37 PM
 *  Author: Kimsong Lor
 */ 

ISR(TIMER1_COMPB_vect) {
	;
}

void timer1_init() {
	TCCR1B |= (1<<WGM12); //CTC mode OCR1A as top
	TCCR1B |= (1<<CS11) | (1<<CS10); //64 prescaling
	
	TIMSK1 |= (1<<OCIE1B); //Timer/Counter1, Output Compare B Match Interrupt Enable
	
	OCR1A = 52; 
	OCR1B = 51; //0.1us trigger timing, compare match B
}