#include "hal/sevensegment/sevensegment.h"

const SevenSeg_Config_t SevenSeg_Config[SEVENSEG_MAX] =
{
    [SEVENSEG_1] = {
        .port = {SEG_PORTA,SEG_PORTA,SEG_PORTA,SEG_PORTA,SEG_PORTA,SEG_PORTA,SEG_PORTA,SEG_PORTA},
        .pins = {SEG_PIN0, SEG_PIN1, SEG_PIN2, SEG_PIN3, SEG_PIN4, SEG_PIN5, SEG_PIN6, SEG_PIN7},
        .type = COMMON_ANODE,
        .Control_port = SEG_PORTA,
        .Control_pin = SEG_PIN8
    }
    
};

/*
    [SEVENSEG_2] = {
        .port = {SEG_PORTB,SEG_PORTB,SEG_PORTB,SEG_PORTB,SEG_PORTB,SEG_PORTB,SEG_PORTB,SEG_PORTB},
        .pins = {SEG_PIN0, SEG_PIN1, SEG_PIN2, SEG_PIN3, SEG_PIN4, SEG_PIN5, SEG_PIN6, SEG_PIN7},
        .type = COMMON_ANODE,
        .Control_port = SEG_PORTB,
        .Control_pin = SEG_PIN8


*/