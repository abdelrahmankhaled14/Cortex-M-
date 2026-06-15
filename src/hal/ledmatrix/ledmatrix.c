#include "mcal/gpio/gpio.h"
#include "hal/ledmatrix/ledmatrix.h"
#include "OS/sched.h"

static uint8_t LedMatrixSavedPattrn[8];
static uint8_t DelayFlag = 0;
static int row = 0;
static int LedMatrixbusy = 0;
static void DisplayRow (int r , int pattern,LedMatrix_Names_t ledMatrix);


Runnable_t LedMatrixRunnable =
    {
        .callback = LedMatriz_Runnable,
        .period_ms = 2,
        .priority = 5,
        .arg = (void *)LED_MATRIX_1,
        .first_delay_ms = 0};
extern LedMatrix_Config_t LedMatrix_Config[LED_MATRIX_MAX];
void LedMatrix_Init(LedMatrix_Config_t *LedMatrix_Config)
{
    GPIO_Configuration_t LedMatrix_gpio_config;

    for (uint8_t i = 0; i < LED_MATRIX_MAX; i++)
    {
        /* Initialize ROW pins */
        for (uint8_t r = 0; r < 8; r++)
        {
            LedMatrix_gpio_config.port = LedMatrix_Config[i].row_port[r];
            LedMatrix_gpio_config.pin = LedMatrix_Config[i].row_pins[r];
            LedMatrix_gpio_config.direction = OUTPUT;
            LedMatrix_gpio_config.output_type = PUSH_PULL;
            LedMatrix_gpio_config.resistor = NO_RESISTOR;
            LedMatrix_gpio_config.mode = AF_0;
            LedMatrix_gpio_config.speed = HIGH_SPEED;
            LedMatrix_gpio_config.output_value = HIGH;

            GPIO_Init(&LedMatrix_gpio_config, 1);
        }

        /* Initialize COLUMN pins */
        for (uint8_t c = 0; c < 8; c++)
        {
            LedMatrix_gpio_config.port = LedMatrix_Config[i].col_port[c];
            LedMatrix_gpio_config.pin = LedMatrix_Config[i].col_pins[c];
            LedMatrix_gpio_config.direction = OUTPUT;
            LedMatrix_gpio_config.output_type = PUSH_PULL;
            LedMatrix_gpio_config.resistor = NO_RESISTOR;
            LedMatrix_gpio_config.mode = AF_0;
            LedMatrix_gpio_config.speed = HIGH_SPEED;
            LedMatrix_gpio_config.output_value = HIGH;

            GPIO_Init(&LedMatrix_gpio_config, 1);
        }
    }
    Sched_RegisterRunnable(&LedMatrixRunnable);
}

void LedMatrix_Display(LedMatrix_Names_t matrix, uint8_t pattern[8])
{
    if (pattern == NULL)
    {
        /*do nothing*/
    }
    else if (LedMatrixbusy == 1)
    {
        /*do nothing*/
    }
    else
    {

        for (int j = 0; j < 8; j++)
        {
            LedMatrixSavedPattrn[j] = pattern[j];
        }

        DelayFlag = 1;
        row = 0;
    }
}
void LedMatriz_Runnable(void *arg)
{
    LedMatrix_Names_t ledMatrix = (LedMatrix_Names_t)arg;
    if (DelayFlag == 0)
    {
        /*do nothing*/
    }
    if (LedMatrixSavedPattrn == NULL)
    {
        /*do nothing*/
    }
    else
    {
        switch (row)
        {
        case _ROW0:
            DisplayRow(_ROW0,LedMatrixSavedPattrn[_ROW0],ledMatrix);
            row = _ROW1;
            break;
        case _ROW1:
            DisplayRow(_ROW1,LedMatrixSavedPattrn[_ROW1],ledMatrix);
            row = _ROW2;
            break;
        case _ROW2:
            DisplayRow(_ROW2,LedMatrixSavedPattrn[_ROW2],ledMatrix);
            row = _ROW3;
            break;
        case _ROW3:
            DisplayRow(_ROW3,LedMatrixSavedPattrn[_ROW3],ledMatrix);
            row = _ROW4;
            break;
        case _ROW4:
            DisplayRow(_ROW4,LedMatrixSavedPattrn[_ROW4],ledMatrix);
            row = _ROW5;
            break;
        case _ROW5:
            DisplayRow(_ROW5,LedMatrixSavedPattrn[_ROW5],ledMatrix);
            row = _ROW6;        
            break;
        case _ROW6:
            DisplayRow(_ROW6,LedMatrixSavedPattrn[_ROW6],ledMatrix);
            row = _ROW7;
            break;
        case _ROW7:
            DisplayRow(_ROW7,LedMatrixSavedPattrn[_ROW7],ledMatrix);
            row = _ROW0;
            break;
        }
    }
}


static void DisplayRow (int r , int pattern,LedMatrix_Names_t ledMatrix)
{
    int row = (1 << r);
    int col = pattern;
    for (int i = 0; i < 8; i++)
    {
        int value = (col >> i) & 1;
        GPIO_SetPinValue(LedMatrix_Config[ledMatrix].col_port[i],LedMatrix_Config[ledMatrix].col_pins[i], (!value));
    }
    for (int i = 0; i < 8; i++)
    {
        int value = (row >> i) & 1;
        GPIO_SetPinValue(LedMatrix_Config[ledMatrix].row_port[i],LedMatrix_Config[ledMatrix].row_pins[i], value);
    }
}