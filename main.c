/*
 * main.c
 *
 *  Created on: Dec 11, 2022
 *      Author: jcaf
 */

#include <avr/io.h>
#include "system.h"
#include "types.h"
#include "Display7S/display7s.h"
#include "display7s_setup.h"
#include "main.h"

volatile struct _isr_flag
{
    unsigned systick :1;
    unsigned __a :7;
} isr_flag;

struct _main_flag main_flag;

int main(void)
{
	PORTC = 0x0;

	PORTA = PORTD = 0x0;
	DDRC = DDRD = DDRA = 0xFF;

	disp7s_init();

	//With prescaler 64, gets 1 ms exact (OCR0=249)
	TCNT0 = 0x00;
	TCCR0 = (1 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00); //CTC, PRES=64
	OCR0 = CTC_SET_OCR_BYTIME(SYSTICK, 64); //TMR8-BIT @16MHz @PRES=1024-> BYTIME maximum = 16ms
	TIMSK |= (1 << OCIE0);

	sei();

	int8_t systick_counter0=0;
	while (1)
	{
		if (isr_flag.systick)
		{
			isr_flag.systick = 0;
			main_flag.systick = 1;
		}
		//----------------------------------
		if (main_flag.systick)
		{
			if (++systick_counter0 == (1E-3/SYSTICK) )
			{
				systick_counter0 = 0x00;
				disp7s_job();
			}

		}
		main_flag.systick = 0;
	}

	return 0;
}
ISR(TIMER0_COMP_vect)
{
	isr_flag.systick= 1;
}
