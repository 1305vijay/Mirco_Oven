#ifndef MAIN_H
#define MAIN_H


#include <xc.h>
#include <string.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"
#include "funct_def.h"

#define FAN                         RC2
#define FAN_DDR                     TRISC2
#define BUZZER                      RC1
#define BUZZER_DDR                  TRISC1


//CONTROL FLAGS
#define POWER_ON_MODE   0xAA
#define MICRO_MODE      0xBB
#define GRILL_MODE      0xCC
#define CONVECTION_MODE 0xDD
#define START_MODE      0xEE
#define START_OPERATION 0X66

//return  of funct
#define START_FLAG      0x11
#define ADD_30SEC       0x22
#define RESUME          0x33
#define STOP            0x44
#define OP_SUCCESS      0X55

#endif MAIN_H