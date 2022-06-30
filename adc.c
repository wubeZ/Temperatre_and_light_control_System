#include <stdbool.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#define _XTAL_FREQ 8000000                 //Useful for __delay_ms() function 

void ADC_Initialize(void){  // set the ADC to the options selected in the User Interface
    // TRIGSEL CCP5; NVCFG VSS; PVCFG VDD; 
    ADCON1 = 0x00;
    // ADFM right; ACQT 2; ADCS FOSC/2; 
    ADCON2 = 0x88;
    // ADRESL 0; 
    ADRESL = 0x00;
    // ADRESH 0; 
    ADRESH = 0x00;
    // GO_nDONE stop; ADON enabled; CHS AN0; 
    ADCON0 = 0x01;
}
int ADC_Read(int channel)
{
    int digital;
    ADCON0 =(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100);

    /*channel 0 is selected i.e.(CHS3CHS2CHS1CHS0=0000)& ADC is disabled*/
    ADCON0 |= ((1<<ADON)|(1<<GO));/*Enable ADC and start conversion*/

    /*wait for End of conversion i.e. Go/done'=0 conversion completed*/
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
    
    return(digital);
}

void Delay_Seconds(unsigned char z)
{
    unsigned char x,y;
    for(y = 0; y < z; y++)
    {
        for(x = 0; x <  10; x++)__delay_ms(10);
    }
}