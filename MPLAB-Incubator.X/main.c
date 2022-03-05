#include <xc.h>
#include "BITS_Configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "KeyBoardLib.h"
#include "LCD_20x4Lib.h"
#include "ADCLib.h"

#define byte uint8_t
#define P_90grados      PORTEbits.RE1
#define P_180grados     PORTEbits.RE0
#define Pin_ServoMotor  LATCbits.LATC0
#define IndicadorHmax   LATAbits.LA4
#define IndicadorHmin   LATAbits.LA5
#define LAMPARA         LATCbits.LATC2
#define VENTILADOR      LATCbits.LATC1

byte atrPosicionX = 3;                          //Indicará la posicion del cursor que parpadea
byte atrPosicionY = 1;                          //Indicará la posicion del cursor que parpadea
char atrCaracter;                      //Almacenará el caracter leido del teclado matricial
byte atrSetPoint1 = 50, atrHisteresis1 = 5, atrVarPorceso1, atrVarPorceso2, atrSetPoint2 = 50, atrHisteresis2 = 5;   //variables(setpoint/histéresis/Variable de proceso)
byte atrStart, atrVarMax, atrVarMin, intV, intL, atrHisteresisMin, atrHisteresisMax;       //variables(Valor maximo/Valor minimo)=Setpoint +- Histéresis 
byte atrIntLampara, atrVentilador;
byte atrArreglo[12];
byte atrPunArr=0;

// variable start indicara con un numero diferente a cero que inicia el programa
/////////////////////////////////////////////////////////////////////////////////////

void pclIniciarPrograma();
void inicializarProcesos();
void procesamientoDatos();

void main(void) {
    TRISAbits.RA4=0;
    TRISAbits.RA5=0;
    TRISCbits.RC0=0;
    
    inicializarProcesos();
    
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
        
        pclIniciarPrograma();
        
  ///////////////////////start=1///////////////////////////////////////
        WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
        
        atrVarMax=atrSetPoint1+atrHisteresis1;       //calculo de histerisis
        atrVarMin=atrSetPoint1-atrHisteresis1;
        atrHisteresisMax=atrSetPoint2+atrHisteresis2;
        atrHisteresisMin=atrSetPoint2-atrHisteresis2;
        
        procesamientoDatos();
        
        teclado_deje_de_pulsar();
        atrStart=0;                            //sale del bucle do---while() 
    }
}

void procesamientoDatos(){
    do{
        if(atrVarPorceso1>atrVarMax){                    //Muy caliente
            PWM1_setDC(0);                //Lampara OFF   RC2  
            intV=adc_read(2);
            PWM2_setDC(intV);     //Ventilador ON  RC1
        }
        if(atrVarPorceso1<atrVarMin){        //Muy frio
            PWM2_setDC(0);                 //ventilador OFF  RC1  
            intL=adc_read(1);
            PWM1_setDC(intL);        //lampara ON  RC2                        
        }
        if(atrVarPorceso2>atrHisteresisMax){
            IndicadorHmax=1;
            IndicadorHmin=0; } 
        else if(atrVarPorceso2<atrHisteresisMin){
            IndicadorHmax=0;
            IndicadorHmin=1; } 
        else{
            IndicadorHmax=0;
            IndicadorHmin=0;    
        }
        if(atrStart==0)
            break;              //sale del bucle do---while()   
    atrCaracter = teclado_lee();
    }while(atrCaracter!=Stop);              //Si la tecla pulsada es Stop
}

void pclIniciarPrograma(){
    while(atrStart==0)
    {
        do{
            if(atrStart==1)
            {
                atrCaracter=Start;
                break;
            }
            atrCaracter=teclado_lee();
        }while(atrCaracter==0);
        teclado_deje_de_pulsar();
        ejecutaComandoPresionado(atrCaracter);
    }
}

void inicializarProcesos(){
    inicializa_ADC();
    LCDInit();
    teclado_inicializa();
    config_TMR1();
    config_TMR0();
    PWM1_PWM2_Init(255);                //Configura los 2 PWM periodo = 819.2us
    LCDWrite(0, 1, "SP=050°C");
    LCDWrite(10, 1, "Hi=05");
    LCDWrite(0, 3, "SP=050%");
    LCDWrite(10, 3, "Hi=05");
}

