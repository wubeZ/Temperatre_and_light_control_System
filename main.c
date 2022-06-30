#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include "kpd.h"
#include "adc.h"
#include "lcd.h"

#define HEATER PORTAbits.RA1
#define FAN PORTAbits.RA2
#define LAMP PORTDbits.RD7

void keypad_press();
void Temp_checker();
void LDR_checker();

short int Temp_Ref;
volatile unsigned char data[10], data2[10];
volatile short int digital, digital1;
volatile short int voltage;

volatile char curr_key, k_scan;

void __interrupt() keypad(){

    if(INTCONbits.INT0IF == 1){
        curr_key = key_get(0);
        while(PORTBbits.RB0);
        INTCONbits.INT0IF = 0;
    }
    else if(INTCON3bits.INT1IF == 1){
        curr_key = key_get(1);
        while(PORTBbits.RB1);
        INTCON3bits.INT1IF = 0;
    }
    else if (INTCON3bits.INT2IF == 1){
        curr_key = key_get(2);
        while(PORTBbits.RB2);
        INTCON3bits.INT2IF = 0;
    }
    // character processing
    if ( curr_key == '#' ){
        sprintf(data,"%d",Temp_Ref);
        lcd_cmd(0x01);               //Clear display
        lcd_cmd(0x80);             // Move to row=1,column=1
        lcd_string("Temp Ref: ");
        lcd_string(data);      //Display the temp ref
        lcd_chr(223);           // Different LCD displays have different char code for degree
        lcd_string(" C"); /*Concatenate result and unit to print*/
     }
    else if ( curr_key != '*' ){
          lcd_chr(curr_key);
          Temp_Ref = 10*Temp_Ref + (Seven_Segment_MAP[curr_key - '0']);
       }       
}

void LDR_checker(){
    digital1 = ADC_Read(3);
        if (digital1 > 511){
            LAMP = 1; }
        else{
            LAMP = 0; }
    __delay_ms(1);
}

void Temp_checker(){
        digital = ADC_Read(0); //adc for channel 0 for temperature sensor;
        voltage = (digital*500)/1024; //convert data into temperature (LM35 produces 10mV per degree celcius)      
        sprintf(data2, "%d",voltage ); // Convert voltage to string
        lcd_cmd(0xC0);                   // Move to row=2,column=1
        lcd_string("Temp is: ");
        lcd_string(data2);      //Display the temp ref
        lcd_chr(223);// Different LCD displays have different char code for degree
        lcd_chr('C'); 

        if ((Temp_Ref - 1) >  voltage)  //If Temp Ref is greater than actual Temp, Switch ON Heater
        {  HEATER = 1,
           FAN = 0; }
        else if ((Temp_Ref + 1) <  voltage)  //If Temp Ref is less than actual Temp, Switch ON Fan
        { HEATER = 0,
          FAN = 1; }
        else  //If Temp Ref is equal to actual Temp, Switch OFF Fan and Heater
        { HEATER = 0,
          FAN = 0; } 
}

void keypad_press(){
    k_scan = 0x08;
    
    while (1){
       if(curr_key == '#')break;
        k_scan = k_scan << 1;
        if(k_scan >= 0x80){
            k_scan = 0x08;
        }
        LATB = k_scan;   
        LDR_checker();
    }    
    INTCON2 = 0xC0;
    INTCON3 = 0x00;
}

void main(void) { 
    ANSELC = 0X00;
    ANSELB = 0x00;
    ANSELA = 0x09;
    ANSELD = 0x00;
    TRISA = 0x09;
    TRISC = 0X00;
    TRISB = 0x07;
    TRISD = 0x00;
    
    LATB = 0x00;
    LATC =0x00;
       
    INTCON = 0xD0;
    INTCON2 = 0xC0;
    
    LATBbits.LATB6 = 1;

    __delay_ms(10);
   
    lcd_init();
    ADC_Initialize(); /*Initialize 10-bit ADC*/
    
    Temp_Ref = 25; 
    lcd_cmd(0x81);
    lcd_string("TEMP & LIGHT");
    __delay_ms(5);
    lcd_cmd(0xC1);
    lcd_stirng("CONTROL SYSTEM");
    __delay_ms(10);
    
    lcd_cmd(0x01);                     //Clear display
    lcd_cmd(0x80);                   // Move to row=1,column=1
    lcd_string("Temp Ref: ");
    lcd_string("25");      //Display the temp ref
    lcd_chr(223);           // Different LCD displays have different char code for degree
    lcd_string(" C"); /*Concatenate result and unit to print*/

    HEATER = 0;                 //Switch OFF Heater on start up
    FAN = 0;                    //Switch OFF Fan on start up
    LAMP = 0;                   //Switch OFF LAMP on start up  
   
    while(1){
        if ( curr_key == '*' )  //If Clear '*' pressed
    {
       lcd_cmd(0x01);                     //Clear display
       lcd_cmd(0x80);                   // Move to row=1,column=1
       lcd_string("Enter Temp Ref");
       Temp_Ref = 0;
       lcd_cmd(0xC0);                   // Move to row=2,column=1
       lcd_string("Temp Ref: ");
       
        INTCON2 = 0xF0; // used to set interrupt edge to rising edge
        INTCON3 = 0xD8; // Used to enable int1IE and int2IE
       keypad_press();
    }
        
 	Temp_checker();  
 	     
        __delay_ms(5);  
        
        LDR_checker();    
}
}
