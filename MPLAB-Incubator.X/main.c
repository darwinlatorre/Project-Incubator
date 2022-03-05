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

byte Px=3;                          //Indicará la posicion del cursor que parpadea
byte Py=1;                          //Indicará la posicion del cursor que parpadea
char Caracter;                      //Almacenará el caracter leido del teclado matricial
byte  Sp1,Hi1,Vp1,Vp2,Sp2,Hi2;                      //variables(setpoint/histéresis/Variable de proceso)
byte start,Vmax,Vmin,intV,intL,Hmin,Hmax;       //variables(Valor maximo/Valor minimo)=Setpoint +- Histéresis 
byte intLampara,intVentilador;
byte arreglo[12];
byte punArr=0;

// variable start indicara con un numero diferente a cero que inicia el programa
/////////////////////////////////////////////////////////////////////////////////////

void main(void) {
    inicializa_ADC();
    TRISAbits.RA4=0;
    TRISAbits.RA5=0;
    TRISCbits.RC0=0;
    Pin_ServoMotor=0;
    IndicadorHmax=0;
    IndicadorHmin=0;
    LCDInit();
    teclado_inicializa();
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
    Sp1=50;
    Hi1=5;
    Sp2=50;
    Hi2=5;
    start=0;
    /////////////////////PROGRAMA PRINCIPAL////////////////////////////////////
    while(1){ 
        IndicadorHmin=0;
        IndicadorHmax=0;
        PWM1_setDC(0);                  //OFF
        PWM2_setDC(0);                  //OFF
        LCDgotoXY(Px,2);
        WriteCmdLCD( DON & CURSOR_OFF & BLINK_ON ); //Cursor parpadea
        while(start==0)
        {
            do{
                if(start==1)
                {
                    Caracter=Start;
                    break;
                }
                Caracter=teclado_lee();
            }
        while(Caracter==0);
        teclado_deje_de_pulsar();
        ejecutaComandoPresionado(Caracter);
    }
  ///////////////////////start=1///////////////////////////////////////
    WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
    Vmax=Sp1+Hi1;                                    //calculo de histerisis
    Vmin=Sp1-Hi1;
    Hmax=Sp2+Hi2;
    Hmin=Sp2-Hi2;
    do{
        if(Vp1>Vmax){                    //Muy caliente
            PWM1_setDC(0);                //Lampara OFF   RC2  
            intV=adc_read(2);
            PWM2_setDC(intV);     //Ventilador ON  RC1
        }
        if(Vp1<Vmin){        //Muy frio
            PWM2_setDC(0);                 //ventilador OFF  RC1  
            intL=adc_read(1);
            PWM1_setDC(intL);        //lampara ON  RC2                        
        }
        if(Vp2>Hmax){
            IndicadorHmax=1;
            IndicadorHmin=0; } 
        else if(Vp2<Hmin){
            IndicadorHmax=0;
            IndicadorHmin=1; } 
        else {
            IndicadorHmax=0;
            IndicadorHmin=0;    
        }
        if(start==0)
            break;              //sale del bucle do---while()   
    Caracter=teclado_lee();
    }
    while(Caracter!=Stop);              //Si la tecla pulsada es Stop
        teclado_deje_de_pulsar();
        start=0;                            //sale del bucle do---while() 
    }
}
/////////////////////FIN DE PROGRAMA PRINCIPAL////////////////////////////////////
void ejecutaComandoPresionado(unsigned char comando){
    char string_cont[6];                //Arreglo que comvierte un dato a cadena
    switch (comando){
        case Abajo :
            Py=3;
            LCDgotoXY(Px,Py);
        break;
        case Arriba:
            Py=1;
            LCDgotoXY(Px,Py);
        break;
        case Izq: Px--;
            if(Px<3)Px=3;
            else if(Px<13&&Px>5)Px=5;
                LCDgotoXY(Px,Py);
            break;
            case Der:  Px++;
                if(Px>5&&Px<13)Px=13;
                else if(Px>14)Px=14;
                LCDgotoXY(Px,Py);
            break;
            case Start:
                start=1;
            break;
            case Stop:
            break;
        default:
            LCDgotoXY(Px,Py);
            WriteDataLCD(comando) ;
            if(Py==1)
            {
                if(Px<6)
                {
                /////////////Setpoint/////////////////
                    LCDgotoXY(3,1); 
                    Sp1=(ReadDataLCD()-'0');
                    if(Sp1>1) Sp1=1;
                        Sp1=Sp1*100;
                        Sp1=Sp1+((ReadDataLCD()-'0')*10);
                        Sp1=Sp1+(ReadDataLCD()-'0');
                    if(Sp1>150)Sp1=150;
                    if(Sp1>99) sprintf(string_cont,"%u°C",Sp1);
                    else if(Sp1>9)sprintf(string_cont,"0%u°C",Sp1);
                    else sprintf(string_cont,"00%u°C",Sp1);
                        LCDWrite(3,1, string_cont);
                }
            /////////////Histeresis/////////////////
                if (Px>12){
                    LCDgotoXY(13,1);
                    Hi1=(ReadDataLCD()-'0')*10;
                    Hi1=Hi1+(ReadDataLCD()-'0');
                if(Hi1>=Sp1)Hi1=10;
                if(Hi1>9)sprintf(string_cont,"%u",Hi1);
                else sprintf(string_cont,"0%u",Hi1);
                    LCDWrite(13,1, string_cont);
                }
            }
            if(Py==3){
                if(Px<6){
                    /////////////Setpoint/////////////////
                    LCDgotoXY(3,3); 
                    Sp2=(ReadDataLCD()-'0');
                    if(Sp2>1) Sp2=1;
                        Sp2=Sp2*100;
                        Sp2=Sp2+((ReadDataLCD()-'0')*10);
                        Sp2=Sp2+(ReadDataLCD()-'0');
                    if(Sp2>100)Sp2=100;
                    if(Sp2>99) sprintf(string_cont,"%u",Sp2);
                    else if(Sp2>9)sprintf(string_cont,"0%u",Sp2);
                    else sprintf(string_cont,"00%u",Sp2);
                    LCDWrite(3, 3, string_cont);
                    putrsLCD("%  ");
                }
            /////////////Histeresis/////////////////
                if (Px>12){
                    LCDgotoXY(13,3);
                    Hi2=(ReadDataLCD()-'0')*10;
                    Hi2=Hi2+(ReadDataLCD()-'0');
                if(Hi2>=Sp2)Hi1=10;
                if(Hi2>9)sprintf(string_cont,"%u",Hi2);
                else sprintf(string_cont,"0%u",Hi2);
                LCDWrite(13,3, string_cont);
                }
            }
        LCDgotoXY(Px,Py);       
    }   
}

