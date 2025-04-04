/*
 * File:   funct_def.c
 */

#include "main.h"

unsigned char timer[] = "0000"; //, temp[] = "000";
unsigned char sec[] = "00", min[] = "00";
static unsigned char micro_once = 1, grill_once = 1, convec_once = 1, start_once = 1; //flag to use in start mode

extern unsigned int half_sec;


//*********************** function to give time as input *************************
unsigned char handle_time_input(unsigned char key, char *flag, char *pos) {
    if (*flag == 1) { // Entering sec field
        if (*pos > 0) {
            sec[*pos - 1] = key + '0';
            (*pos)--;
        } else {
            *flag = 0;
            *pos = 2;
        }
    } else { // Entering min field
        if (*pos> 0) {
            min[*pos - 1] = key + '0';
            (*pos)--;
        } else {
            *flag = 1;
            *pos = 2;
        }
    }

    // Handle Clear Key '*'
    if (key == '*') {
        sec[0] = '0';
        sec[1] = '0';
        min[0] = '0';
        min[1] = '0';
        *flag = 1;
        *pos = 2;
    }

    // Handle Enter Key '#'
    if (key == '#') {
        timer[0] = min[1];
        timer[1] = min[0];
        timer[2] = sec[1];
        timer[3] = sec[0];

        if (timer[0] == '\0' || timer[1] == '\0' || timer[2] == '\0' || timer[3] == '\0') {
            timer[0] = '0';
            timer[1] = '0';
            timer[2] = '0';
            timer[3] = '0';
        }

        return START_FLAG;  // Return start flag to indicate transition
    }

    //return 0;  // Return 0 if no transition
}


//************************ function to clear the clcd display ************************
void clear_display()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
}

//**********************  function to blink the timer ***************************
void blink_timer_display(char micro_flag, char micro_pos) {
    static unsigned char blink_state = 1;
    static unsigned int blink_counter = 0;

    // Blink toggle logic
    if (++blink_counter == 2) {  // Adjust the value based on system speed
        blink_state = !blink_state;
        blink_counter = 0;
    }

    // Print Min Field
    if (micro_flag == 0 && ((micro_pos - 1) == 1) && blink_state) 
        clcd_putch('_', LINE2(5)); // Blink tens place
    else 
        clcd_putch(min[1], LINE2(5));

    if (micro_flag == 0 && ((micro_pos - 1) == 0) && blink_state) 
        clcd_putch('_', LINE2(6)); // Blink ones place
    else 
        clcd_putch(min[0], LINE2(6));

    clcd_putch(':', LINE2(7));

    // Print Sec Field
    if (micro_flag == 1 && ((micro_pos - 1) == 1) && blink_state) 
        clcd_putch('_', LINE2(8)); // Blink tens place
    else 
        clcd_putch(sec[1], LINE2(8));

    if (micro_flag == 1 && ((micro_pos - 1) == 0) && blink_state) 
        clcd_putch('_', LINE2(9)); // Blink ones place
    else 
        clcd_putch(sec[0], LINE2(9));
}


//**************function to check the given time is valid ************************
void validate_time()
{
    int seconds = (sec[1] - '0') * 10 + (sec[0] - '0');
    int minutes = (min[1] - '0') * 10 + (min[0] - '0');

    // If seconds exceed 59, reset to 00 and increase minutes
    if (seconds > 59)
    {
        sec[0] = '0';
        sec[1] = '0';

        minutes++; // Increment minutes
        if (minutes > 60) // Prevent minutes from exceeding 60
        {
            minutes = 0;
        }

        // Convert minutes back to character array
        min[1] = (minutes / 10) + '0';
        min[0] = (minutes % 10) + '0';
    }

    // If minutes exceed 60, reset to 00
    if (minutes > 60)
    {
        min[0] = '0';
        min[1] = '0';
    }
}

//**************** micro mode function call ************************
//this function requires a time which is set by user and can add 30sec extra or pause or stop 
//the timer as needed, after completing the operation system should give a buzz sound

unsigned char micro_mode(unsigned char key)
{
    static char micro_pos = 2, micro_flag = 1, blink_toggle = 1;
    
    if (micro_once) 
    { 
        micro_once = 0;
        key = ALL_RELEASED;
        clear_display();
        
        // Reset the timer and cursor position
        sec[0] = '0'; sec[1] = '0';
        min[0] = '0'; min[1] = '0';
        micro_pos = 2;  // Reset cursor position
        micro_flag = 1; // Ensure we start at seconds field
    }
    
    //if any key pressed for min and sec field
    if(key != ALL_RELEASED)
    {
        unsigned char result = handle_time_input(key, &micro_flag, &micro_pos);
        if (result == START_FLAG) {
            micro_once = 1;
            return START_FLAG;
        }
        
    }
    validate_time();
    
    clcd_print("SET TIME <MM:SS>",LINE1(0));
    clcd_print("TIME: ",LINE2(0));
    
    // Call the blink function to handle blinking logic
    blink_timer_display(micro_flag, micro_pos);
    
    clcd_print("* Clear  # Enter",LINE4(0));

}

