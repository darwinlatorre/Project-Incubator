#include "RTCLib.h"

uint8_t BCD_a_Decimal (uint8_t numero)            // Función que convierte un número BCD a decimal.
{
  return ((numero >> 4) * 10 + (numero & 0x0F));  // Retornamos la variable "numero" desplazado 4 posiciones a la izquierda, multiplicado por 10 más "numero" &&  1111.
}

uint8_t Decimal_a_BCD (uint8_t numero)            // Función que convierte un número decimal a BCD. 
{
    return (((numero / 10) << 4) + (numero % 10));// Retornamos la decena de la variable "numero" desplazado 4 bits a la derecha y las unidades de la variable "numero" en el nibble menos significativo
}

void readTime()
{
    I2CStart();             // Llamamos a la función Start.
    I2CWrite(0xD0);         // Escribimos en SSPBUF la dirección de DS1307 1101000 + 0 de escritura.
    I2CWrite(0);            // Dirección de segundos.
    I2CReStart();           // Llamamos a la función ReStart.
    I2CWrite(0xD1);         // Escribimos en SSPBUF la dirección de DS1307 1101000 +1 de lectura.
    seconds=I2CRead();      // Cargamos la variable "seconds" con el valor de SSPBUF.
    I2CAck();               // ACK.
    minutes=I2CRead();      // Cargamos la variable "minutes" con el valor de SSPBUF.
    I2CAck();               // ACK.
    hours=I2CRead();        // Cargamos la variable "hours" con el valor de SSPBUF.
    I2CAck();               // ACK.
    I2CRead();              // Valor no leido.
    I2CAck();               // ACK.
    day=I2CRead();          // Cargamos la variable "day" con el valor de SSPBUF.
    I2CAck();               // ACK.
    month=I2CRead();        // Cargamos la variable "month" con el valor de SSPBUF.
    I2CAck();               // ACK.
    year=I2CRead();         // Cargamos la variable "year" con el valor de SSPBUF.
    I2CNoAck();             // NO ACK.
    I2CStop();              // Llamamos a la función Stop.
        
    seconds = BCD_a_Decimal(seconds);       // Llamamos a la función de conversión de BCD a Decimal.
    minutes = BCD_a_Decimal(minutes);       // Llamamos a la función de conversión de BCD a Decimal.
    hours   = BCD_a_Decimal(hours);         // Llamamos a la función de conversión de BCD a Decimal.
    day     = BCD_a_Decimal(day);           // Llamamos a la función de conversión de BCD a Decimal.
    month   = BCD_a_Decimal(month);         // Llamamos a la función de conversión de BCD a Decimal.
    year    = BCD_a_Decimal(year);          // Llamamos a la función de conversión de BCD a Decimal.
    
    Time[6] =  hours   / 10 + '0';          // Guardamos las decenas de la variable "hours" en la posición 6 del arreglo "Time".
    Time[7] =  hours   % 10 + '0';          // Guardamos las unidades de la variable "hours" en la posición 7 del arreglo "Time".
    Time[9] =  minutes / 10 + '0';          // Guardamos las decenas de la variable "minutes" en la posición 9 del arreglo "Time".
    Time[10]=  minutes % 10 + '0';          // Guardamos las unidades de la variable "minutes" en la posición 10 del arreglo "Time".
    Time[12]=  seconds / 10 + '0';          // Guardamos las decenas de la variable "seconds" en la posición 12 del arreglo "Time".
    Time[13]=  seconds % 10 + '0';          // Guardamos las unidades de la variable "secods" en la posición 13 del arreglo "Time".
    
    Date[6] =  day     / 10 + '0';          // Guardamos las decenas de la variable "day" en la posición 6 del arreglo "Date".
    Date[7] =  day     % 10 + '0';          // Guardamos las unidades de la variable "day" en la posición 7 del arreglo "Date".
    Date[9] =  month   / 10 + '0';          // Guardamos las decenas de la variable "month" en la posición 9 del arreglo "Date".
    Date[10]=  month   % 10 + '0';          // Guardamos las unidades de la variable "month" en la posición 10 del arreglo "Date".
    Date[14]=  year    / 10 + '0';          // Guardamos las decenas de la variable "year" en la posición 14 del arreglo "Date".
    Date[15]=  year    % 10 + '0';          // Guardamos las unidades de la variable "year" en la posición 15 del arreglo "Date".
}