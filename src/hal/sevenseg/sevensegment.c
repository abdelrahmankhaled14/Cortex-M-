#include "mcal/gpio/gpio.h"
#include "hal/sevensegment/sevensegment.h"

static const uint8_t SevenSeg_Lookup[16] =
    {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
        0b01110111, // A
        0b01111100, // B
        0b00111001, // C
        0b01011110, // D
        0b01111001, // E
        0b01110001  // F
};
extern SevenSeg_Config_t SevenSeg_Config[SEVENSEG_MAX];

void SEVENSEG_Init(const SevenSeg_Config_t *SevenSeg_Config)
{
    GPIO_Configuration_t Sevenseg_gpio_config;
    for (uint8_t i = 0; i < SEVENSEG_MAX; i++) /*Seven segment itself*/
    {
        Sevenseg_gpio_config.port = SevenSeg_Config[i].Control_port;
        Sevenseg_gpio_config.pin = SevenSeg_Config[i].Control_pin;
        Sevenseg_gpio_config.direction = OUTPUT;
        Sevenseg_gpio_config.output_type = PUSH_PULL;
        Sevenseg_gpio_config.resistor = NO_RESISTOR;
        Sevenseg_gpio_config.mode = AF_0;
        Sevenseg_gpio_config.speed = HIGH_SPEED;
        if (SevenSeg_Config->type == COMMON_ANODE)
        {
            Sevenseg_gpio_config.output_value = HIGH;
        }
        else
        {
            Sevenseg_gpio_config.output_value = LOW;
        }
        GPIO_Init(&Sevenseg_gpio_config, 1);
        for (uint8_t j = 0; j < 8; j++) /*Seven segment pins*/
        {
            Sevenseg_gpio_config.port = SevenSeg_Config[i].port[j];
            Sevenseg_gpio_config.pin = SevenSeg_Config[i].pins[j];
            Sevenseg_gpio_config.direction = OUTPUT;
            Sevenseg_gpio_config.output_type = PUSH_PULL;
            Sevenseg_gpio_config.resistor = NO_RESISTOR;
            Sevenseg_gpio_config.mode = AF_0;
            Sevenseg_gpio_config.speed = HIGH_SPEED;
            if (SevenSeg_Config->type == COMMON_ANODE)
            {
                Sevenseg_gpio_config.output_value = HIGH;
            }
            else
            {
                Sevenseg_gpio_config.output_value = LOW;
            }
            GPIO_Init(&Sevenseg_gpio_config, 1);
        }
    }
}

void SEVENSEG_Display(SevenSeg_Names_t seg, SevenSeg_Value_t number)
{
    if (SevenSeg_Config[seg].type == COMMON_CATHODE)
    {
        for (int i = 0; i < 8; i++)
        {
            int bit = (SevenSeg_Lookup[number] >> i) & 1;
            GPIO_SetPinValue(SevenSeg_Config[seg].port[i], SevenSeg_Config[seg].pins[i], bit);
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            int bit = (SevenSeg_Lookup[number] >> i) & 1;
            GPIO_SetPinValue(SevenSeg_Config[seg].port[i], SevenSeg_Config[seg].pins[i], (!bit));
        }
    }
}
