
#include "main.h"
#include "pinGetLevel/pinGetLevel.h"
#include "psmode_program.h"
#include "psmode_operative.h"
#include "psmode_viewTemp.h"
#include "indicator/indicator.h"
#include "pid/pid.h"
#include "Display7S/display7s.h"
#include "display7s_setup.h"
#include "disp7s_applevel.h"



volatile struct _isr_flag
{
	unsigned sysTickMs :1;
	unsigned __a :7;
} isr_flag = { 0 };
struct _mainflag mainflag;

const struct _process ps_reset;
struct _fryer fryer;
const struct _fryer fryer_reset;

struct _job job_captureTemperature;
const struct _job job_reset;

//k-load from EEPROM
struct _t EEMEM COOKTIME[BASKET_MAXSIZE]= { {7, 0}, {7,0} };    //mm:ss

/* Declare PID objects*/
struct PID mypid0;

/* Setear constantes para el primer objeto PID*/
/* cada PWM_ACCESS_TIME_MS se lleva el timing del periodo del PWM */
#define PWM_ACCESS_TIME_MS 100 //ms

void mypid0_set(void)
{
	mypid0.pwm.timing.kmax_ticks_ms = PWM_ACCESS_TIME_MS/SYSTICK_MS;//100 ms

	/* tengo que convertir a unidades de tiempo */
	mypid0.algo.scaler_time_ms = 1000.0f/PWM_ACCESS_TIME_MS;	// 1seg/PWM_ACCESS_TIME_MS


	/* aqui es donde realmente se fija el valor */
	/* todo depende practicamente del valor asignado asignado a KP*/
	mypid0.algo.pid_out_max_ms = 10 * mypid0.algo.scaler_time_ms; //10s
	mypid0.algo.pid_out_min_ms = 0 * mypid0.algo.scaler_time_ms;	//0s

	/* El sistema presenta esos delays */
	mypid0.pwm.timing.k_systemdelay_ton_ms = 500.0f / PWM_ACCESS_TIME_MS;
	mypid0.pwm.timing.k_systemdelay_toff_ms = 5000.0f/ PWM_ACCESS_TIME_MS;

	/* PID ktes x algorithm */

	/* 10 es la salida deseada del controlador PID que representa
	 * el valor maximo (mayor o igual a este siempre será acotado a 10)
	 *
	 * +50 es el la error SP-PV, a partir de esa diferencia queremos el controlador
	 * empieza a regular entre 10..0 de manera proporcional
	 *  */
	mypid0.algo.kp = 10.0f / 50;
	//pid.algo.kp = 1.0f/5;

	mypid0.algo.ki = 1.0f/10;
	mypid0.algo.kd = 0;
	mypid0.algo.kei_windup_min = 0;
	mypid0.algo.kei_windup_max = 10;

	mypid0.pwm.io.port = &PORTWxSOL_GAS_QUEMADOR;
	mypid0.pwm.io.pin = PINxKB_SOL_GAS_QUEMADOR;
	//
	//mypid0.algo.sp = 300;
}

/* cada objeto PID es particular y necesita ser afinado antes de pasar al control */
int16_t mypid0_adjust_kei_windup(void)
{
	/* 1. error es target-specific */
	//int16_t pv = TCtemperature;
	//int16_t pv = 110;
	//int16_t error =  mypid0.algo.sp - pv;
	int16_t error =  tmprture_coccion.TC - TCtemperature;

	/* adjust windup for integral error */
	if (error > 5)
	{
		mypid0.algo.kei_windup_max = 10;
	}
	else
	{
		mypid0.algo.kei_windup_max = 1;
	}
	return error;
}

