#ifndef XC_FUNCTIONSHEADER_H
#define	XC_FUNCTIONSHEADER_H

#define byte uint8_t

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "BITS_Configuration.h"
#include "KeyBoardLib.h"
#include "ADCLib.h"
#include "LCD20x4Lib.h"
#include "I2CLib.h"                   // Incluimos la librería I2C.

#define P_90grados      PORTEbits.RE1
#define P_180grados     PORTEbits.RE0
#define Pin_ServoMotor  LATCbits.LATC0
#define IndicadorHmax   LATAbits.LA4
#define IndicadorHmin   LATAbits.LA5
#define LAMPARA         LATCbits.LATC2
#define VENTILADOR      LATCbits.LATC1

void ejecutaComandoPresionado(unsigned char prmComando);
void StartOption();
void DataProcess();
void MostrarTemperatura();
void MostrarHumedad();
void ConfigServoMotor();
void msDelay(unsigned int val);

#endif	/* XC_FUNCTIONSHEADER_H */

