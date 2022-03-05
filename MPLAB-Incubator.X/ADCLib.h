
#ifndef librerias_H
#define	librerias_H
#define	_XTAL_FREQ 20000000
#include <xc.h> // include processor files - each processor file is guarded.
void ejecutaComandoPresionado(unsigned char comando);
void PWM1_PWM2_Init(unsigned char periodo);
void PWM1_setDC(unsigned char dutycycle);
void PWM2_setDC(unsigned char dutycycle);

void config_TMR0();                     //Funcion que configura el temporizador 0 
void config_TMR1();

void inicializa_ADC();
unsigned char adc_read(char chx);
#endif	/* XC_HEADER_TEMPLATE_H */

