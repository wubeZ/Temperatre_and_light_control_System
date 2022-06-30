#include "lcd.h"
#include <xc.h>

#define lcd PORTC
#define rs PORTDbits.RD0
#define en PORTDbits.RD1

void lcd_init(){
    lcd_cmd(0x38);              //8-bit mode, 2 lines, 5x7 dots matrix
    lcd_cmd(0x0E);              //Cursor Blinking
    lcd_cmd(0x01);              //Clear LCD
    lcd_cmd(0x06);              //Increment Cursor
    lcd_cmd(0x80);              //Force cursor to beginning ( 1st line)

}


//void lcd_lat(){
//    en = 1;
//    delay(1);
//    en = 0;
//}

void lcd_cmd(unsigned char cmd){
    lcd = cmd;  /*Send higher nibble of command first to PORT*/ 
	rs = 0;  /*Command Register is selected i.e.RS=0*/ 
    en = 1;
    delay(1);
    en = 0;
	delay(1);
    
    }

void lcd_chr(unsigned char chrctr){
    lcd = chrctr;  /*Send higher nibble of command first to PORT*/ 
	rs = 1;  /*Command Register is selected i.e.RS=0*/ 
    en = 1;
    delay(1);
    en = 0;
	delay(3);
    }

void lcd_string(unsigned char *str){
    while(*str != '\0'){   // if the pointer is not null, same to array
        lcd_chr(*str++);
    }
}

void delay(unsigned char ms){
    unsigned int i,j;
    for(i=0;i<=120;i++)
        for(j=0;j<=ms;j++);
}