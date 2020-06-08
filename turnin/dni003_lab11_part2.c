/*	Author: dni003
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "keypad.h"
#include "bit.h"
#include "scheduler.h"
#include "timer.h"
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char msg[] = "   CS120B is Legend ... wait for it DARY!   "
unsigned char counter = 1;

enum message {message};
int messageSMTick(int state){
	switch(state) {
		case message:
			state = message;
			for(int i = 1;i <= 16; i++){
				LCD_Cursor(i);
				LCD_WriteData(msg[counter + i - 2]);
				if(counter + i + 1 == 44){
					i = 1;
				}
			}
			break;

		default:
			state = message;
			break;
	}

	switch(state){
		case message:
			i++;
			break;

	}
	return state;
}

enum display_States {display};
int displaySMTick(int state){
	unsigned char output;

	switch(state){
		case display:
			state = display;
			break;

		default:
			state = display;
			break;
	}

	switch(state){
		case display:
			output = temp;
			break;

	}
	PORTB = output;
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    LCD_init();

    //the message
    task1.state = start;
    task1.period = 200;
    task1.elapsedTime = task1.period;
    task1.TickFct = &messageSMTick;

    //the display
    task2.state = start;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &displaySMTick;

    unsigned short 1;
    unsigned long GCD = task[0]->period;
    for(i = 1; i < numTasks; i++){
    	GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();
    /* Insert your solution below */
    while (1) {
	for(i = 0; i < numTasks; i++){
		if(tasks[i]->elpasedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->);
			tasks[i]->elapsedTime = 0;
		}
		task[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
