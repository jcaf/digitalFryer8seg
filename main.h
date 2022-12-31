/*
 * main.h
 *
 *  Created on: Dec 30, 2022
 *      Author: jcaf
 */

#ifndef MAIN_H_
#define MAIN_H_

struct _mainflag
{
		unsigned sysTickMs :1;
		unsigned __a:7;
};
extern struct _mainflag mainflag;



#define SYSTICK_MS 1	//1ms
//#define SYSTICK_MS 1	//1ms


#endif /* MAIN_H_ */
