#include "hal/lcd/lcd.h"



Lcd_config_t lcd1[LCD_MAX] = 
{
    [LCD_1]
    {
    .rsPort = PORTB, .rsPin = PIN0,
    .rwPort = PORTB, .rwPin = PIN1,
    .enPort = PORTB, .enPin = PIN2,

    .dataPort = {PORTB, PORTB, PORTB, PORTB, PORTB, PORTB, PORTB, PORTB},
    .dataPin  = {PIN3, PIN4, PIN5, PIN6, PIN7, PIN8, PIN9, PIN10}
    }

};