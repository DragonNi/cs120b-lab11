/*	Author: dni003
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include "bit.h"
#include "scheduler.h"
#include "timer.h"
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tempOutput = 0x00;
unsigned char x = 0;
unsigned char outputChar;
unsigned char cursor = 0;

enum keypad_States {keypad};
int keypadSMTick(int state){
	x = GetKeypadKey();

	switch(state){
		case keypad:
			state = keypad;
	switch(x){
		case '\0': 
			tempOutput = 0x1F;
			break;
		case '1':
			tempOutput = 0x01;
			LCD_ClearScreen();
			break;
		case '2':
			tempOutput = 0x02;
			LCD_ClearScreen();
			break;
		case '3':
			tempOutput = 0x03;
			LCD_ClearScreen();
			break;
		case '4': 
			tempOutput = 0x04;
			LCD_ClearScreen();
			break;
		case '5':
			tempOutput = 0x05;
			LCD_ClearScreen();
          		break;
		case '6':
			tempOutput = 0x06;
			LCD_ClearScreen();
			break;
		case '7':
			tempOutput = 0x07;
			LCD_ClearScreen();
			break;
		case '8':
			tempOutput = 0x08;
			LCD_ClearScreen();
			break;
		case '9':
			tempOutput = 0x09;
			LCD_ClearScreen();
			break;
		case 'A':
			tempOutput = 0x11; 
			LCD_ClearScreen();	
			break;
		case 'B':
			tempOutput = 0x12;
			LCD_ClearScreen();
			break;
		case 'C':
			tempOutput = 0x13;
			LCD_ClearScreen();
			break;
		case 'D':
			tempOutput = 0x14;
			LCD_ClearScreen();
			break;
		case '*':
			tempOutput = 0x0E;
			LCD_ClearScreen();
			break;
		case '0':
			tempOutput = 0x00;
			LCD_ClearScreen();
			break;
		case '#':
			tempOutput = 0x0F;
			LCD_ClearScreen();
			break;

		default: 
			tempOutput = 0x1B;
			LCD_ClearScreen();
			break;
	}

		default:
			state = keypad;
			break;
	}
	return state;
}

enum display_States {display};
int displaySMTick(int state){

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
			if((tempOutput & 0xFF) != 0x1F)
			{
				if(cursor > 16){
					cursor = 1;
				}
				LCD_Cursor(cursor);
			if((tempOutput & 0xFF) == 0x0E){
				LCD_WriteData(tempOutput + 0x1C);
			}
			else if((tempOutput & 0xFF) == 0x0F){
				LCD_WriteData(tempOutput + 0x14);
			}
			else{
				LCD_WriteData(tempOutput + '0');
			}
			cursor++;
			}
			break;

	}
	return state;
}

int main(void) {
    DDRA = 0xFF; PORTA = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    LCD_init();
    LCD_ClearScreen();
    LCD_DisplayString(1, "Congratulations!");

    //the message
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &keypadSMTick;

    //the display
    task2.state = start;
    task2.period = 200;
    task2.elapsedTime = task2.period;
    task2.TickFct = &displaySMTick;

    unsigned short i;
    unsigned long GCD = tasks[0]->period;
    for(i = 1; i < numTasks; i++){
    	GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();
    while (1) {
	for(i = 0; i < numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
