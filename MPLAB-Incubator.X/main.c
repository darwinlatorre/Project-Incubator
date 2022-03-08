#include "BITS_Configuration.h"
#include "FunctionsHeader.h"
#include "RTCLib.h"

void main(void) {
    TRISAbits.RA4=0;
    TRISAbits.RA5=0;
    TRISCbits.RC0=0;
    
    inicializa_ADC();
    
    //I2CInit(100000);
    
    LCDInit();
    
    InitKeyBoard();
    
    config_TMR1();
    config_TMR0();
    PWM1_PWM2_Init(255);                //Configura los 2 PWM periodo = 819.2us
    
    LCDWrite(0, 1, "SP=050°C");
    LCDWrite(10, 1, "Hi=05");
    LCDWrite(0, 3, "SP=050%");
    LCDWrite(10, 3, "Hi=05");
    
    RCONbits.IPEN=1;                    //Interupcion de prioridades habilitado
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    
    /////////////////////PROGRAMA PRINCIPAL////////////////////////////////////
    while(1){
        IndicadorHmin=0;
        IndicadorHmax=0;
        
        PWM1_setDC(0);                  //OFF
        PWM2_setDC(0);                  //OFF
        
        LCDgotoXY(atrPosicionX,2);
        WriteCmdLCD( DON & CURSOR_OFF & BLINK_ON ); //Cursor parpadea
        
        StartOption();
        
        DataProcess();

        //readTime();
        
        msDelay(50);
    }
}
/////////////////////FIN DE PROGRAMA PRINCIPAL////////////////////////////////////

__interrupt(low_priority)void ISR_TMR0_TMR1(){
    if(PIR1bits.TMR1IF==1){
        
        MostrarTemperatura();
        MostrarHumedad();
        
        intV=adc_read(2);
        intL=adc_read(1);
        
        PIR1bits.TMR1IF=0;
    }
    if(INTCONbits.TMR0IF==1){ 
        ConfigServoMotor();
    }
}
////////////////FIN DE ZONA DE INTERRUPCIONES ///////////////////////////////////////
