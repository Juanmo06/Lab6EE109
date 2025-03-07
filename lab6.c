/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 6 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"

void play_note(uint16_t);
void variable_delay_us(int16_t);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
uint16_t frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };
int16_t count = 0;		// Count to display
int main(void) {
    uint8_t new_state, old_state;
    uint8_t changed = 0;  // Flag for state change
    uint8_t a, b;

    // Initialize DDR and PORT registers and LCD
	lcd_init();
	DDRC &= ~((1 << 1) | (1 << 5)); 
	PORTC |= (1 << PC1) | (1 << PC5);
	DDRB |= (1 << PB4);

    // Write a splash screen to the LCD
	lcd_writecommand(1);
	lcd_stringout("Juan Moreno");
	lcd_moveto(1,0);

	char date[11];
	snprintf(date, 11, "%d/%d/%d", 8,3, 2006);
	lcd_stringout(date);
	_delay_ms(1000);
	lcd_writecommand(1);


        // The splash screen is Task 1 and is part of the Checkpoint.
        // Write your name on the top line, and use snprintf to write your
        // birthday on second line.  Delay for 1 sec and then clear the screen.


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    

    if (!b && !a)
	old_state = 0;
    else if (!b && a)
	old_state = 1;
    else if (b && !a)
	old_state = 2;
    else
	old_state = 3;

    new_state = old_state;
	char buf[10];
	snprintf(buf, 10, "count=%d", count);
	lcd_stringout(buf);

    while (1) {                 // Loop forever
	int change = 0;
	// Read the input bits and determine A and B.
	char x = PINC;
	char a = (x & (1 << 1))? 1 : 0;  // Extract bit 1
	char b = (x & (1 << 5))? 1 : 0;  // Extract bit 5


	
	// The following code is for Tasks 5 and later.
	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	if (old_state == 0) {
		// Handle A and B inputs for state 0
		if (a==1){new_state=1;}
	   	else if(b==1){new_state=3;}

	}
	else if (old_state == 1) {
		if (b==1){new_state=2;}
		else if(a==0){new_state=0;}
	    // Handle A and B inputs for state 1
		

	}
	else if (old_state == 2) {
		if (a==0){new_state=3;}
		else if(b==0){new_state=1;}
	    // Handle A and B inputs for state 2

	}
	else {   // old_state = 3
		if (b==0){new_state=0;}
		else if(a==1){new_state=2;}
	    // Handle A and B inputs for state 3

	}

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
		int changed;
		if (new_state>old_state){changed=1;}
		else if (new_state<old_state){changed=-1;}
	    if (old_state==3&&new_state==0){changed=1;}
		else if (old_state==0&&new_state==3){changed=-1;}
	    old_state = new_state;
	

        if (changed) { // Did state change?

			// Output count to LCD
			count+=changed;
			lcd_writecommand(1);
			snprintf(buf, 10, "count=%d", count);
			lcd_stringout(buf);
			changed = 0;        // Reset changed flag
			// Do we play a note?
		}
	    if ((count % 8) == 0) {
		// Determine which note (0-7) to play
		int index = ((abs(count) % 64) / 8);

		// Find the frequency of the note
		//frequency[index];

		// Call play_note and pass it the frequency
		play_note(frequency[index], count);
	    }
        
    }
	}
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(uint16_t freq, uint16_t cnt)
{
    uint32_t period;

    period = 1000000 / freq;    // Period of note in microseconds

    while (freq--) {
	PORTB |= (1 << PB4);    // Buzzer output high
	variable_delay_us(period / 2);  // Delay for half the period
	PORTB &= ~(1 << PB4);   // Buzzer output log
	variable_delay_us(period / 2);  // Delay for half the period
    }
	count = cnt;

}

/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}

ISR(PCINT1_vect)
{
    // In Task 7, add code to read the encoder inputs and determine the new
    // count value.

}


void timer1_init()
{
    // In Task 8, add code to inititialize TIMER1, but don't start it counting

}
ISR(TIMER1_COMPA_vect)
{
    // In Task 8, add code to change the output bit to the buzzer, and to turn
    // off the timer after enough periods of the signal have gone by.

}
