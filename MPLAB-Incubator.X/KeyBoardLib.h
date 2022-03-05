 
#ifndef teclado_H
#define	teclado_H
#define	_XTAL_FREQ 20000000
#include <xc.h>
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

#define teclado_

void teclado_inicializa();
char teclado_lee();
void teclado_deje_de_pulsar();
char leclado_orden_tecla();

#endif	/* XC_HEADER_TEMPLATE_H */

