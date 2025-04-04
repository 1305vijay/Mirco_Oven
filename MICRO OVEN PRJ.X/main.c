/********************************************
 * File:   main.c
 * NAME: M VIJAY
 * DATE: 25-02-25
 * PROJECT: MICRO OVEN 
 *********************************************/
/*********************************************
 Desciption: micro oven is used to heat the food or make them cook in different modes
 such as micro mode, grill mode, convection mode, start mode.
 * and worked on timer set, timer countdown, pre heating with temp set efficiently
 * project uses 16f877A pic micro controller and mplab simulator  
 **********************************************/


#include "main.h"

#pragma config WDTE = OFF

#define _XTAL_FREQ 20000000  // Set microcontroller's oscillator frequency (20MHz)



void init_config()
{
    init_matrix_keypad();
    
    //init_timer0();
    init_timer2(); // initializing timer2
    
    init_clcd();
    
    //set fan as output and turnoff init
    FAN_DDR = 0;
    FAN = 0;
    
    //set Buzzer as output and turn off init
    BUZZER_DDR = 0;
    BUZZER = 0;
     
    PEIE = 1; // Peripheral Interrupt Enable Bit (For Timer2)
    GIE = 1; // Enable all the Global Interrupts 
    
    clcd_print("################",LINE1(0));
    clcd_print("   MICRO OVEN   ",LINE2(0));
    clcd_print("  POWERING ON   ",LINE3(0));
    clcd_print("################",LINE4(0));
    __delay_ms(2000);
    clear_display();
}

void main(void) {
    
    init_config();
    
    char msg[]="0000";  //MMSS
    unsigned char control_flag = POWER_ON_MODE;
    unsigned char key = 0;//, prev_key = 1, delay = 0, mode = 'A', field;
    //static unsigned char timer = 0, flag = 0 , count = 0;
    
    while (1)
    {
        key = read_matrix_keypad(STATE);

        //if mode is in PowerOnMode
        if(control_flag == POWER_ON_MODE)
        {
            clcd_print("1.MICRO",LINE1(0));
            clcd_print("2.GRILL",LINE2(0));
            clcd_print("3.CONVECTION",LINE3(0));
            clcd_print("4.START",LINE4(0));
            switch(key)
            {
                case 1:
                    control_flag = MICRO_MODE;
                    clear_display();
                    break;
                case 2:
                    control_flag = GRILL_MODE;
                    clear_display();
                    break;
                case 3:
                    control_flag = CONVECTION_MODE;
                    clear_display();
                    break;
                case 4:
                    control_flag = START_MODE;
                    //clear_display();
                    break;
            }
        }

        //if mode is selected
        switch(control_flag)
        {
            clear_display();
            case MICRO_MODE:
                
                if(micro_mode(key) == START_FLAG)   //if return got start flag 
                {
                    clear_display();
                    control_flag = START_OPERATION;
                    //start_mode(key);
                }
                break;
                
            case GRILL_MODE:
                
                if(grill_mode(key) == START_FLAG)
                {
                    clear_display();
                    control_flag = START_OPERATION;
                }
                break;
                
            case CONVECTION_MODE:
                if(convection_mode(key) == START_FLAG)
                {
                    clear_display();
                    control_flag = START_OPERATION;
                }
                break;
            case START_MODE:        //START MODE
                if(start_mode(key) == START_FLAG)
                {
                    clear_display();
                    control_flag = START_OPERATION;
                }
                
                break;
                
            case START_OPERATION:
                
            {
                TMR2ON = 1;
                clcd_print("TIME = ",LINE1(1));
                clcd_print("4.START/RESUME",LINE2(1));
                clcd_print("5.PAUSE",LINE3(1));
                clcd_print("6.STOP",LINE4(1));
                unsigned char res = start_operation();
                if(res == STOP)   //clr the function start mode
                {
                    clear_display();
                    control_flag = POWER_ON_MODE;
                }
                else if(res == OP_SUCCESS)
                {
                    clear_display();
                    clcd_print("It's ready",LINE2(3));
                    clcd_print("Enjoy your food!", LINE3(0));
                    
                    FAN = 0;    //TURN OFF FAN
                    BUZZER = 1; //TURN ON BUZZER FOR 2SEC
                    __delay_ms(2000);
                    BUZZER = 0;
                    clear_display();
                    control_flag = POWER_ON_MODE;
                }
            }
                break;  
        }
    }
    

    return;
}
