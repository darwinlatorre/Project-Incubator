#ifndef XC_KEYBOARD_H
#define	XC_KEYBOARD_H

#include "FunctionsHeader.h"

#define Start   'A'
#define Stop    'B'
#define Arriba   'C'
#define Abajo   'D'
#define Der     '#'
#define Izq     '*'
#define PORT_teclado      PORTB
#define TRIS_teclado      TRISB
#define LAT_teclado       LATB
#define teclado_c1        PORTBbits.RB4
#define teclado_c2        PORTBbits.RB5
#define teclado_c3        PORTBbits.RB6
#define teclado_c4        PORTBbits.RB7

char atrCaracter;                      //Almacenará el caracter leido del teclado matricial
byte atrStart, atrVarMax, atrVarMin, intV, intL, atrHisteresisMin, atrHisteresisMax;       //variables(Valor maximo/Valor minimo)=Setpoint +- Histéresis 
// variable start indicara con un numero diferente a cero que inicia el programa

void InitKeyBoard();
char ReadKeyBoard();
void teclado_deje_de_pulsar();
char leclado_orden_tecla();

#endif	/* XC_KEYBOARD_H */