/////////////////////FIN DE PROGRAMA PRINCIPAL////////////////////////////////////
void ejecutaComandoPresionado(unsigned char prmComando){
    char string_cont[6];                //Arreglo que comvierte un dato a cadena
    switch (prmComando){
        case Abajo :
            atrPosicionY=3;
            LCDgotoXY(atrPosicionX,atrPosicionY);
        break;
        case Arriba:
            atrPosicionY=1;
            LCDgotoXY(atrPosicionX,atrPosicionY);
        break;
        case Izq: atrPosicionX--;
            if(atrPosicionX<3)atrPosicionX=3;
            else if(atrPosicionX<13&&atrPosicionX>5)atrPosicionX=5;
                LCDgotoXY(atrPosicionX,atrPosicionY);
            break;
            case Der:  atrPosicionX++;
                if(atrPosicionX>5&&atrPosicionX<13)atrPosicionX=13;
                else if(atrPosicionX>14)atrPosicionX=14;
                LCDgotoXY(atrPosicionX,atrPosicionY);
            break;
            case Start:
                atrStart=1;
            break;
            case Stop:
            break;
        default:
            LCDgotoXY(atrPosicionX,atrPosicionY);
            WriteDataLCD(prmComando) ;
            if(atrPosicionY==1)
            {
                if(atrPosicionX<6)
                {
                /////////////Setpoint/////////////////
                    LCDgotoXY(3,1); 
                    atrSetPoint1=(ReadDataLCD()-'0');
                    if(atrSetPoint1>1) atrSetPoint1=1;
                        atrSetPoint1=atrSetPoint1*100;
                        atrSetPoint1=atrSetPoint1+((ReadDataLCD()-'0')*10);
                        atrSetPoint1=atrSetPoint1+(ReadDataLCD()-'0');
                    if(atrSetPoint1>150)atrSetPoint1=150;
                    if(atrSetPoint1>99) sprintf(string_cont,"%u°C",atrSetPoint1);
                    else if(atrSetPoint1>9)sprintf(string_cont,"0%u°C",atrSetPoint1);
                    else sprintf(string_cont,"00%u°C",atrSetPoint1);
                        LCDWrite(3,1, string_cont);
                }
            /////////////Histeresis/////////////////
                if (atrPosicionX>12){
                    LCDgotoXY(13,1);
                    atrHisteresis1=(ReadDataLCD()-'0')*10;
                    atrHisteresis1=atrHisteresis1+(ReadDataLCD()-'0');
                if(atrHisteresis1>=atrSetPoint1)atrHisteresis1=10;
                if(atrHisteresis1>9)sprintf(string_cont,"%u",atrHisteresis1);
                else sprintf(string_cont,"0%u",atrHisteresis1);
                    LCDWrite(13,1, string_cont);
                }
            }
            if(atrPosicionY==3){
                if(atrPosicionX<6){
                    /////////////Setpoint/////////////////
                    LCDgotoXY(3,3); 
                    atrSetPoint2=(ReadDataLCD()-'0');
                    if(atrSetPoint2>1) atrSetPoint2=1;
                        atrSetPoint2=atrSetPoint2*100;
                        atrSetPoint2=atrSetPoint2+((ReadDataLCD()-'0')*10);
                        atrSetPoint2=atrSetPoint2+(ReadDataLCD()-'0');
                    if(atrSetPoint2>100)atrSetPoint2=100;
                    if(atrSetPoint2>99) sprintf(string_cont,"%u",atrSetPoint2);
                    else if(atrSetPoint2>9)sprintf(string_cont,"0%u",atrSetPoint2);
                    else sprintf(string_cont,"00%u",atrSetPoint2);
                    LCDWrite(3, 3, string_cont);
                    putrsLCD("%  ");
                }
            /////////////Histeresis/////////////////
                if (atrPosicionX>12){
                    LCDgotoXY(13,3);
                    atrHisteresis2=(ReadDataLCD()-'0')*10;
                    atrHisteresis2=atrHisteresis2+(ReadDataLCD()-'0');
                if(atrHisteresis2>=atrSetPoint2)atrHisteresis1=10;
                if(atrHisteresis2>9)sprintf(string_cont,"%u",atrHisteresis2);
                else sprintf(string_cont,"0%u",atrHisteresis2);
                LCDWrite(13,3, string_cont);
                }
            }
        LCDgotoXY(atrPosicionX,atrPosicionY);       
    }   
}

__interrupt(low_priority)void ISR_TMR0_TMR1(){
    if(PIR1bits.TMR1IF==1){
        unsigned char string_cont1[40];
        atrVarPorceso1=(adc_read(0)*150.0)/255.0;
        atrVarPorceso2=(adc_read(3)*100.0)/255.0;
   
        //////////////////PARA LA TEMPERATURA /////////////////
        if(atrVarPorceso1>99) 
            sprintf(string_cont1,"Temperatura:   %u°C",atrVarPorceso1);
        else if(atrVarPorceso1>9)
            sprintf(string_cont1,"Temperatura:   0%u°C",atrVarPorceso1);
        else 
            sprintf(string_cont1,"Temperatura:   00%u°C",atrVarPorceso1);
        if(atrStart==0)
            WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
        LCDWrite(0, 0, string_cont1);
    
        //////////////////PARA LA HUMEDAD //////////////////////////
        if(atrVarPorceso2>99) 
            sprintf(string_cont1,"Humedad:        %u",atrVarPorceso2);
        else if(atrVarPorceso2>9)
            sprintf(string_cont1,"Humedad:        0%u",atrVarPorceso2);
        else 
            sprintf(string_cont1,"Humedad:        00%u",atrVarPorceso2);
        LCDWrite(0, 2, string_cont1);
        WriteDataLCD('%');
        if(atrStart==0)
            WriteCmdLCD( DON & CURSOR_OFF & BLINK_ON ); //parpadeo de cursor apagado
        LCDgotoXY(atrPosicionX,atrPosicionY);
        intV=adc_read(2);
        intL=adc_read(1);
        
        PIR1bits.TMR1IF=0;
        }
        if(INTCONbits.TMR0IF==1){ 
            TMR0H=0X3C;                 //dato calculado pata lograr 20ms
            TMR0L=0XB0;                 //dato calculado pata lograr 20ms
            Pin_ServoMotor=1;
            if(P_90grados==1&&P_180grados==0){
                __delay_us(1500);
            }
            else if(P_180grados==1&&P_90grados==0){
                __delay_us(2000);
            }
            else __delay_us(1000);
            Pin_ServoMotor=0;
            INTCONbits.TMR0IF=0;
        }
}
////////////////FIN DE ZONA DE INTERRUPCIONES ///////////////////////////////////////
