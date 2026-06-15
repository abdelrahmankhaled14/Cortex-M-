
#include "hal/switch/switch.h"

const SWITCH_Configuration_t SWITCH_Config[SWITCH_MAX] = 
{
    [SWITCH_1]{
        .port = SWITCH_PORTA,
        .pin = SWITCH_PIN6,
        .resistor = SWITCH_INTERNAL_PULL_U
    }/* SWITCH_1 */
};
