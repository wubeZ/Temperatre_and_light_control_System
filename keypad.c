#include <xc.h>
#define _XTAL_FREQ 8000000                 //Useful for __delay_ms() function 
#include "kpd.h"

#define c1 PORTBbits.RB0
#define c2 PORTBbits.RB1
#define c3 PORTBbits.RB2
#define r1 LATBbits.LATB3
#define r2 LATBbits.LATB4
#define r3 LATBbits.LATB5
#define r4 LATBbits.LATB6

char key_get(char col){
    char key = col;
    if(r1)
        return kpd_matrix[key];
    key += 3;
    
    if(r2)
        return kpd_matrix[key];
    key += 3;
    
    if(r3)
        return kpd_matrix[key];
    key += 3;
    if(r4)
        return kpd_matrix[key];
    return 0;
}
