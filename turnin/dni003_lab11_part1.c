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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char x = 0;
unsigned char temp = 0x00;

enum keypad_States {keypad};
int keyPadSMTick(int state) {
	x = GetKeyPadKey();

	switch(state){
		case keypad:
			state = keypad;

	switch(x){
		case '\0' : 
			temp = 0x1F; break;
		case '1': 
			temp = 0x01; break;
		case '2': 
			temp = 0x02; break;
		case '3': 
			temp = 0x03; break;
		case '4': 
			temp = 0x04; break;
		case '5': 
			temp = 0x05; break;
		case '6': 
			temp = 0x06; break;
		case '7': 
			temp = 0x07; break;
		case '8': 
			temp = 0x08; break;
		case '9': 
			temp = 0x09; break;
		case 'A': 
			temp = 0x0A; break;
		case 'B': 
			temp = 0x0B; break;
		case 'C': 
			temp = 0x0C; break;
		case 'D': 
			temp = 0x0D; break;
		case '*': 
			temp = 0x0E; break;
		case '0': 
			temp = 0x00; break;
		case '#': 
			temp = 0x0F; break;

		default:
			temp = 0x1B; break;
	}

	break;

		default:
			state = keypad;
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
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    //the keypad
    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &keypadSMTick;

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
