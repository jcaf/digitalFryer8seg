/*
 * main.h
 *
 *  Created on: Dec 30, 2022
 *      Author: jcaf
 */

#ifndef MAIN_H_
#define MAIN_H_

struct _main_flag
{
    unsigned systick :1;
    unsigned sw1_toggle:1;
    unsigned __a:6;

};
extern struct _main_flag main_flag;



#define SYSTICK 1E-3	//1ms
//#define SYSTICK_MS 1	//1ms


#endif /* MAIN_H_ */
