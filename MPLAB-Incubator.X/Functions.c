#include "FunctionsHeader.h"

void ejecutaComandoPresionado(unsigned char prmComando)
{
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
void StartOption()
{
    while(atrStart==0)
        {
        do{
            if(atrStart==1)
            {
                atrCaracter=Start;
                break;
            }
            atrCaracter=ReadKeyBoard();
        }while(atrCaracter==0);
        teclado_deje_de_pulsar();
        ejecutaComandoPresionado(atrCaracter);
    }
  ///////////////////////start=1///////////////////////////////////////
    WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
}
void DataProcess()
{
    atrVarMax=atrSetPoint1+atrHisteresis1;       //calculo de histerisis
    atrVarMin=atrSetPoint1-atrHisteresis1;
    atrHisteresisMax=atrSetPoint2+atrHisteresis2;
    atrHisteresisMin=atrSetPoint2-atrHisteresis2;
    
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
        atrCaracter = ReadKeyBoard();
    }while(atrCaracter!=Stop);              //Si la tecla pulsada es Stop
    teclado_deje_de_pulsar();
    atrStart=0;                            //sale del bucle do---while() 
}
void MostrarTemperatura()
{
    //////////////////PARA LA TEMPERATURA /////////////////
    atrVarPorceso1=(adc_read(0)*150.0)/255.0;
    if(atrVarPorceso1>99) 
        sprintf(string_cont1,"Temperatura:   %u°C",atrVarPorceso1);
    else if(atrVarPorceso1>9)
        sprintf(string_cont1,"Temperatura:   0%u°C",atrVarPorceso1);
    else 
        sprintf(string_cont1,"Temperatura:   00%u°C",atrVarPorceso1);
    if(atrStart==0)
        WriteCmdLCD( DON & CURSOR_OFF & BLINK_OFF ); //parpadeo de cursor apagado
    LCDWrite(0, 0, string_cont1);
}
void MostrarHumedad()
{
    //////////////////PARA LA HUMEDAD //////////////////////////
    atrVarPorceso2=(adc_read(3)*100.0)/255.0;
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
}
void ConfigServoMotor()
{
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
void msDelay(unsigned int val)
{
    unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);     //This count Provide delay of 1 ms for 8MHz Frequency
}