//*******************grill mode function call************************
//this function requires a time which is set by user and can add 30sec extra or pause or stop 
//the timer as needed, after completing the operation system should give a buzz sound

unsigned char grill_mode(unsigned char key)
{
    static char grill_pos = 2, grill_flag = 1, blink_toggle = 1;
    
    if (grill_once) 
    { 
        grill_once = 0;
        key = ALL_RELEASED;
        clear_display();
        
        // Reset the timer and cursor position
        sec[0] = '0'; sec[1] = '0';
        min[0] = '0'; min[1] = '0';
        grill_pos = 2;  // Reset cursor position
        grill_flag = 1; // Ensure we start at seconds field
    }
    
    //if any key pressed for min and sec field
    if(key != ALL_RELEASED)
    {
        
        unsigned char result = handle_time_input(key, &grill_flag, &grill_pos);
        if (result == START_FLAG) {
            grill_once = 1;
            return START_FLAG;
        }
        
    }
    validate_time();
    
    clcd_print("SET TIME <MM:SS>",LINE1(0));
    clcd_print("TIME: ",LINE2(0));
    
    // Call the blink function to handle blinking logic
    blink_timer_display(grill_flag, grill_pos);
    
    clcd_print("* Clear  # Enter",LINE4(0));
       
}

//************* convection mode **********************
//this function requires a time which is set by user and can add 30sec extra or pause or stop 
//the timer as needed, after completing the operation system should give a buzz sound

unsigned char convection_mode(unsigned char key)
{
    static char convec_pos = 2, convec_flag = 1, pre_heat = 1;
    static int count = 60;
    
    if (convec_once) 
    { 
        //convec_once = 0;
        key = ALL_RELEASED;
        clear_display();
        
        // Reset the timer and cursor position
        sec[0] = '0'; sec[1] = '0';
        min[0] = '0'; min[1] = '0';
        convec_pos = 2;  // Reset cursor position
        convec_flag = 1; // Ensure we start at seconds field
        pre_heat = 1;
        convec_once = 0;
    }
    
    if (pre_heat)
    {
        clear_display; // Clear display before updating
        clcd_print("SET Temp  (*C)", LINE1(0));
        clcd_print("TEMP: ", LINE2(0));
        clcd_print("* Clear  # Enter", LINE4(0));

        //static unsigned int temp = 0; // Store temperature as an integer
        static unsigned int temp = 0;
        static unsigned char temp_pos = 0; // Track position (0 = 100s, 1 = 10s, 2 = 1s)
    
    if (key != ALL_RELEASED && (key >= 0 && key <= 9)) // Accept only numeric keys
    {
        unsigned char digit = key ;//- '0';//+ '0';
        
        if (temp_pos == 0) // 100s place
        {
            temp = (temp % 100) + (digit * 100);
            temp_pos = 1;
        }
        else if (temp_pos == 1) // 10s place
        {
            temp = (temp / 100) * 100 + (temp % 10) + (digit * 10);
            temp_pos = 2;
        }
        else if (temp_pos == 2) // 1s place
        {
            temp = (temp / 10) * 10 + digit;
            //temp_pos = 0;
        }
    }
    
    // Display the entered temperature
    clcd_putch((temp / 100)+'0', LINE2(6)); // Hundreds place
    clcd_putch(((temp / 10) % 10) +'0' , LINE2(7)); // Tens place
    clcd_putch((temp % 10)+'0', LINE2(8)); // Ones place
    
    if (key == '#') // Confirm temperature input
    {
        if (temp > 180)  // Ensure temp is within the valid range (0 - 180°C)
        {
            clear_display();
            clcd_print("Invalid Temp!", LINE1(0));
            __delay_ms(1000);
            clear_display();
            temp = 0; // Reset temperature
            temp_pos = 0;
        }
        else
        {
            key = ALL_RELEASED; // Clear key input
            clear_display();
            clcd_print("Pre-Heating", LINE1(0));
            clcd_print("Time Left: ", LINE2(0));
            clcd_putch('S',LINE2(13));
            //pre_heat = 0; // Exit preheat mode
            while (count >= 0)
            {
                
                clcd_putch((count / 10) + '0', LINE2(11));
                clcd_putch((count % 10) + '0', LINE2(12));
                
                
                __delay_ms(1000); // Delay for 1 second
                count--;
            }
            clear_display();
            pre_heat = 0;   //exit preheat mode
        }
        temp = 0;//RESET TEMP
        
    }
    
    if (key == '*') // Clear the entered temperature
    {
        temp = 0;
        temp_pos = 0;
        clear_display(); // Clear and update display
    }
    }
    
    //
    if(!pre_heat)
    {
      //if any key pressed for min and sec field
        if(key != ALL_RELEASED)
        {
            unsigned char result = handle_time_input(key, &convec_flag, &convec_pos);
        if (result == START_FLAG) {
            convec_once = 1;
            return START_FLAG;
        }
        
    }
    validate_time();
    
    clcd_print("SET TIME <MM:SS>",LINE1(0));
    clcd_print("TIME: ",LINE2(0));
    
    // Call the blink function to handle blinking logic
    blink_timer_display(convec_flag, convec_pos);
    
    clcd_print("* Clear  # Enter",LINE4(0));  
    }

}

