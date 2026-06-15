#include "mcal/gpio/gpio.h"



typedef struct
{
    Gpio_Port_t rsPort;
    Gpio_Pin_t  rsPin;
    Gpio_Port_t rwPort;
    Gpio_Pin_t  rwPin;
    Gpio_Port_t enPort;
    Gpio_Pin_t  enPin;

    Gpio_Port_t dataPort[8];
    Gpio_Pin_t  dataPin[8];
} Lcd_config_t;

typedef enum
{
    LCD_1,
    LCD_MAX,
}LCD_Names_t;