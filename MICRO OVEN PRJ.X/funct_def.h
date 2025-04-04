/*
 * funct_def.h 
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FUNCT_DEF_H
#define	FUNCT_DEF_H

void clear_display();

void validate_time();

unsigned char micro_mode(unsigned char key);

unsigned char grill_mode(unsigned char key);

unsigned char convection_mode(unsigned char key);

unsigned char start_mode(unsigned char key);

unsigned char start_operation(void);


#endif	/* XC_HEADER_TEMPLATE_H */

