/*	Author: jpica003
 *  Partner(s) Name: Jonathan Picazo
 *	Lab Section: Summer 2019
 *	Assignment: Final Project
 *	Exercise Description: Pong!
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "game.h"
int main(void) {
	unsigned char pd;
	// Outputs
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	//Inputs
	DDRD = 0x00; PORTD = 0xFF; 
	while (1) {
     	// PORTC = 0x01; // to test out if lights work
     	// PORTC |= 0x04;
		pd = ~PIND;
		startGame(pd);
    }
}