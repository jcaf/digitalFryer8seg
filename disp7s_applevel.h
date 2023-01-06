/*
 * disp7s_applevel.h
 *
 *  Created on: Jan 3, 2023
 *      Author: jcaf
 */

#ifndef DISP7S_APPLEVEL_H_
#define DISP7S_APPLEVEL_H_

#include "display7s_setup.h"


/* Numerical 0 to 9 */
//0x3F,//    0b0011 1111,//0
#define D7S_DATA_0 ((0<<D7S_DP)|(0<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
//0x06,//    0b0000 0110,//1
#define D7S_DATA_1 ((0<<D7S_DP)|(0<<D7S_G)|(0<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(0<<D7S_A))
//0x5B,//    0b0101 1011,//2
#define D7S_DATA_2 ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
//0x4F,//    0b0100 1111,//3
#define D7S_DATA_3 ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(0<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
//0x66,//    0b0110 0110,//4
#define D7S_DATA_4 ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(0<<D7S_A))
//0x6D,//    0b0110 1101,//5
#define D7S_DATA_5 ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(0<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(1<<D7S_A))
//0x7C,//    0b0111 1100,//6
#define D7S_DATA_6 ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//0x07,//    0b0000 0111,//7
#define D7S_DATA_7 ((0<<D7S_DP)|(0<<D7S_G)|(0<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
//0x7F,//    0b0111 1111,//8
#define D7S_DATA_8 ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
//0x67,//    0b01100111,//9
#define D7S_DATA_9 ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))

//////////////////////////////////////////////////////
// Display Off (cathode commun)
#define D7S_DATA_BLANK 0

//////////////////////////////////////////////////////
//E
#define D7S_DATA_E ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(1<<D7S_A))
//r
#define D7S_DATA_r ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(0<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//o
#define D7S_DATA_o ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//u
#define D7S_DATA_u ((0<<D7S_DP)|(0<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))

//c
#define D7S_DATA_c ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))


//n
#define D7S_DATA_n ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))

//t
#define D7S_DATA_t ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//h
#define D7S_DATA_h ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//////////////////////////////////////////////////////

//signo de grado
#define D7S_DATA_grado ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(0<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))

//-----------------------------------------------------
//-----------------------------------------------------

void disp7s_datarr_ErrorTh(void);
void disp7s_on2DecPoint_basket0(void);
void disp7s_on2DecPoint_basket1(void);
void disp7s_off2DecPoint_basket0(void);
void disp7s_off2DecPoint_basket1(void);

#endif /* DISP7S_APPLEVEL_H_ */
