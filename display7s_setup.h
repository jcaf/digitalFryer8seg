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

	/*
    //TRANSISTORES PARA EL BARRIDO/MULTIPLEXING
    #define PORTWxDISPLAY7S_Q0 		PORTD
    #define PORTRxDISPLAY7S_Q0 		PIND
    #define CONFIGIOxDISPLAY7S_Q0 	DDRD
    #define PINxDISPLAY7S_Q0 		5

	#define PORTWxDISPLAY7S_Q1 		PORTD
    #define PORTRxDISPLAY7S_Q1 		PIND
    #define CONFIGIOxDISPLAY7S_Q1 	DDRD
    #define PINxDISPLAY7S_Q1		2

	#define PORTWxDISPLAY7S_Q2 		PORTD
    #define PORTRxDISPLAY7S_Q2 		PIND
    #define CONFIGIOxDISPLAY7S_Q2 	DDRD
    #define PINxDISPLAY7S_Q2		3

	#define PORTWxDISPLAY7S_Q3 		PORTD
    #define PORTRxDISPLAY7S_Q3 		PIND
    #define CONFIGIOxDISPLAY7S_Q3 	DDRD
    #define PINxDISPLAY7S_Q3		4

	#define PORTWxDISPLAY7S_Q4 		PORTA
    #define PORTRxDISPLAY7S_Q4 		PINA
    #define CONFIGIOxDISPLAY7S_Q4 	DDRA
    #define PINxDISPLAY7S_Q4		7

	#define PORTWxDISPLAY7S_Q5 		PORTA
    #define PORTRxDISPLAY7S_Q5 		PINA
    #define CONFIGIOxDISPLAY7S_Q5 	DDRA
    #define PINxDISPLAY7S_Q5		6

	#define PORTWxDISPLAY7S_Q6 		PORTA
    #define PORTRxDISPLAY7S_Q6 		PINA
    #define CONFIGIOxDISPLAY7S_Q6 	DDRA
    #define PINxDISPLAY7S_Q6		5

	#define PORTWxDISPLAY7S_Q7 		PORTA
    #define PORTRxDISPLAY7S_Q7 		PINA
    #define CONFIGIOxDISPLAY7S_Q7 	DDRA
    #define PINxDISPLAY7S_Q7		4
	*/

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

///////////////////////////////////////////////////
// DISPLAY NORMAL POSITION
// Define position hardware pin
#define D7S_A 6
#define D7S_B 7
#define D7S_C 1
#define D7S_D 3
#define D7S_E 0
#define D7S_F 5
#define D7S_G 4
#define D7S_DP 2

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

///////////////////////////////////////////////////
// DISPLAY NORMAL POSITION
// Define position hardware pin

#define DISP7Sinvfix(data) (((data & 0x10)>>2) | ((data & 0x04)<<2) | (data & 0xEB))

#define D7S_Ginv D7S_DP
#define D7S_DPinv D7S_G
////0x3F,//    0b0011 1111,//0
//#define D7S_DATA_0inv ((0<<D7S_DPinv)|(0<<D7S_Ginv)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
////0x06,//    0b0000 0110,//1
//#define D7S_DATA_1inv ((0<<D7S_DPinv)|(0<<D7S_Ginv)|(0<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(0<<D7S_A))
////0x5B,//    0b0101 1011,//2
//#define D7S_DATA_2inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
////0x4F,//    0b0100 1111,//3
//#define D7S_DATA_3inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(0<<D7S_F)|(0<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
////0x66,//    0b0110 0110,//4
//#define D7S_DATA_4inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(1<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(0<<D7S_A))
////0x6D,//    0b0110 1101,//5
//#define D7S_DATA_5inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(1<<D7S_F)|(0<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(1<<D7S_A))
////0x7C,//    0b0111 1100,//6
//#define D7S_DATA_6inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
////0x07,//    0b0000 0111,//7
//#define D7S_DATA_7inv ((0<<D7S_DPinv)|(0<<D7S_Ginv)|(0<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
////0x7F,//    0b0111 1111,//8
//#define D7S_DATA_8inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))
////0x67,//    0b01100111,//9
//#define D7S_DATA_9inv ((0<<D7S_DPinv)|(1<<D7S_Ginv)|(1<<D7S_F)|(0<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(1<<D7S_B)|(1<<D7S_A))



//-----------------------------------------------------
//-----------------------------------------------------
//E
#define D7S_DATA_E ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(1<<D7S_A))
//r
#define D7S_DATA_r ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(0<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//o
#define D7S_DATA_o ((0<<D7S_DP)|(1<<D7S_G)|(0<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))

#define D7S_DATA_OFF 0
//t
#define D7S_DATA_t ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(1<<D7S_D)|(0<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))
//h
#define D7S_DATA_h ((0<<D7S_DP)|(1<<D7S_G)|(1<<D7S_F)|(1<<D7S_E)|(0<<D7S_D)|(1<<D7S_C)|(0<<D7S_B)|(0<<D7S_A))


//-----------------------------------------------------
//-----------------------------------------------------
void disp7s_init(void);

#endif /* DISPLAY7S_SETUP_H_ */
