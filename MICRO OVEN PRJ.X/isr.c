#include <xc.h>
#include "main.h"

unsigned int half_sec;
void __interrupt() isr(void)
{
    static unsigned int count = 0;

    if (TMR2IF == 1)
    {
        if (++count == 625)     //count for 1 sec is 1250
        {
            half_sec++;
            count = 0;
        }

        TMR2IF = 0;
    }
    
    
    /*static unsigned int count1 = 0;
    
    if (TMR0IF == 1)
    {
        // TMR0 Register valur + 6 (offset count to get 250 ticks) + 2 Inst Cycle 
        TMR0 = TMR0 + 8;
        
        if (++count1 == 20000)
        {
            count1 = 0;
            
            //time = !time;
            //time--;
        }
        
        TMR0IF = 0;
    }*/
}