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

const unsigned char DISP7_NUMERIC_ARR[10] =
{
	D7S_DATA_0,
	D7S_DATA_1,
	D7S_DATA_2,
	D7S_DATA_3,
	D7S_DATA_4,
	D7S_DATA_5,
	D7S_DATA_6,
	D7S_DATA_7,
	D7S_DATA_8,
	D7S_DATA_9,
};

/*
void disp7s_datarr_ErrorTh(void)
{
	disp7s_data_array[0]= D7S_DATA_E;
	disp7s_data_array[1]= D7S_DATA_r;
	disp7s_data_array[2]= DISP7Sinvfix(D7S_DATA_r);
	disp7s_data_array[3]= D7S_DATA_o;
	disp7s_data_array[4]= D7S_DATA_r;
	disp7s_data_array[5]= D7S_DATA_BLANK,
	//disp7s_data_array[6]= DISP7Sinvfix(D7S_DATA_t);
	disp7s_data_array[6]= D7S_DATA_BLANK;

	disp7s_data_array[7]= D7S_DATA_BLANK;
	//disp7s_data_array[7]= D7S_DATA_h;
}
*/
/*
void disp7s_datarr_ErrorTh(void)
{
	disp7s_data_array[0]= D7S_DATA_BLANK;
	disp7s_data_array[1]= D7S_DATA_8;
	disp7s_data_array[2]= DISP7Sinvfix(D7S_DATA_BLANK);
	disp7s_data_array[3]= D7S_DATA_8;
	disp7s_data_array[4]= D7S_DATA_BLANK;
	disp7s_data_array[5]= D7S_DATA_8,
	//disp7s_data_array[6]= DISP7Sinvfix(D7S_DATA_t);
	disp7s_data_array[6]= D7S_DATA_BLANK;

	disp7s_data_array[7]= D7S_DATA_8;
	//disp7s_data_array[7]= D7S_DATA_h;
}
*/
void disp7s_datarr_ErrorTh(void)
{
	/* PARECE QUE EL ASUNTO ESTA EN EL SEGMENTO D Y E
	 * */
	disp7s_data_array[0]= D7S_DATA_E;//mientras no toque otro segmento, est[a ok
	disp7s_data_array[1]= D7S_DATA_8;
	disp7s_data_array[2]= DISP7Sinvfix(D7S_DATA_BLANK);
	disp7s_data_array[3]= D7S_DATA_8;
	disp7s_data_array[4]= D7S_DATA_BLANK;
	disp7s_data_array[5]= D7S_DATA_8,
	//disp7s_data_array[6]= DISP7Sinvfix(D7S_DATA_t);
	disp7s_data_array[6]= D7S_DATA_BLANK;

	disp7s_data_array[7]= D7S_DATA_8;
	//disp7s_data_array[7]= D7S_DATA_h;
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

//parsear a toda el array es lo mejor
void disp7s_fix_upsidedown_display(unsigned char *data)
{
	*data = (((*data & 0x10)>>2) | ((*data & 0x04)<<2) | (*data & 0xEB));
}


//Display order
//[0][1][2][3]    [4][5][6][7]
//[2] -> upsidedown
//[6] -> upsidedown
void disp7s_fix_all_upsidedown_display(void)
{
	disp7s_fix_upsidedown_display(&disp7s_data_array[2]);
	disp7s_fix_upsidedown_display(&disp7s_data_array[6]);
}
void disp7s_blank_displays(unsigned char *data, int8_t initial_position, int8_t num_displays)
{
	int8_t idx = initial_position;
	for (int i=0; i<num_displays; i++)
	{
		data[idx++] = D7S_DATA_BLANK;
	}
}