//**************** start mode function call *************************
//start mode is used to heat the food for 30sec before eating
unsigned char start_mode(unsigned char key)
{
    
    if(start_once)
    {
        start_once = 0;
        key = ALL_RELEASED;
        clear_display();
        
        // Reset the timer and cursor position
        sec[0] = '0'; sec[1] = '0';
        min[0] = '0'; min[1] = '0';
        
        timer[2] = '3'; timer[3] = '0';
    }
    
    start_once = 1;
    return START_FLAG;

    validate_time();
    
    clcd_print("SET TIME <MM:SS>",LINE1(0));
    clcd_print("TIME: ",LINE2(0));
    
    // Call the blink function to handle blinking logic
    //blink_timer_display(start_flag, start_pos);
    
    clcd_print("* Clear  # Enter",LINE4(0));
    
    
    
}

//************************Start operation function call ******************
//this function will run the timer and after reaching timer gives a buzzer sound & turn off the fan

unsigned char start_operation()
{

    static unsigned char running_mode = 1;
    unsigned char secs = 0, key, paused_flag = 0;
    int total_seconds = 0;
    half_sec = 0;    //starting sec with 0
    
    // Ensure all time values are properly initialized
    if (timer[3] < '0' || timer[3] > '9') 
        timer[3] = '0';
    if (timer[2] < '0' || timer[2] > '9') 
        timer[2] = '0';
    if (timer[1] < '0' || timer[1] > '9') 
        timer[1] = '0';
    if (timer[0] < '0' || timer[0] > '9') 
        timer[0] = '0';

    
    // **Convert timer[] to an integer value (MMSS)**
    total_seconds = ((timer[0] - '0') * 600) +  // MM first digit
                    ((timer[1] - '0') * 60) +   // MM second digit
                    ((timer[2] - '0') * 10) +   // SS first digit
                    (timer[3] - '0');          // SS second digit
    
    
    clcd_putch(timer[0], LINE1(9));
    clcd_putch(timer[1], LINE1(10));
    clcd_putch(':',LINE1(11));
    clcd_putch(timer[2], LINE1(12));
    clcd_putch(timer[3], LINE1(13));
    
    FAN = 1;//turn on fan after timer set
    
    while (total_seconds > 0)
    {
        //FAN = 1;//turn on fan after timer set
        
        key = read_matrix_keypad(STATE);  // Continuously read key
        
            if(key == 4)
            {
                
                if(paused_flag) //if flag is raised
                {
                    FAN = 1;
                    paused_flag = 0;    //turning off flag to resume timer
                }
                else            //if flag is turned off
                {
                    total_seconds+=30;
                }
                
            }
            else if(key == 5)   //pause the timer in running mode
            {
                
                FAN = 0;
                paused_flag = 1;//raise the flag    
               
            }
            else if(key == 6)   //stop the timer 
            {
                min[0] = '0';
                min[1] = '0';
                sec[0] = '0';
                sec[1] = '0';
                FAN = 0;    //turn off fan
                micro_once = 1;
                grill_once = 1;
                convec_once = 1;
                start_once = 1;
                return STOP;    //exit the timer
            }
        
        if (!paused_flag && half_sec >= 2)  // Every second (half_sec increments twice per second)
        {
            half_sec = 0;  // Reset half_sec

            //clcd_putch(key+'0',LINE4(14));

            if (total_seconds == 0)  
            {
                break;  // Stop when reaching "00:00"
                //return OP_SUCCESS;
            }
            

            total_seconds--;  // Decrement time

            // **Convert back to character format**
            timer[0] = (total_seconds / 600) + '0';
            timer[1] = ((total_seconds % 600) / 60) + '0';
            timer[2] = ((total_seconds % 60) / 10) + '0';
            timer[3] = (total_seconds % 10) + '0';

            // **Update display**
            clcd_putch(timer[0], LINE1(9));
            clcd_putch(timer[1], LINE1(10));
            clcd_putch(':',LINE1(11));
            clcd_putch(timer[2], LINE1(12));
            clcd_putch(timer[3], LINE1(13));
        }
        
    }
    return OP_SUCCESS;

}
