#include "KeyBoardLib.h"

char TecladoHex[16]= {'1','2','3',Start,
                      '4','5','6',Stop,
                      '7','8','9',Arriba,
                      Izq,'0',Der,Abajo };
void InitKeyBoard(){
    TRIS_teclado=0b11110000;
    
    INTCON2bits.RBPU=0;
    teclado_deje_de_pulsar();}

void teclado_deje_de_pulsar(){
    LAT_teclado=0b11110000;
    do{      
     __delay_ms(20);   
    }
    while(PORT_teclado!=0b11110000);}

char ReadKeyBoard(){
    unsigned char t_orden=0;
    LAT_teclado = 0b11111110;
    while(t_orden<15){
    if(teclado_c1==0)break;
    else t_orden++;
    if(teclado_c2==0)break;
    else t_orden++;
    if(teclado_c3==0)break;
    else t_orden++;
    if(teclado_c4==0)break;
    else t_orden++;
    LAT_teclado=(LAT_teclado<<1|0b00000001); }
    if (t_orden>15)return 0;
    else return TecladoHex[t_orden];}

