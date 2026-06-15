// sevensegment.h
#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H
#include "std_types.h"
#include "hal/sevensegment/sevensegment_cfg.h"

typedef enum
{
    SEG_PORTA, SEG_PORTB, SEG_PORTC, SEG_PORTD, SEG_PORTE, SEG_PORTH
}Seg_Port_t;

typedef enum
{
    SEG_PIN0, SEG_PIN1, SEG_PIN2, SEG_PIN3, SEG_PIN4, SEG_PIN5, SEG_PIN6, SEG_PIN7,SEG_PIN8,SEG_PIN9,SEG_PIN10,SEG_PIN11,SEG_PIN12,SEG_PIN13,SEG_PIN14,SEG_PIN15
}Seg_Pin_t;

typedef enum
{
    COMMON_ANODE,
    COMMON_CATHODE
}Seg_Type_t;

typedef struct
{
    Seg_Port_t port[8];
    Seg_Pin_t pins[8];
    uint8_t Control_port;
    uint8_t Control_pin;
    Seg_Type_t type;
}SevenSeg_Config_t;

typedef enum
{
    SEVEN_SEG_0,
    SEVEN_SEG_1,
    SEVEN_SEG_2,
    SEVEN_SEG_3,
    SEVEN_SEG_4,
    SEVEN_SEG_5,
    SEVEN_SEG_6,
    SEVEN_SEG_7,
    SEVEN_SEG_8,
    SEVEN_SEG_9,
    SEVEN_SEG_A,
    SEVEN_SEG_B,
    SEVEN_SEG_C,
    SEVEN_SEG_D,
    SEVEN_SEG_E,
    SEVEN_SEG_F
} SevenSeg_Value_t;



void SEVENSEG_Init( const SevenSeg_Config_t* SevenSeg_Config);
void SEVENSEG_Display(SevenSeg_Names_t seg, SevenSeg_Value_t number);

#endif