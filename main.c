/*
 Atmega32
 ----------------------------------------
 1)    http://www.engbedded.com/fusecalc/
 lock bits:
 http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p
 2) verificar que responda el atmega (ONLY A RESET)
 [jcaf@jcafpc ~]$ avrdude -c usbasp -B5 -p m32

 3) programar fuse (sin preservar EEPROM)
 -U lfuse:w:0xbf:m -U hfuse:w:0xcf:m
 [jcaf@jcafpc ~]$ avrdude -c usbasp -B5 -p m32 -U lfuse:w:0xbf:m -U hfuse:w:0xcf:m

 4) GRABAR EL CODIGO FUENTE CON EL COMANDO ACOSTUMBRADO
 [root@JCAFPC Release]# avrdude -c usbasp -B5 -p m32 -U flash:w:digitalFryer.hex
 [root@JCAFPC Release]# avrdude -c usbasp -B1 -p m32 -V -U flash:w:digitalFryer.hex (SIN VERIFICAR)
 [jcaf@JCAFPC Release]$ avrdude -c usbasp -B5 -p m32 (ONLY A RESET)

 NUEVO
 [root@JCAFPC Release]# avrdude -c usbasp -B0.3 -p m32 -V -U flash:w:digitalFryer.hex (MAS RAPIDO!)
 Tambien puede ser sin -BX.. cuando ya esta bien configurado los fuses:
 [root@JCAFPC Release]# avrdude -c usbasp -p m32 -U flash:w:digitalFryer.hex

 5)
 8:50 a. m.
 GRABAR LA EEPROM
 [jcaf@jcafpc Release]$ avrdude -c usbasp -B4 -p m32 -V -U eeprom:w:digitalFryer.eep

 6) programar fuse (PRESERVANDO EEPROM)

 avrdude -c usbasp -B5 -p m32 -U lfuse:w:0xbf:m -U hfuse:w:0xc7:m

 7) Verificar los fuses
 [jcaf@jcafpc Release]$ avrdude -c usbasp -B4 -p m32 -U lfuse:r:-:i -v

 +++++++++++++++++++++++
 Acomodar para Atmega32A
 proteger flash (modo 3): lectura y escritura

 avrdude -c usbasp -B10 -p m32 -U lock:w:0xFC:m

 (ignorar el error de 0x3C... pues los 2 bits de mayor peso no estan implentados)
*/

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

struct _main_schedule main_schedule;
const struct _main_schedule main_schedule_reset;


const struct _process ps_reset;
struct _fryer fryer;
const struct _fryer fryer_reset;

struct _job job_captureTemperature;
const struct _job job_reset;

//k-load from EEPROM
struct _t EEMEM COOKTIME[BASKET_MAXSIZE]= { {7, 10}, {7,10} };    //mm:ss

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
	 * +50 es el error SP-PV, a partir de esa diferencia queremos el controlador
	 * empieza a regular entre 10..0 de manera proporcional
	 *  */
	mypid0.algo.kp = 10.0f / 50;
	//pid.algo.kp = 1.0f/5;

	mypid0.algo.ki = 1.0f/10;
	mypid0.algo.kd = 0;
	mypid0.algo.kei_windup_min = 0;
	mypid0.algo.kei_windup_max = 10;

	mypid0.pwm.io.port = &PORTWxSOL_GAS_QUEMADOR;
	mypid0.pwm.io.pin = PINxSOL_GAS_QUEMADOR;
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


