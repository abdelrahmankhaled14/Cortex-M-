#include "hal/switch/switch.h"
#include "mcal/gpio/gpio.h"
#include "OS/sched.h"
extern SWITCH_Configuration_t SWITCH_Config[SWITCH_MAX];


Runnable_t SWITCH_Runnable_1 =
{
    .callback = SWITCH_runnnable,
    .first_delay_ms = 0,
    .period_ms = 1,
    .priority = 1,
    .arg = NULL,
};
uint8_t Prev_Switch_State[SWITCH_MAX];
uint8_t Switch_state[SWITCH_MAX];
uint8_t Switch_counter[SWITCH_MAX];
uint8_t Current_state[SWITCH_MAX];

void SWITCH_Init(SWITCH_Configuration_t* SWITCH_Configuration, uint8_t size)

{

    GPIO_Configuration_t switch_gpio_config;  
    for (uint8_t i = 0; i < size; i++)
    {
        switch_gpio_config.port = SWITCH_Config[i].port;
        switch_gpio_config.pin = SWITCH_Config[i].pin;
        switch_gpio_config.direction = INPUT;
        switch_gpio_config.output_type = PUSH_PULL;
        switch_gpio_config.output_value = LOW;

        if (SWITCH_Config[i].resistor == SWITCH_INTERNAL_PULL_U )
        {
            switch_gpio_config.resistor = PULL_UP;
            Prev_Switch_State[i]= 1;
        }
        else if (SWITCH_Config[i].resistor == SWITCH_INTERNAL_PULL_D )
        {
            switch_gpio_config.resistor = PULL_DOWN;
            Prev_Switch_State[i]= 0;

        }
        else if (SWITCH_Config[i].resistor == SWITCH_EXTERNAL_PULL_U || SWITCH_Config[i].resistor == SWITCH_EXTERNAL_PULL_D )
        {
            switch_gpio_config.resistor = NO_RESISTOR;
        }

        switch_gpio_config.mode = AF_0;
        switch_gpio_config.speed = HIGH_SPEED;
        GPIO_Init(&switch_gpio_config, 1);
    }
    Sched_RegisterRunnable(&SWITCH_Runnable_1);

}         
status_t SWITCH_GetState(SWITCH_Names_t switch_num, Switch_State_t* state)
{
    status_t ret = STATUS_OK;

    if (switch_num >= SWITCH_MAX || state == NULL)
    {
        ret = STATUS_ERROR;
    }
    else
    {

        if ((SWITCH_Config[switch_num].resistor == SWITCH_INTERNAL_PULL_U) || (SWITCH_Config[switch_num].resistor == SWITCH_EXTERNAL_PULL_U))
        {
            if (Switch_state[switch_num] == LOW)
            {
                *state = SWITCH_PRESSED;
            }
            else
            {
                *state = SWITCH_RELEASED;
            }
        }
        else if ((SWITCH_Config[switch_num].resistor == SWITCH_INTERNAL_PULL_D) || (SWITCH_Config[switch_num].resistor == SWITCH_EXTERNAL_PULL_D))
        {
            if (Switch_state[switch_num] == HIGH)
            {
                *state = SWITCH_PRESSED;
            }
            else
            {
                *state = SWITCH_RELEASED;
            }
        }
    }

    return ret;
}




void SWITCH_runnnable(void*arg)
{
    Switch_State_t state = 0;
    for(int i = 0 ; i < SWITCH_MAX ; i++)
    {
        GPIO_ReadPinValue(SWITCH_Config[i].port,SWITCH_Config[i].pin,&state);
        Current_state[i] = state;
        if(Current_state[i] == Prev_Switch_State[i])
        {
            Switch_counter[i]++;
        }
        else
        {
            Switch_counter[i] = 0;
            Prev_Switch_State[i] = Current_state[i];
        }
        if(Switch_counter[i] >= 5)
        {
            Switch_state[i]=Current_state[i];
            Switch_counter[i] = 0;
            Prev_Switch_State[i] = Current_state[i];
        }
    }

}