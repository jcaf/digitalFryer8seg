/*
 * display7s_setup.h
 *
 *  Created on: Dec 30, 2022
 *      Author: jcaf
 */

#ifndef DISPLAY7S_SETUP_H_
#define DISPLAY7S_SETUP_H_

	#define DISP7S_TOTAL_NUMMAX 8

	//DISLAYS DE CATODO COMUN - Todo el puerto de 8bits
    #define PORTWxDISPLAY7S_DATA 	PORTC
    #define PORTRxDISPLAY7S_DATA 	PINC
    #define CONFIGIOxDISPLAY7S_DATA 	DDRC

	///////////////////////////////////////////////////
	//Define position of hardware pin into the Data Port
	#define D7S_A 6
	#define D7S_B 7
	#define D7S_C 1
	#define D7S_D 3
	#define D7S_E 0
	#define D7S_F 5
	#define D7S_G 4
	#define D7S_DP 2

	//Fix to inverted display
	#define DISP7Sinvfix(data) (((data & 0x10)>>2) | ((data & 0x04)<<2) | (data & 0xEB))
	#define D7S_Ginv D7S_DP
	#define D7S_DPinv D7S_G


	//TRANSISTORES PARA EL BARRIDO/MULTIPLEXING
    #define PORTWxDISPLAY7S_Q7 		PORTD
    #define PORTRxDISPLAY7S_Q7 		PIND
    #define CONFIGIOxDISPLAY7S_Q7 	DDRD
    #define PINxDISPLAY7S_Q7 		5

	#define PORTWxDISPLAY7S_Q6 		PORTD
    #define PORTRxDISPLAY7S_Q6 		PIND
    #define CONFIGIOxDISPLAY7S_Q6 	DDRD
    #define PINxDISPLAY7S_Q6		2

	#define PORTWxDISPLAY7S_Q5 		PORTD
    #define PORTRxDISPLAY7S_Q5 		PIND
    #define CONFIGIOxDISPLAY7S_Q5 	DDRD
    #define PINxDISPLAY7S_Q5		3

	#define PORTWxDISPLAY7S_Q4 		PORTD
    #define PORTRxDISPLAY7S_Q4 		PIND
    #define CONFIGIOxDISPLAY7S_Q4 	DDRD
    #define PINxDISPLAY7S_Q4		4

	#define PORTWxDISPLAY7S_Q3 		PORTA
    #define PORTRxDISPLAY7S_Q3 		PINA
    #define CONFIGIOxDISPLAY7S_Q3 	DDRA
    #define PINxDISPLAY7S_Q3		7

	#define PORTWxDISPLAY7S_Q2 		PORTA
    #define PORTRxDISPLAY7S_Q2 		PINA
    #define CONFIGIOxDISPLAY7S_Q2 	DDRA
    #define PINxDISPLAY7S_Q2		6

	#define PORTWxDISPLAY7S_Q1 		PORTA
    #define PORTRxDISPLAY7S_Q1 		PINA
    #define CONFIGIOxDISPLAY7S_Q1 	DDRA
    #define PINxDISPLAY7S_Q1		5

	#define PORTWxDISPLAY7S_Q0 		PORTA
    #define PORTRxDISPLAY7S_Q0 		PINA
    #define CONFIGIOxDISPLAY7S_Q0 	DDRA
    #define PINxDISPLAY7S_Q0		4

	void disp7s_init(void);
	extern unsigned char disp7s_data_array[DISP7S_TOTAL_NUMMAX];

#endif /* DISPLAY7S_SETUP_H_ */
