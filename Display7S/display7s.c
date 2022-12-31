/*
 * display7s.c
 *
 *  Created on: Dec 29, 2022
 *      Author: jcaf

 q = 4 3  2 1 0
 *   [][]-[][][]

 */
#include "../system.h"
#include "../types.h"
#include "display7s.h"

static uint8_t DISP7S_TOTAL_NUMMAX;
static volatile uint8_t * PORTWxDISPLAY7S_DATA;
static volatile char * disp7s_data;
static struct _disp7s_mux * disp7s_mux;

void disp7s_setup(uint8_t disp7s_nummax, volatile uint8_t * port,  char * _disp7s_data, struct _disp7s_mux * __disp7s_mux)
{
	DISP7S_TOTAL_NUMMAX = disp7s_nummax;
	PORTWxDISPLAY7S_DATA = port;
	disp7s_data = _disp7s_data;
	disp7s_mux = __disp7s_mux;
}

inline static void disp7s_mux_Q(int8_t q)//q=transistor #
{

    /*
    //option 1

    for (i=0; i< DISP7S_TOTAL_NUMMAX; i++)
    {
        if ( (1<<i) & (1<<q) )
            disp7s_mux[i].Qonoff[1]();//on
        else
            disp7s_mux[i].Qonoff[0]();//off
    }
    */

    //option 2
	//int8_t i;
    /*
    for (i=0; i< DISP7S_TOTAL_NUMMAX; i++)
        {
                disp7s_mux[i].Qonoff[0]();//off
        }
    disp7s_mux[q].Qonoff[1]();//on
    */



    //+-off current Q before to present new digit
	int8_t pq=0;

	pq= q-1;

	if (pq<0)
	{
		pq = DISP7S_TOTAL_NUMMAX-1;
	}
	disp7s_mux[pq].Qonoff[0]();//off
	//-+
	disp7s_mux[q].Qonoff[1]();//on

}



void disp7s_job(void)
{
    static int8_t q;//q=transistor #

    //+-off current Q before to present new digit
    int8_t pq = q-1;
    if (pq<0)
    {
        pq = DISP7S_TOTAL_NUMMAX-1;
    }
    disp7s_mux[pq].Qonoff[0]();//off
    //-+

    *PORTWxDISPLAY7S_DATA = disp7s_data[q];


    disp7s_mux_Q(q);
    //
    if (++q == DISP7S_TOTAL_NUMMAX)
        {q=0x0;}
}
