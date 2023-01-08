/*
 * temperature.c
 *
 *  Created on: Dec 3, 2020
 *      Author: jcaf
 */

#include "../main.h"
#include "../MAX6675/MAX6675.h"
#include "../smoothAlg/smoothAlg.h"
#include "temperature.h"
#include "../psmode_program.h"
#include "../disp7s_applevel.h"
#include "../utils/utils.h"

#ifdef MAX6675_UTILS_LCD_PRINT3DIG_C
/*****************************************************
Format with 3 digits 999C
*****************************************************/
void MAX6675_formatText3dig_C(int16_t temper, char *str_out)
{
    char buff[10];

	if (temper == MAX6675_THERMOCOUPLED_OPEN)
	{
		strcpy(str_out,"N.C ");//4posit
        return;
    }
    else
    {
        itoa(temper, buff, 10);//convierte

        //4 positions to display
        strcpy(str_out,"   C");

        if (temper< 10)
        {
            strncpy(&str_out[2], buff, 1);
        }
        else if (temper<100)
        {
            strncpy(&str_out[1], buff, 2);
        }
        else if (temper<1000)
        {
            strncpy(&str_out[0], buff, 3);
        }
        else
        {
            strncpy(&str_out[0], buff, 4);
        }
    }
}
#endif
#ifdef MAX6675_UTILS_LCD_PRINT3DIG
/*****************************************************
Format with 4 digits 999 sin grados ni C
*****************************************************/


void MAX6675_formatText3dig(int16_t temperatura, unsigned char *str_out)
{
	//1. clear the basket display
	disp7s_blank_displays(str_out, 0, BASKET_DISP_MAX_CHARS_PERBASKET);

	if (temperatura == MAX6675_THERMOCOUPLED_OPEN)
	{
		str_out[0] = D7S_DATA_n | (1<< D7S_DP);
		str_out[1] = D7S_DATA_c;
	}
	else if (temperatura>999)
	{
		str_out[0] = D7S_DATA_o;
		str_out[1] = D7S_DATA_u;
		str_out[2] = D7S_DATA_t;
	}
	else//numerical
	{
		unsigned char bcd[10];
		int k = integer_to_arraybcd_msb_lsb(temperatura, bcd);
		int idx= ((BASKET_DISP_MAX_CHARS_PERBASKET))-1 -k;
		for (int i = 0; i< k; i++ )
		{
			str_out[idx++] = DISP7_NUMERIC_ARR[bcd[i]];
		}
		str_out[BASKET_DISP_MAX_CHARS_PERBASKET-1] = D7S_DATA_GRADE_CENTIGRADE;
	}
	//fix right basket: upsidedown displays
	disp7s_fix_upsidedown_display(&str_out[2]);
}
#endif
/*****************************************************

*****************************************************/
#ifdef MAX6675_UTILS_LCD_PRINTCOMPLETE_C
void MAX6675_convertIntTmptr2str_wformatPrintComplete(int16_t temper, char *str_out)
{
    char buff[10];

    if (temper < 0)
    {
        if (temper == -1)
            {strcpy(str_out,"N.C  ");}
        return;
    }
    else
    {
        itoa(temper, buff, 10);

        //5 positions to display: 1023 + C
        strcpy(str_out,"    C");

        if (temper< 10)
        {
            strncpy(&str_out[3], buff, 1);
        }
        else if (temper<100)
        {
            strncpy(&str_out[2], buff, 2);
        }
        else if (temper<1000)
        {
            strncpy(&str_out[1], buff, 3);
        }
        else
        {
            strncpy(&str_out[0], buff, 4);
        }
    }
}
#endif


#define TEMPERATURE_SMOOTHALG_MAXSIZE 4// 8
static int16_t smoothVector[TEMPERATURE_SMOOTHALG_MAXSIZE];

struct _smoothAlg smoothAlg_temp;
const struct _smoothAlg smoothAlg_reset;

int8_t MAX6675_accSamples(void)
{
	int16_t temperature12bits = MAX6675_get12bitsTemp();

	//
	if (temperature12bits == MAX6675_THERMOCOUPLED_OPEN)    //error cable disconnected ?
	{
		job_captureTemperature = job_reset;
		smoothAlg_temp = smoothAlg_reset;

		return MAX6675_THERMOCOUPLED_OPEN;//-1
	}
	else
	{
		smoothVector[job_captureTemperature.counter0] = temperature12bits;
		if (++job_captureTemperature.counter0 >= TEMPERATURE_SMOOTHALG_MAXSIZE)
		{
			job_captureTemperature.counter0 = 0x00;
			return 1;
		}
	}
	return 0;
}

/*****************************************************

*****************************************************/
int8_t MAX6675_smoothAlg_nonblock_job(int16_t *TCtemperature)
{
	float smoothAnswer;

	if (smoothAlg_nonblock(&smoothAlg_temp, smoothVector, TEMPERATURE_SMOOTHALG_MAXSIZE, &smoothAnswer))
	{
		if (smoothAnswer > 0.0f)
		{
			*TCtemperature = ( (smoothAnswer * MAX6675_TMPR_MAX)/ (4095) )  + MAX6675_TEMPERATURE_DEVIATION;;
		}
		else
		{
			*TCtemperature = 0;
		}
		return 1;
	}
	return 0;
}

/*****************************************************
tendria que cambiar la temperature_job para saber cuando tiene correctamente la temperatura
para poder leer al inicio del programa, ojo xq se necesita el flag de systick
*****************************************************/
int TCtemperature;
//void
int8_t temperature_job(void)
{
	int8_t codret = 0;

	static int8_t MAX6675_sm0;
	static uint16_t MAX6675_ConversionTime_access;
	int8_t MAX6675_job_rpta;

	if (MAX6675_sm0 == 0)
	{
		if (mainflag.sysTickMs)
		{
			if (++MAX6675_ConversionTime_access >= (uint16_t)(220.0f/SYSTICK_MS) ) //MAX6675 has max 0.22s
			{
				MAX6675_ConversionTime_access = 0;
				//

				MAX6675_job_rpta = MAX6675_accSamples();
				if (MAX6675_job_rpta == MAX6675_THERMOCOUPLED_OPEN)
				{
					MAX6675_ConversionTime_access = 0;//reset the count with valid thermocouple value
					TCtemperature = MAX6675_THERMOCOUPLED_OPEN;
					codret = 1;
				}
				else if (MAX6675_job_rpta == 1)//00..1024C
				{
					MAX6675_sm0++;
				}
			}
		}
	}
	else
	{
		if (MAX6675_smoothAlg_nonblock_job( &TCtemperature ))
		{
			/* fin del proceso */
			//Por defecto MAX6675 entrega en grados Celsius
			if (pgrmode.bf.unitTemperature == FAHRENHEIT)
			{
				TCtemperature = (TCtemperature*(9.0f/5)) + 32;
			}

			MAX6675_sm0 = 0x00;

			codret = 1;	//fin del proceso
		}
	}
	return codret;
}
