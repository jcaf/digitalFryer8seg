/*
 * disp7s_applevel.c
 *
 *  Created on: Jan 3, 2023
 *      Author: jcaf
 */
#include "main.h"
#include "system.h"
#include "disp7s_applevel.h"
#include "display7s_setup.h"



void disp7s_datarr_ErrorTh(void)
{
	disp7s_data_array[0]= D7S_DATA_E;
	disp7s_data_array[1]= D7S_DATA_r;
	disp7s_data_array[2]= DISP7Sinvfix(D7S_DATA_r);
	disp7s_data_array[3]= D7S_DATA_o;
	disp7s_data_array[4]= D7S_DATA_r;
	disp7s_data_array[5]= D7S_DATA_BLANK,
	disp7s_data_array[6]= DISP7Sinvfix(D7S_DATA_t);
	disp7s_data_array[7]= D7S_DATA_h;
}

void disp7s_on2DecPoint_basket0(void)
{
	OR_BITWISE(disp7s_data_array[1],(1<< D7S_DPinv));
	OR_BITWISE(disp7s_data_array[2],(1<< D7S_DP));
}
void disp7s_on2DecPoint_basket1(void)
{
	OR_BITWISE(disp7s_data_array[5],(1<< D7S_DPinv));
	OR_BITWISE(disp7s_data_array[6],(1<< D7S_DP));
}

void disp7s_off2DecPoint_basket0(void)
{
	AND_BITWISE(disp7s_data_array[1],~(1<< D7S_DPinv));
	AND_BITWISE(disp7s_data_array[2],~(1<< D7S_DP));
}
void disp7s_off2DecPoint_basket1(void)
{
	AND_BITWISE(disp7s_data_array[5],~(1<< D7S_DPinv));
	AND_BITWISE(disp7s_data_array[6],~(1<< D7S_DP));
}
