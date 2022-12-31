/*
 * main.h
 *
 *  Created on: Dec 30, 2022
 *      Author: jcaf
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include "types.h"
#include "system.h"
#include "lcdan/lcdan.h"
#include "lcdan/lcdan_aux.h"
#include "ikb/ikb.h"
#include "timing/timing.h"
#include "utils/utils.h"
#include "blink/blink.h"
#include "MAX6675/MAX6675.h"
#include "Temperature/temperature.h"
#include "SPI/SPI.h"

#define SYSTICK_MS 1	//1ms

struct _mainflag
{
		unsigned sysTickMs :1;
		unsigned __a:7;
};
extern struct _mainflag mainflag;



#define DISP_CURSOR_BASKETLEFT_START_X 0x00
#define DISP_CURSOR_BASKETRIGHT_START_X 0x0B

enum _PSMODE
{
	PSMODE_OPERATIVE,
	PSMODE_PROGRAM,
};

enum E_KBMODE
{
	KBMODE_DEFAULT = 0,
	KBMODE_EDIT_COOKCYCLE,
	KBMODE_EDIT_TSETPOINT
};
enum DISPLAY_OWNER
{
	DISPLAY_TIMING = 0,
	DISPLAY_EDITCOOKCYCLE,
};

struct _pgrmmode
{
		struct _pgrmmode_bf
		{
				unsigned unitTemperature:1;//Degrees Fahrenheit or Celsius
				unsigned __a:7;//reserved
		}bf;
		int8_t numBaskets;//Default=1;
		int8_t language;//Languages, default SPANISH

};

struct _basket
{
		struct _display_basket
		{
				struct _cursor
				{
						int8_t x;
						int8_t y;
				}cursor;

				struct _bf_display_basket
				{
						unsigned print_cookCycle :1;
						unsigned __a:7;
				}bf;

				int8_t owner;
		}display;

		struct _kb_basket
		{
				int8_t startStop;
				int8_t sleep;
				int8_t down;
				int8_t up;
				int8_t program;
		} kb;

		int8_t kbmode;

		struct _cookCycle
		{
				struct _t time;
				int8_t counterTicks;

				struct _bf_cookCycle
				{
						unsigned on :1;
						unsigned forceCheckControl :1;
						unsigned blinkDone :1;
						unsigned __a:5;
				} bf;

				struct _editcycle
				{
						int16_t timerTimeout;

						struct _bf_editcycle
						{
								unsigned blinkIsActive :1;
								unsigned __a:7;
						} bf;

				} editcycle;

		} cookCycle;

		struct _bf_basket
		{
				unsigned user_startStop:1;
				unsigned prepareReturnToKBDefault:1;
				unsigned __a:6;

		}bf;

		struct _blink blink;
};


#define BASKET_MAXSIZE 2
#define BASKET_LEFT 0
#define BASKET_RIGHT 1

struct _process
{
		int8_t sm0;
};

enum _FRYER_VIEWMODE
{
	FRYER_VIEWMODE_PREHEATING = 0,
    FRYER_VIEWMODE_COOK,
    FRYER_VIEWMODE_PROGRAM,
    FRYER_VIEWMODE_VIEWCOOKTEMP
};

struct _fryer
{
		struct _basket basket[BASKET_MAXSIZE];
		struct _fryer_bf
		{
				unsigned preheating:1;
				unsigned program_mode:1;
				unsigned operative_mode:1;
				unsigned __a:5;
		}bf;

		int8_t viewmode;

		struct _process ps_program;
		struct _process ps_operative;
		struct _process ps_viewTemp;//added 7 ab 2022
};
extern const struct _process ps_reset;
extern struct _fryer fryer;

void kbmode_default(struct _kb_basket *kb);
void kbmode_2basket_set_default(void);

struct _job
{
		int8_t sm0;
		uint16_t counter0;
		//uint16_t counter1;

		struct _job_f
		{
				unsigned enable:1;
				unsigned job:1;
				unsigned lock:1;
				unsigned recorridoEnd:1;
				unsigned __a:4;
		}f;
};
extern struct _job job_captureTemperature;
extern const struct _job job_reset;


extern struct _t EEMEM COOKTIME[BASKET_MAXSIZE];


//////////////////////////////////////////
//Tiempos de Ignicion
#define TchispaIg 500//ms Tiempo de Chispa de igninicion
#define TdelayGasBurned 0//ms Tiempo prudente que puede demorar en prenderse el gas (Si es que lo existiera, sino seria 0)
//Tiempos de deteccion de llamas

//	//Temperaturas
//	#define TmprtPrecalentamiento
//	#define TmprtCoccion
//#define TMPR_MAX 218//C i 425F -> E-5 = FRYER TOO HOT


//Constantes
#define KintentosIg 3//intentos de ignicion

//Alarmas
#define CodeAlerta0
#define CodeAlerta1


/* C = (F-32)/1.8
 * F= (C*1.8) + 32
 */


/* oficialmente PC2 es la salida del PID */
#define PORTWxSOL_GAS_QUEMADOR 		PORTB
#define PORTRxSOL_GAS_QUEMADOR 		PINB
#define CONFIGIOxSOL_GAS_QUEMADOR 	DDRB
#define PINxKB_SOL_GAS_QUEMADOR		2


#define PORTWxBUZZER 	PORTA
#define PORTRxBUZZER 	PINA
#define CONFIGIOxBUZZER 	DDRA
#define PINxBUZZER	0


//ikey layout
//#define KB_LYOUT_LEFT_STARTSTOP 0
//#define KB_LYOUT_LEFT_SLEEP 1
//#define KB_LYOUT_LEFT_DOWN 2
//#define KB_LYOUT_LEFT_UP 3
////
//#define KB_LYOUT_PROGRAM 4
//
//#define KB_LYOUT_RIGHT_DOWN 5	//7
//#define KB_LYOUT_RIGHT_UP	6	//8
//#define KB_LYOUT_RIGHT_SLEEP 7	//6
//#define KB_LYOUT_RIGHT_STARTSTOP 8	//5


//ikey layout
#define KB_LYOUT_LEFT_STARTSTOP 0
#define KB_LYOUT_LEFT_DOWN 1
#define KB_LYOUT_LEFT_UP 2
//
#define KB_LYOUT_PROGRAM 3

#define KB_LYOUT_RIGHT_DOWN 4
#define KB_LYOUT_RIGHT_UP	5
#define KB_LYOUT_RIGHT_STARTSTOP 6


#endif /* MAIN_H_ */
