/* 
 * File:   lcd.h
 * Author: dignal
 *
 * Created on July 14, 2020, 12:23 AM
 */

#ifndef LCD_H
#define	LCD_H
#include <xc.h>


#define PORT_RS          LATDbits.LD0    
#define PORT_EN          LATDbits.LD1 
#define PORT_RW          LATDbits.LD2
#define TRIS_RS          TRISDbits.RD0   
#define TRIS_EN          TRISDbits.RD1  
#define TRIS_RW          TRISDbits.RD2 
#define TRIS_DATA        TRISD               //4bits TRIS DATA PORT
#define PORT_DATA        LATD               //4bits LAT DATA PORT
#define PORT_DATO        PORTD



/****** LCD Predefined Commands ******/
#define HOME_LCD                        0b00000010
#define FOURBIT_2LINE_5X7               0b00101000  //Function set -> 101000
#define DISP_ON_CURSOR_OFF_BLINK_OFF    0b00001100  //Display ON/OFF Control -> 1100
#define INCREMENT_CURSOR_NOT_SHIFT      0b00000110  //Entry mode set -> 110

/* Display ON/OFF Control defines */
#define DON         0b00001100  /* Display on      */
#define DOFF        0b00001000  /* Display off     */
#define CURSOR_ON   0b00000010  /* Cursor on       */
#define CURSOR_OFF  0b00000000  /* Cursor off      */
#define BLINK_ON    0b00000001  /* Cursor Blink    */
#define BLINK_OFF   0b00000000  /* Cursor No Blink */

/* Function Set defines */
#define ONE_LINE   0b00000000   // LCD trabaja con una sola linea
#define TWO_LINE   0b00001000   // LCD trabaja con dos sola linea
#define FOUR_BIT   0b00100000  /* 4-bit Interface               */
#define EIGHT_BIT  0b00110000  /* 8-bit Interface               */
#define LINE_5X7   0b00000000  /* 5x7 characters, single line   */
#define LINE_5X10  0b00000100  /* 5x10 characters               */




#define LINE_0	   0b10000000
#define NEXT_LINE  0b11000000

void lcd_init();
void lcd_putc(char c);
void lcd_cmd(char data);
void lcd_clear(void) ;
void lcd_home(void);
void lcd_print(char *data);
void lcd_goto(char x, char y);
void lcd_save_char(char addr, char c[]);
void lcd_escribe(char m);

#endif	/* LCD_H */

