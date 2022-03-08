#ifndef XC_RTCLIB_H
#define	XC_RTCLIB_H

#include "I2CLib.h"

uint8_t hours;                             // Variable de 8 bits para las horas.
uint8_t minutes;                           // Variable de 8 bits para los minutos.
uint8_t seconds;                           // Variable de 8 bits para los segundos.
uint8_t year;                              // Variable de 8 bits para los año.
uint8_t month;                             // Variable de 8 bits para el mes.
uint8_t day;                               // Variable de 8 bits para el dia.
uint8_t i;                                 // Variable de 8 bits.

char Time[] = "TIME: 00:00:00";     // Arreglo estático char "Time". 
char Date[] = "DATE: 00/00/2000";   // Arreglo estático char "Date".

uint8_t Decimal_a_BCD (uint8_t);    // Función que convierte un número decimal a BCD. 
uint8_t BCD_a_Decimal (uint8_t);    // Función que convierte un número BCD a decimal.

void readTime();


#endif	/* XC_RTCLIB_H */