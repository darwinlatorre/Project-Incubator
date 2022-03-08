#include "ADCLib.h"

void PWM1_PWM2_Init(unsigned char periodo){  
    //TRISCbits.RC2 =0; // RC2 como salida
    /* CCP modo PWM */
    CCP1CONbits.DC1B = 0b00; // 5,4 bits limpios (DC1B1:DC1B0 = 00)
    CCP1CONbits.CCP1M = 0b1100 ; // modo PWM ( CCP1M3:CCP1M0 = 1100)
    /* Timer2 configuración */
    /////////////////////////////////////////////////////////////////
    TRISCbits.RC1 =0; // RC1 como salida
    /* CCP modo PWM */
    CCP2CONbits.DC2B = 0b00; // 5,4 bits limpios (DC1B1:DC1B0 = 00)
    CCP2CONbits.CCP2M = 0b1100 ; // modo PWM ( CCP2M3:CCP1M0 = 1100)
    TRISCbits.RC2 =0; // RC2 como salida
    PR2 = periodo; // configure PERIODO TMR2
    T2CONbits.T2CKPS = 0x02; // Prescaler x16
    T2CONbits.TMR2ON = 1; // timer2 ON

}

void PWM1_setDC(unsigned char dutycycle){
    CCPR1L = dutycycle; // PWM duty cycle - 8-bits (MSb)
}
void PWM2_setDC(unsigned char dutycycle){
    CCPR2L = dutycycle; // PWM duty cycle - 8-bits (MSb)
}

void config_TMR0(){
    T0CONbits.T08BIT=0;        //configura a 16 bits 0-65535
    T0CONbits.T0CS=0;           //selecciona modo temporizador
    T0CONbits.T0PS=0b000;       //prescaler x2
    T0CONbits.PSA=0;           //con prescaler 
    TMR0H=0X3C;                 //dato calculado pata lograr 20ms
    TMR0L=0XB0;                 //dato calculado pata lograr 20ms
    INTCONbits.TMR0IF=0;        //bandera de interrupcion apagada
    INTCONbits.TMR0IE=1;        //habilita interrupcion TMR0
    INTCON2bits.TMR0IP=0;       //interrupcion de baja prioridad
}
void config_TMR1(){
    T1CONbits.RD16=1;     //16 bits
    T1CONbits.TMR1CS=0;       //modo temporizador
    T1CONbits.T1CKPS=0b11;   //prescaler 8
    TMR1H=0X00;             //cada 100mS seg produce interrupcion
    TMR1L=0X00;
    T1CONbits.TMR1ON=1;
    PIR1bits.TMR1IF=0;
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=0;   //interrupcion de baja prioridad
}

void inicializa_ADC(){        
    ADCON1bits.PCFG=0b1011;   //  AN0 , AN1 , AN2 y AN3
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