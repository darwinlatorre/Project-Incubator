#ifndef __XLCD_H
#define __XLCD_H

#include "FunctionsHeader.h"

/* DATA_PORT defines the port to which the LCD data lines are connected */
#define DATA_PORT  PORTD
#define TRIS_DATA_PORT  TRISD

/* CTRL_PORT defines the port where the control lines are connected.*/
#define RW_PIN   LATDbits.LATD4   /* PORT for RW */
#define TRIS_RW  TRISDbits.TRISD4    /* TRIS for RW */
#define RS_PIN   LATDbits.LATD5   /* PORT for RS */
#define TRIS_RS  TRISDbits.TRISD5    /* TRIS for RS */
#define E_PIN    LATDbits.LATD6   /* PORT for E  */
#define TRIS_E   TRISDbits.TRISD6    /* TRIS for E  */

/* Display ON/OFF Control defines */
#define DON         0b00001111  /* Display on      */
#define DOFF        0b00001011  /* Display off     */
#define CURSOR_ON   0b00001111  /* Cursor on       */
#define CURSOR_OFF  0b00001101  /* Cursor off      */
#define BLINK_ON    0b00001111  /* Cursor Blink    */
#define BLINK_OFF   0b00001110  /* Cursor No Blink */

/* -----------------Some Command Type defines----------------------	*/
#define CLEAR_XLCD		0x01		/* Clears the LCD*/
#define RETURN_CURSOR_HOME	0x02		/*Returns the cursor to the HOME position*/

/* -----------------Cursor or Display Shift defines -----------------*/
#define SHIFT_CUR_LEFT    0b00010011  /* Cursor shifts to the left   */
#define SHIFT_CUR_RIGHT   0b00010111  /* Cursor shifts to the right  */
#define SHIFT_DISP_LEFT   0b00011011  /* Display shifts to the left  */
#define SHIFT_DISP_RIGHT  0b00011111  /* Display shifts to the right */

// ----------------- Function Set defines------------------------- 
#define FOUR_BIT   0b00101111  /* 4-bit Interface               */
#define EIGHT_BIT  0b00111111  /* 8-bit Interface               */
#define LINE_5X7   0b00110011  /* 5x7 characters, single line   */
#define LINE_5X10  0b00110111  /* 5x10 characters               */
#define LINES_5X7  0b00111011  /* 5x7 characters, multiple line */
/* LINES*/
#define DDRAM_LINE1   0b10000000  /* 4-bit Interface               */
#define DDRAM_LINE2   0b11000000  /* 4-bit Interface               */
#define DDRAM_LINE3   0b10010100  /* 4-bit Interface               */
#define DDRAM_LINE4   0b11010100  /* 4-bit Interface               */
#define CGRAM_ADDRESS 0b01000000  /* 4-bit Interface               */

byte atrPosicionX = 3;                          //Indicará la posicion del cursor que parpadea
byte atrPosicionY = 1;                          //Indicará la posicion del cursor que parpadea
byte atrSetPoint1 = 50, atrHisteresis1 = 5, atrSetPoint2 = 50, atrHisteresis2 = 5;   //variables(setpoint/histéresis/Variable de proceso)
unsigned char string_cont1[40];

// -----------OpenXLCD --> Configura pines de I/O para el LCD
void LCDStart(unsigned char prmLCDtype);

// -----------SetDDRamAddr --> Establece la dirección de visualización de datos
void SetDDRamAddr(unsigned char prmDDaddr);

// -----------BusyXLCD --> Retorna el estatus de ocupado del LCD
unsigned char BusyLCD(void);

// -----------ReadDataXLCD --> Lee un byte de dato
char ReadDataLCD(void);

// --------------WriteCmdXLCD --> Escribe un comando a la pantalla LCD
void WriteCmdLCD(unsigned char prmCMD);

// --------------WriteDataXLCD -->  Escribe un byte de datos en la pantalla LCD
void WriteDataLCD(char prmData);

// --------------putcXLCD  -->   Un putc es una escritura
#define putcXLCD WriteDataLCD
#define XLCD_clear WriteCmdLCD(CLEAR_XLCD);

// --------------putrsXLCD --> Escribe una cadena de caracteres en la ROM del LCD
void putrsLCD(const char *prmBuffer);

extern void Delay1us(void);   // 1us delay
extern void DelayPORLCD(void);    // 15ms delay
extern void DelayLCD(void); // 4.1ms
extern void DelayExecution(void);

//--------------------Funciones para el LCD utilizando las librerias×
void LCDgotoXY(int prmFila,int prmColumna);
void LCDInit(void);
void LCDWrite(int prmFila,int prmColumna, const char *prmBuffer);
#endif