void kbmode_default(struct _kb_basket *kb)
{
	struct _key_prop key_prop = { 0 };
	//
	key_prop = propEmpty;
	//
	key_prop.uFlag.f.onKeyPressed = 1;
	ikb_setKeyProp(kb->sleep,key_prop);
	ikb_setKeyProp(kb->startStop ,key_prop);
	//
	key_prop.uFlag.f.onKeyPressed = 0;
	key_prop.uFlag.f.atTimeExpired2 = 1;
	ikb_setKeyProp(kb->program ,key_prop);//programacion
	//
	key_prop.uFlag.f.atTimeExpired2 = 0;
	//
	key_prop.uFlag.f.onKeyPressed = 1;
	key_prop.uFlag.f.reptt = 1;
	key_prop.numGroup = 0;
	key_prop.repttTh.breakTime = (uint16_t) 200.0 / KB_PERIODIC_ACCESS;
	key_prop.repttTh.period = (uint16_t) 50.0 / KB_PERIODIC_ACCESS;
	ikb_setKeyProp(kb->down ,key_prop);
	ikb_setKeyProp(kb->up ,key_prop);
}
void kbmode_2basket_set_default(void)
{
	for (int i=0; i<BASKET_MAXSIZE; i++)
	{
		kbmode_default(&fryer.basket[i].kb);
	}
}
void fryer_init(void)
{
	fryer = fryer_reset;
	//++--
	/* se usara la etiqueta KB_LYOUT_PROGRAM y no el [i]kb.program, xq es comun para ambos */
	fryer.basket[BASKET_LEFT].kb.startStop = KB_LYOUT_LEFT_STARTSTOP;
	//fryer.basket[BASKET_LEFT].kb.sleep = KB_LYOUT_LEFT_SLEEP;
	fryer.basket[BASKET_LEFT].kb.down = KB_LYOUT_LEFT_DOWN;
	fryer.basket[BASKET_LEFT].kb.up = KB_LYOUT_LEFT_UP;
	fryer.basket[BASKET_LEFT].kb.program = KB_LYOUT_PROGRAM;//comun a ambos

	fryer.basket[BASKET_LEFT].display.cursor.x = BASKETLEFT_DISP_CURSOR_START_X;//0x00;
	fryer.basket[BASKET_LEFT].display.cursor.y = 0x00;

	//
	fryer.basket[BASKET_RIGHT].kb.startStop = KB_LYOUT_RIGHT_STARTSTOP;
	//fryer.basket[BASKET_RIGHT].kb.sleep = KB_LYOUT_RIGHT_SLEEP;
	fryer.basket[BASKET_RIGHT].kb.down = KB_LYOUT_RIGHT_DOWN;
	fryer.basket[BASKET_RIGHT].kb.up = KB_LYOUT_RIGHT_UP;
	fryer.basket[BASKET_RIGHT].kb.program = KB_LYOUT_PROGRAM;//comun a ambos

	fryer.basket[BASKET_RIGHT].display.cursor.x = BASKETRIGHT_DISP_CURSOR_START_X;//0x0B;
	fryer.basket[BASKET_RIGHT].display.cursor.y = 0x00;
	//--++
}

void ind(void)
{

	int8_t c=0;
	int8_t i=0;
	while (1)
	{
		if (isr_flag.sysTickMs)
		{
			isr_flag.sysTickMs = 0;
			mainflag.sysTickMs = 1;
		}
		if (mainflag.sysTickMs)
		{
			if (++c == (20/SYSTICK_MS))    //20ms
			{
				c = 0;

				ikb_job();
			}
		}



		if (ikb_key_is_ready2read(fryer.basket[i].kb.down))
		{
			ikb_key_was_read(fryer.basket[i].kb.down);

			indicator_setKSysTickTime_ms(1000/SYSTICK_MS);
			indicator_On();
		}
		if (ikb_key_is_ready2read(fryer.basket[i].kb.up))
		{
			ikb_key_was_read(fryer.basket[i].kb.up);
			indicator_setKSysTickTime_ms(75/SYSTICK_MS);
			indicator_On();
		}
		indicator_job();
		mainflag.sysTickMs = 0;
	}
}

