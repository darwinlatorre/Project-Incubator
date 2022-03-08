#ifndef XC_I2CLIB_H
#define	XC_I2CLIB_H

#include "FunctionsHeader.h"

//#define _XTAL_FREQ 8000000
//#include <stdint.h>
//#include <xc.h>

void I2CInit (uint32_t frecuencia);//Función de inicialización del periferico I2C.
void I2CStart (void);              // Función que inicia la comunicación I2C.
void I2CReStart (void);            // Función que reinicia la comuncación I2C.
void I2CStop (void);               // Función que detiene la comunicación I2c.
void I2CAck (void);                // Función para transmitir Acknowledge.
void I2CNoAck (void);             // Función para transmitir NO Acknowledge.
void I2CWrite (uint8_t data);      // Función para escribir el SSPBUF.
uint8_t I2CRead (void);            // Función para leer el SSPBUF.

#endif	/* XC_I2CLIB_H */