int main(void)
{
	int counter0 = 0;
	int counter1 = 0;
	unsigned char str[10];//cambiar a char_arr
	unsigned char data_array_buffer[DISP7S_TOTAL_NUMMAX];

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


	PinTo0(PORTWxSOL_GAS_QUEMADOR, PINxSOL_GAS_QUEMADOR);
	ConfigOutputPin(CONFIGIOxSOL_GAS_QUEMADOR, PINxSOL_GAS_QUEMADOR);


	PinTo0(PORTWxCONTROLLER_ONOFF, PINxCONTROLLER_ONOFF);
	ConfigOutputPin(CONFIGIOxCONTROLLER_ONOFF, PINxCONTROLLER_ONOFF);

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


	while (1)
	{
		if (isr_flag.sysTickMs)
		{
			isr_flag.sysTickMs = 0;
			mainflag.sysTickMs = 1;
		}

		if (mainflag.sysTickMs)
		{
			if (++systick_counter0 >= (1/SYSTICK_MS) )//ms
			{
				systick_counter0 = 0x00;
				disp7s_job();
			}
		}

		if (temperature_job())
		{
			main_schedule.bf.startup_finish_stable_temperature = STARTUP_FINISHED;

			if (TCtemperature == MAX6675_THERMOCOUPLED_OPEN)
			{
				disp7s_clear_all();
				MAX6675_formatText3dig(TCtemperature, str);
				disp7s_update_data_array(str, BASKETRIGHT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);

				main_schedule.bf.sensor_status_thermocuple = SENSOR_STATUS_BAD;
			}
			else
			{
				main_schedule.bf.sensor_status_thermocuple = SENSOR_STATUS_OK;
			}
		}

		//----------------------
		if (mainflag.sysTickMs)
		{
			if (++counter0 == (20/SYSTICK_MS))    //20ms
			{
				counter0 = 0;

				//
				pinGetLevel_job();
				//---------------------------

				if (pinGetLevel_hasChanged(PGLEVEL_LYOUT_SWONOFF))
				{
					pinGetLevel_clearChange(PGLEVEL_LYOUT_SWONOFF);
					//
					main_schedule.bf.startup_finish_read_switch_onoff = STARTUP_FINISHED;

					//
					indicator_setKSysTickTime_ms(75/SYSTICK_MS);
					indicator_On();
					//
					disp7s_clear_all();

					if (pinGetLevel_level(PGLEVEL_LYOUT_SWONOFF)== 0)	//active in low
					{
						/* ON*/
						/* forzar en sacar el nivel del PWM para el primer periodo, ya que T = 10s*/
						int16_t error = mypid0_adjust_kei_windup(); /* dejar preparado para job()*/
						pid_find_ktop_ms(&mypid0, error);
						pid_pwm_stablish_levelpin(&mypid0);//set PWM por primera vez//tener de inmediato el valor de ktop_ms

						//
						main_schedule.bf.switch_status_onoff = 1;
						PinTo1(PORTWxCONTROLLER_ONOFF, PINxCONTROLLER_ONOFF);
					}
					else
					{
						main_schedule.bf.switch_status_onoff = 0;

						disp7s_update_data_array(DIPS7S_MSG_OFF, BASKETRIGHT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);
						//
						pid_pwm_set_pin(&mypid0, 0);
						fryer_init();
						main_schedule = main_schedule_reset;
						//
						PinTo0(PORTWxCONTROLLER_ONOFF, PINxCONTROLLER_ONOFF);
					}
				}
			}
		}


		if (main_schedule.bf.startup_finish_read_switch_onoff == STARTUP_FINISHED)
		{
			if (main_schedule.bf.switch_status_onoff == 1)
			{
				if ((main_schedule.bf.startup_finish_stable_temperature == STARTUP_FINISHED) && (main_schedule.bf.sensor_status_thermocuple == SENSOR_STATUS_OK))
				{
					//the machine can operate properly
					if (main_schedule.sm0 == 0)
					{
						kbmode_2basket_set_default();

						disp7s_update_data_array(DIPS7S_MSG_PRECALENTAMIENTO, BASKETLEFT_DISP_CURSOR_START_X, BASKET_DISP_MAX_CHARS_PERBASKET);
						//
						fryer.bf.preheating = 1;
						fryer.viewmode = FRYER_VIEWMODE_PREHEATING;
						//
						//igDeteccFlama_resetJob();
						main_schedule.sm0++;
					}
					else if (main_schedule.sm0 == 1)
					{
						if (1)//(igDeteccFlama_doJob())
						{
							main_schedule.sm0++;    	//OK...Ignicion+deteccion de flama OK
														//PID_Control -> setpoint = Tprecalentamiento
						}
					}
					else if (main_schedule.sm0 == 2)
					{
						//Precalentamiento
						if (fryer.viewmode == FRYER_VIEWMODE_PREHEATING)
						{
							//if (TCtemperature >= tmprture_coccion.TC)
							if (TCtemperature >= 0)
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

								main_schedule.sm0++;
							}
						}
					}
					else if (main_schedule.sm0 == 3)
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

								disp7s_save_data_array(data_array_buffer, DISP7S_TOTAL_NUMMAX);
							}
							//added 7 abr 2022
							else//KB_BEFORE_THR
							{
								/// Visualizar la temperatura
								fryer.viewmode = FRYER_VIEWMODE_VIEWCOOKTEMP;
								fryer.ps_viewTemp = ps_reset;

								indicator_setKSysTickTime_ms(1000/SYSTICK_MS);
								indicator_On();

								//added

								disp7s_save_data_array(data_array_buffer, DISP7S_TOTAL_NUMMAX);
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
								//lcdan_print_PSTRstring(PSTR("MELT"));
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
								//lcdanBuff_dump2device(lcdanBuff);
								disp7s_update_data_array(data_array_buffer, 0, DISP7S_TOTAL_NUMMAX);
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
								//lcdan_print_PSTRstring(PSTR("MELT"));
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
								//lcdanBuff_dump2device(lcdanBuff);
								disp7s_update_data_array(data_array_buffer, 0, DISP7S_TOTAL_NUMMAX);

							}
						}
					}

					if (fryer.bf.operative_mode == 1)
					{
						psmode_operative();
					}

					// PID control
					int16_t error = mypid0_adjust_kei_windup();
					pid_job(&mypid0, error);

					if (mainflag.sysTickMs)
					{
						if (++counter1 == (20/SYSTICK_MS))
						{
							counter1 = 0;
							//
							ikb_job();
						}
					}
				}
			}
		}

		indicator_job();
		mainflag.sysTickMs = 0;

	}//while end

	return 0;
}

ISR(TIMER0_COMP_vect)
{
	isr_flag.sysTickMs = 1;
}
