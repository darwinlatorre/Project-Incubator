

#include <xc.h>
#include "ADC_libreria.h"
#define _XTAL_FREQ 20000000

void inicializa_ADC(){        
    ADCON1bits.PCFG=0b1100;   //  AN0 , AN1  Y AN2
    ADCON1bits.VCFG=0;        //VREF  5v y GND
    ADCON2bits.ADFM=0;        // justificacion a la derecha
    ADCON2bits.ACQT=3;       // 6TAD de tiempo de adquisicion para 2.45uS   6*TAD= 4.8 uS > 2.45uS
    ADCON2bits.ADCS=5;       // FOSC/16 ó 16*TOSC    ES 0.8 uS   es > 0.7 uS < TAD < 3.5us
    ADCON0bits.GO_DONE=0;     //ADC en reposo
    ADCON0bits.ADON=1;
}
unsigned char adc_read(char chx){ ///se  envia el canal a leer y retorna un int=16bits
    ADCON0= ((ADCON0&0b11000011) |chx<<2);
    ADCON0bits.GO=1;
    while(ADCON0bits.GO==1);
    return ADRESH;
}