__interrupt(low_priority)void ISR_TMR0_TMR1(){
    if(PIR1bits.TMR1IF==1){
        unsigned char string_cont1[40];
        Vp1=(adc_read(0)*150.0)/255.0;
        Vp2=(adc_read(3)*100.0)/255.0;
   
        //////////////////PARA LA TEMPERATURA /////////////////
        if(Vp1>99) 
            sprintf(string_cont1,"Temperatura:   %u°C",Vp1);
        else if(Vp1>9)
            sprintf(string_cont1,"Temperatura:   0%u°C",Vp1);
        else 
            sprintf(string_cont1,"Temperatura:   00%u°C",Vp1);
        if(start==0)
            WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
        LCDWrite(0, 0, string_cont1);
    
        //////////////////PARA LA HUMEDAD //////////////////////////
        if(Vp2>99) 
            sprintf(string_cont1,"Humedad:        %u",Vp2);
        else if(Vp2>9)
            sprintf(string_cont1,"Humedad:        0%u",Vp2);
        else 
            sprintf(string_cont1,"Humedad:        00%u",Vp2);
        LCDWrite(0, 2, string_cont1);
        WriteDataLCD('%');
        if(start==0)
            WriteCmdLCD( DON & CURSOR_OFF & BLINK_ON ); //parpadeo de cursor apagado
        LCDgotoXY(Px,Py);
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
