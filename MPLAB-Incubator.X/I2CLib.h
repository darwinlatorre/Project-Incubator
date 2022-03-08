#ifndef XC_I2CLIB_H
#define	XC_I2CLIB_H

#include "FunctionsHeader.h"

//#define _XTAL_FREQ 8000000
//#include <stdint.h>
//#include <xc.h>

void I2CInit (uint32_t frecuencia);//Funci�n de inicializaci�n del periferico I2C.
void I2CStart (void);              // Funci�n que inicia la comunicaci�n I2C.
void I2CReStart (void);            // Funci�n que reinicia la comuncaci�n I2C.
void I2CStop (void);               // Funci�n que detiene la comunicaci�n I2c.
void I2CAck (void);                // Funci�n para transmitir Acknowledge.
void I2CNoAck (void);             // Funci�n para transmitir NO Acknowledge.
void I2CWrite (uint8_t data);      // Funci�n para escribir el SSPBUF.
uint8_t I2CRead (void);            // Funci�n para leer el SSPBUF.

#endif	/* XC_I2CLIB_H */