int main(void)
{
	int8_t sm0 = 0;
	int c = 0;
	unsigned char str[10];//cambiar a char_arr


	disp7s_init();//new

	eeprom_read_block((struct _Tcoccion *)&tmprture_coccion , (struct _Tcoccion *)&TMPRTURE_COCCION, sizeof(struct _Tcoccion) );

	fryer_init();

	//Tiempo Necesario para estabilizar la tarjeta
	//__delay_ms(2000);//estabilizar tarjeta de deteccion

	//Active pull-up
	PinTo1(PORTWxKB_KEY0, PINxKB_KEY0);
	PinTo1(PORTWxKB_KEY1, PINxKB_KEY1);
	PinTo1(PORTWxKB_KEY2, PINxKB_KEY2);
	PinTo1(PORTWxKB_KEY3, PINxKB_KEY3);
	PinTo1(PORTWxKB_KEY4, PINxKB_KEY4);
	PinTo1(PORTWxKB_KEY5, PINxKB_KEY5);
	PinTo1(PORTWxKB_KEY6, PINxKB_KEY6);
	__delay_ms(1);
	ikb_init();

	pinGetLevel_init(); //with Changed=flag activated at initialization


	PinTo0(PORTWxSOL_GAS_QUEMADOR, PINxKB_SOL_GAS_QUEMADOR);
	ConfigOutputPin(CONFIGIOxSOL_GAS_QUEMADOR, PINxKB_SOL_GAS_QUEMADOR);

	//
	ConfigOutputPin(CONFIGIOxBUZZER, PINxBUZZER);
	indicator_setPortPin(&PORTWxBUZZER, PINxBUZZER);
	indicator_setKSysTickTime_ms(75/SYSTICK_MS);

	//lcdan_init();

	InitSPI_MASTER();


	//With prescaler 64, gets 1 ms exact (OCR0=249)
	TCNT0 = 0x00;
	TCCR0 = (1 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00); //CTC, PRES=64
	OCR0 = CTC_SET_OCR_BYTIME(1e-3, 64); //TMR8-BIT @16MHz @PRES=1024-> BYTIME maximum = 16ms
	TIMSK |= (1 << OCIE0);

	sei();

	mypid0_set();	//1 vez


int8_t systick_counter0=0;

//DDRC = 0xFF;
//PORTC = DISP7Sinvfix(D7S_DATA_t);
//PinTo1(PORTWxDISPLAY7S_Q6, PINxDISPLAY7S_Q6);
//while (1);

/*
while (1)
{
	if (isr_flag.sysTickMs)
	{
		isr_flag.sysTickMs = 0;
		mainflag.sysTickMs = 1;
	}
	//----------------------------------
	if ( temperature_job() == 1)
	{
		MAX6675_formatText3dig(TCtemperature, str);
		disp7s_update_data_array(str, BASKETRIGHT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);

	}
	if (mainflag.sysTickMs)
	{
		if (++systick_counter0 == (1/SYSTICK_MS) )//ms
		{
			systick_counter0 = 0x00;
			disp7s_job();
		}

	}
	mainflag.sysTickMs = 0;
}
*/
///////////////////////////////////////

	while (1)
	{
		if (isr_flag.sysTickMs)
		{
			isr_flag.sysTickMs = 0;
			mainflag.sysTickMs = 1;
		}

		if (mainflag.sysTickMs)
		{
			if (++systick_counter0 == (1/SYSTICK_MS) )//ms
			{
				systick_counter0 = 0x00;
				disp7s_job();
			}
		}


		if (sm0 == 0)
		{
			/* sm0 == 0, es un init para todos los procesos q necesitan de mainflag.sysTickMs al iniciar el sistema*/
			if ( temperature_job() == 1)
			{
				if (TCtemperature == MAX6675_THERMOCOUPLED_OPEN)
				{
					MAX6675_formatText3dig(TCtemperature, str);
					disp7s_update_data_array(str, BASKETRIGHT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);
				}
				else
				{
					sm0++;
				}
			}
		}
		else
		{
			//----------------------
			if (mainflag.sysTickMs)
			{
				if (++c == (20/SYSTICK_MS))    //20ms
				{
					c = 0;
					//
					pinGetLevel_job();
					//---------------------------
					if (pinGetLevel_hasChanged(PGLEVEL_LYOUT_SWONOFF))
					{
						indicator_setKSysTickTime_ms(75/SYSTICK_MS);
						indicator_On();
						//
						//lcdan_clear();
						disp7s_clear_all();

						if (pinGetLevel_level(PGLEVEL_LYOUT_SWONOFF)== 0)	//active in low
						{
							/* ON*/

							/* forzar en sacar el nivel del PWM para el primer periodo, ya que T = 10s*/
							int16_t error = mypid0_adjust_kei_windup(); /* dejar preparado para job()*/
							pid_find_ktop_ms(&mypid0, error);
							pid_pwm_stablish_levelpin(&mypid0);//set PWM por primera vez//tener de inmediato el valor de ktop_ms
						}
						else
						{
							/* OFF */

							/* pin to 0*/
							pid_pwm_set_pin(&mypid0, 0);

							//lcdan_set_cursor(6, 0);
							//lcdan_print_PSTRstring(PSTR("OFF"));
							disp7s_update_data_array(DIPS7S_MSG_OFF, BASKETRIGHT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);

						}
						/* en ambos casos, iniciar desde el principio*/
						sm0 = 0x00;
						fryer_init();
						//
						pinGetLevel_clearChange(PGLEVEL_LYOUT_SWONOFF);
					}
					//chispero();
					ikb_job();
				}
			}


			//
			if (pinGetLevel_level(PGLEVEL_LYOUT_SWONOFF)== 0)
			{
				if (sm0 == 1)
				{
					kbmode_2basket_set_default();

					//lcdan_set_cursor(0, 0);
					//lcdan_print_PSTRstring(PSTR("MELT"));
					disp7s_update_data_array(DIPS7S_MSG_PRECALENTAMIENTO, BASKETLEFT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);


					//
					fryer.bf.preheating = 1;
					fryer.viewmode = FRYER_VIEWMODE_PREHEATING;
					//

					//igDeteccFlama_resetJob();
					sm0++;
				}
				else if (sm0 == 2)
				{
					if (1)//(igDeteccFlama_doJob())
					{
						sm0++;    	//OK...Ignicion+deteccion de flama OK
									//PID_Control -> setpoint = Tprecalentamiento
					}
				}
				else if (sm0 == 3)
				{
					//Precalentamiento
					//if (TCtemperature >= mypid0.algo.sp)

					if (fryer.viewmode == FRYER_VIEWMODE_PREHEATING)
					{
						if (TCtemperature >= tmprture_coccion.TC)
						{
							//
							indicator_setKSysTickTime_ms(1000/SYSTICK_MS);
							indicator_On();
							//
							fryer.bf.preheating = 0;

							fryer.viewmode = FRYER_VIEWMODE_COOK;
							for (int i=0; i<BASKET_MAXSIZE; i++)//added
							{
								kbmode_default(&fryer.basket[i].kb);
								fryer.basket[i].kbmode = KBMODE_DEFAULT;
							}
							fryer.bf.operative_mode = 1;
							//

							sm0++;
						}
					}
				}
				else if (sm0 == 4)
				{
				}

				if ((fryer.viewmode == FRYER_VIEWMODE_PREHEATING) || (fryer.viewmode == FRYER_VIEWMODE_COOK))
				{
					if (ikb_key_is_ready2read(KB_LYOUT_PROGRAM))
					{
						ikb_key_was_read(KB_LYOUT_PROGRAM);

						if ( ikb_get_AtTimeExpired_BeforeOrAfter(KB_LYOUT_PROGRAM) == KB_AFTER_THR)
						{
							fryer.viewmode = FRYER_VIEWMODE_PROGRAM;
							fryer.ps_program = ps_reset;

							indicator_setKSysTickTime_ms(1000/SYSTICK_MS);
							indicator_On();

							//Salir actualizando eeprom
							for (int i=0; i<BASKET_MAXSIZE; i++)
							{
								eeprom_update_block( (struct _t *)(&basket_temp[i].cookCycle.time), (struct _t *)(&COOKTIME[i]), sizeof(struct _t));
							}
						}
						//added 7 abr 2022
						else//KB_BEFORE_THR
						{
							/* Visualizar la temperatura */
							fryer.viewmode = FRYER_VIEWMODE_VIEWCOOKTEMP;
							fryer.ps_viewTemp = ps_reset;

							indicator_setKSysTickTime_ms(1000/SYSTICK_MS);
							indicator_On();
						}
					}
				}
				//
				if (fryer.viewmode == FRYER_VIEWMODE_PROGRAM)
				{
					if (psmode_program() == 1)
					{
						if (fryer.bf.preheating == 1)//sigue en precalentamiento ?
						{
							fryer.viewmode = FRYER_VIEWMODE_PREHEATING;

							//set kb
							for (int i=0; i<BASKET_MAXSIZE; i++)
							{
								kbmode_default(&fryer.basket[i].kb);
							}
//							lcdan_clear();
//							lcdan_set_cursor(0, 0);
//							lcdan_print_PSTRstring(PSTR("MELT"));
							disp7s_clear_all();
							disp7s_update_data_array(DIPS7S_MSG_PRECALENTAMIENTO, BASKETLEFT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);
						}
						else
						{
							fryer.viewmode = FRYER_VIEWMODE_COOK;
							for (int i=0; i<BASKET_MAXSIZE; i++)//added
							{
								kbmode_default(&fryer.basket[i].kb);
								fryer.basket[i].kbmode = KBMODE_DEFAULT;
							}
							//added
							lcdanBuff_dump2device(lcdanBuff);
						}

					}
				}

				//added 7 ab 2022
				if (fryer.viewmode == FRYER_VIEWMODE_VIEWCOOKTEMP)
				{
					if (psmode_viewTemp() == 1)
					{
						if (fryer.bf.preheating == 1)	//sigue en precalentamiento
						{
							fryer.viewmode = FRYER_VIEWMODE_PREHEATING;

							//set kb
							for (int i=0; i<BASKET_MAXSIZE; i++)
							{
								kbmode_default(&fryer.basket[i].kb);
							}
//							lcdan_clear();
//							lcdan_set_cursor(0, 0);
//							lcdan_print_PSTRstring(PSTR("MELT"));
							disp7s_clear_all();
							disp7s_update_data_array(DIPS7S_MSG_PRECALENTAMIENTO, BASKETLEFT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);
						}
						else
						{
							fryer.viewmode = FRYER_VIEWMODE_COOK;

							for (int i=0; i<BASKET_MAXSIZE; i++)//added
							{
								kbmode_default(&fryer.basket[i].kb);
								fryer.basket[i].kbmode = KBMODE_DEFAULT;
							}
							//added
							lcdanBuff_dump2device(lcdanBuff);
						}
					}
				}

				if (fryer.bf.operative_mode == 1)
				{
					psmode_operative();
				}

				/* PID control */
				int16_t error = mypid0_adjust_kei_windup();
				pid_job(&mypid0, error);
				//
			}
			else //switch OFF
			{

			}

			/* actua sobre el buzzer */
			indicator_job();

			temperature_job();

		}//end sm0 == 0

		/* ---------- */
		mainflag.sysTickMs = 0;
	}
	return 0;
}

ISR(TIMER0_COMP_vect)
{
	isr_flag.sysTickMs = 1;
}
