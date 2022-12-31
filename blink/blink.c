/*
 * blink.c
 *
 *  Created on: Mar 5, 2021
 *      Author: jcaf
 */
#include "blink.h"

struct _blink *blink;

void blink_set(struct _blink *b)
{
	blink = b;//set address
}

void blink_reset(int8_t BLINK_TOGGLE_STATE)
{
	blink->timerBlink = 0x00;
	blink->bf.update = 1;//BLINK_UPDATE;
	//blink->bf.toggle = !BLINK_TOGGLE_BLANK;//mejor q sea 0
	blink->bf.toggle = BLINK_TOGGLE_STATE;
}

void blink_timing(void)
{
	if (++blink->timerBlink >= blink->timerBlink_K)
	{
		blink->timerBlink = 0x00;
		//
		blink->bf.toggle = !blink->bf.toggle;
		blink->bf.update = 1;//BLINK_UPDATE;
	}
}
