#ifndef SWITCH_H
#define SWITCH_H
#include "std_types.h"
#include "hal/switch/switch_cfg.h"
typedef enum {
    SWITCH_PORTA,SWITCH_PORTB,SWITCH_PORTC,SWITCH_PORTD,SWITCH_PORTE,SWITCH_PORTH
}Switch_Port_t;

typedef enum {
    SWITCH_PIN0,SWITCH_PIN1,SWITCH_PIN2,
    SWITCH_PIN3,SWITCH_PIN4,SWITCH_PIN5,
    SWITCH_PIN6,SWITCH_PIN7,SWITCH_PIN8,
    SWITCH_PIN9,SWITCH_PIN10,SWITCH_PIN11,
    SWITCH_PIN12,SWITCH_PIN13,SWITCH_PIN14,
    SWITCH_PIN15
}Switch_Pin_t;
typedef enum {
    SWITCH_PRESSED,
    SWITCH_RELEASED
}Switch_State_t;
typedef enum {
    SWITCH_NORESISTOR,
    SWITCH_PULLUP,
    SWITCH_PULLDOWN,
}Switch_Resistor_t;
typedef enum {
    SWITCH_EXTERNAL_PULL_U,
    SWITCH_EXTERNAL_PULL_D,
    SWITCH_INTERNAL_PULL_U,
    SWITCH_INTERNAL_PULL_D  
}Switch_Regester_State_t;
typedef struct {
    Switch_Port_t port;
    Switch_Pin_t pin;
    Switch_Regester_State_t resistor;

}SWITCH_Configuration_t;
/*************************************************************************************************************************
 *                                               GPIO Function Prototype                                            *
 *************************************************************************************************************************/
void SWITCH_Init(SWITCH_Configuration_t* SWITCH_Configuration, uint8_t size);
status_t SWITCH_GetState(SWITCH_Names_t switch_num, Switch_State_t* state);
void SWITCH_runnnable(void*arg);
#endif // SWITCH_H