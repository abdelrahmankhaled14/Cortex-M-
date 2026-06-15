
// cppcheck-suppress normalCheckLevelMaxBranches
#include "hal/lcd/lcd.h"
#include "mcal/gpio/gpio.h"
#include "mcal/systic/systick.h"
#include "OS/sched.h"

extern Lcd_config_t lcd1[LCD_MAX];
void LCD_init(Lcd_config_t *Lcd_config)
{
    Gpio_Port_t controlPorts[] = {Lcd_config->rsPort, Lcd_config->rwPort, Lcd_config->enPort};
    Gpio_Pin_t controlPins[] = {Lcd_config->rsPin, Lcd_config->rwPin, Lcd_config->enPin};

    for (int i = 0; i < 3; i++)
    {
        GPIO_SetPinDirection(controlPorts[i], controlPins[i], OUTPUT);
        GPIO_SetPinMode(controlPorts[i], controlPins[i], AF_0);
        GPIO_SetPinOutputType(controlPorts[i], controlPins[i], PUSH_PULL);
    }

    for (int i = 0; i < 8; i++)
    {
        GPIO_SetPinDirection(Lcd_config->dataPort[i], Lcd_config->dataPin[i], OUTPUT);
        GPIO_SetPinMode(Lcd_config->dataPort[i], Lcd_config->dataPin[i], AF_0);
        GPIO_SetPinOutputType(Lcd_config->dataPort[i], Lcd_config->dataPin[i], PUSH_PULL);
    }

    // Wait after power-on
    SysTick_StartBusyWait(20);

    LCD_SendCommand(Lcd_config, LCD_FUNCTION_SET_8BIT_2LINE_5x8); // 8-bit, 2 lines, 5x8 font

    LCD_SendCommand(Lcd_config, LCD_CLEAR);
    SysTick_StartBusyWait(5); // CLEAR needs longer delay

    LCD_SendCommand(Lcd_config, LCD_CURSOR_HOME);
    SysTick_StartBusyWait(5); // CURSOR_HOME also needs longer delay

    LCD_SendCommand(Lcd_config, LCD_DISPLAY_OFF_UNDERLINE_OFF_CURSOR_OFF);

    LCD_SendCommand(Lcd_config, LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON);
    SysTick_StartBusyWait(1);
}

void LCD_SendCommand(Lcd_config_t *Lcd_config, LCD_Commands_t command)
{
    GPIO_SetPinValue(Lcd_config->rsPort, Lcd_config->rsPin, LOW);
    GPIO_SetPinValue(Lcd_config->rwPort, Lcd_config->rwPin, LOW);


    for (int i = 0; i < 8; i++)
    {
        int value = (command >> i) & 1;
        GPIO_SetPinValue(Lcd_config->dataPort[i], Lcd_config->dataPin[i], value);
    }

    SysTick_StartBusyWait(1);   

    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, HIGH);
    SysTick_StartBusyWait(1);
    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, LOW);

    
    if (command == LCD_CLEAR || command == LCD_CURSOR_HOME)
        SysTick_StartBusyWait(5);
    else
        SysTick_StartBusyWait(1);
}

void LCD_Write_Char(Lcd_config_t *Lcd_config, char c)
{
    GPIO_SetPinValue(Lcd_config->rsPort, Lcd_config->rsPin, HIGH);
    GPIO_SetPinValue(Lcd_config->rwPort, Lcd_config->rwPin, LOW);
    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, HIGH);
    SysTick_StartBusyWait(1);

    for (int i = 0; i < 8; i++)
    {
        int value = (c >> i) & 1;
        GPIO_SetPinValue(Lcd_config->dataPort[i], Lcd_config->dataPin[i], value);
    }
    SysTick_StartBusyWait(1);

    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, LOW);
}

void LCD_Write_String(Lcd_config_t *Lcd_config, char *Buffer)
{
    for (int i = 0; Buffer[i] != '\0'; i++)
    {
        LCD_Write_Char(Lcd_config, Buffer[i]);
    }
}

void LCD_Clear(Lcd_config_t *Lcd_config)
{
    LCD_SendCommand(Lcd_config, LCD_CLEAR);
}

void LCD_Set_Cursor_postion(Lcd_config_t *Lcd_config, int row, int column)
{
    if (row < 1)
        row = 1;
    if (row > NUMBER_OF_ROWS)
        row = NUMBER_OF_ROWS;

    if (column < 1)
        column = 1;
    if (column > NUMBER_OF_COLOMNS)
        column = NUMBER_OF_COLOMNS;

    uint8_t address = 0;
    if (row == ROW1)
    {
        address = 0x00 + (column - 1);
    }
    else if (row == ROW2)
    {
        address = 0x40 + (column - 1);
    }
    LCD_SendCommand(Lcd_config, LCD_DDRAM_START | address);
}

/*************************Async********************************/

static LCD_state_t LCD_state = LCD_NO_ACTION;
static LCD_init_seq_t LCD_init_seq = NO_ACTION;
static LCD_Commamd_seq_t LCD_Commamd_seq = COMMAND_NO_ACTION;
static LCD_write_seq_t LCD_write_seq = WRITE_NO_ACTION;
static LCD_Commands_t cmd;

static Init_t initState = NOT_DONE;

static char requestedString[NUMBER_OF_ROWS * NUMBER_OF_COLOMNS];

static int stringIndex = 0;

Runnable_t LCD_Runnable_1 =
    {

        .callback = LCD_Runnable,
        .first_delay_ms = 0,
        .period_ms = 2,
        .priority = 3,
        .arg = (void *)LCD_1,

};

static void LCD_EnLowFor(Lcd_config_t *Lcd_config)
{
    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, LOW);
}

void LCD_Async_init(Lcd_config_t *Lcd_config)
{
    Sched_RegisterRunnable(&LCD_Runnable_1);
    Gpio_Port_t controlPorts[] = {Lcd_config->rsPort, Lcd_config->rwPort, Lcd_config->enPort};
    Gpio_Pin_t controlPins[] = {Lcd_config->rsPin, Lcd_config->rwPin, Lcd_config->enPin};

    for (int i = 0; i < 3; i++)
    {
        GPIO_SetPinDirection(controlPorts[i], controlPins[i], OUTPUT);
        GPIO_SetPinMode(controlPorts[i], controlPins[i], AF_0);
        GPIO_SetPinOutputType(controlPorts[i], controlPins[i], PUSH_PULL);
    }

    for (int i = 0; i < 8; i++)
    {
        GPIO_SetPinDirection(Lcd_config->dataPort[i], Lcd_config->dataPin[i], OUTPUT);
        GPIO_SetPinMode(Lcd_config->dataPort[i], Lcd_config->dataPin[i], AF_0);
        GPIO_SetPinOutputType(Lcd_config->dataPort[i], Lcd_config->dataPin[i], PUSH_PULL);
    }
    LCD_state = LCD_INIT;
    LCD_init_seq = FUNCTION_SET_START;
}
static void LCD_Async_SendCommand_internal(Lcd_config_t *Lcd_config, LCD_Commands_t LCD_command)
{
    GPIO_SetPinValue(Lcd_config->rsPort, Lcd_config->rsPin, LOW);
    GPIO_SetPinValue(Lcd_config->rwPort, Lcd_config->rwPin, LOW);
    for (int i = 0; i < 8; i++)
    {
        int value = (LCD_command >> i) & 1;
        GPIO_SetPinValue(Lcd_config->dataPort[i], Lcd_config->dataPin[i], value);
    }
    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, HIGH);
}

static void LCD_Async_Write_Char(Lcd_config_t *Lcd_config, char c)
{
    GPIO_SetPinValue(Lcd_config->rsPort, Lcd_config->rsPin, HIGH);
    GPIO_SetPinValue(Lcd_config->rwPort, Lcd_config->rwPin, LOW);
    for (int i = 0; i < 8; i++)
    {
        int value = (c >> i) & 1;
        GPIO_SetPinValue(Lcd_config->dataPort[i], Lcd_config->dataPin[i], value);
    }
    GPIO_SetPinValue(Lcd_config->enPort, Lcd_config->enPin, HIGH);
}
status_t LCD_Async_Write_String(Lcd_config_t *Lcd_config, char *Buffer)
{
    status_t ret = STATUS_OK;
    if (Buffer == NULL)
    {
        ret = STATUS_ERROR;
    }
    else if ((LCD_init_seq != NO_ACTION) || (LCD_state != LCD_NO_ACTION) || (initState != DONE) || (LCD_Commamd_seq != COMMAND_NO_ACTION))
    {
        ret = STATUS_ERROR;
    }
    else
    {
        for (int i = 0; i < (NUMBER_OF_ROWS * NUMBER_OF_COLOMNS) - 1; i++)
        {
            requestedString[i] = '\0';
        }
        int i = 0;
        while (Buffer[i] != '\0' && i < NUMBER_OF_ROWS * NUMBER_OF_COLOMNS - 1)
        {
            requestedString[i] = Buffer[i];
            i++;
        }
        requestedString[i] = '\0';
        stringIndex = 0;
        LCD_state = LCD_WRITE;
        LCD_write_seq = CURSUR_POSITION_START;
    }
    return ret;
}
void LCD_Async_SendCommand(Lcd_config_t *Lcd_config, LCD_Commands_t LCD_command)
{
    if ((LCD_init_seq != NO_ACTION) || (LCD_state != LCD_NO_ACTION) || (initState != DONE) || (LCD_write_seq != WRITE_NO_ACTION))
    {
        // Do nothing
    }
    else
    {
        cmd = LCD_command;
        LCD_state = LCD_COMMAND;
        LCD_Commamd_seq = COMMAND_START;
    }
}

void LCD_Async_Clear(Lcd_config_t *Lcd_config)
{
    LCD_Async_SendCommand(Lcd_config, LCD_CLEAR);
}
static void LCD_Async_Set_Cursor_postion_internal(Lcd_config_t *Lcd_config, int row, int column)
{
    if (row < 1)
        row = 1;
    if (row > NUMBER_OF_ROWS)
        row = NUMBER_OF_ROWS;

    if (column < 0)
        column = 0;
    if (column > NUMBER_OF_COLOMNS)
        column = NUMBER_OF_COLOMNS;

    uint8_t address = 0;
    if (row == ROW1)
    {
        address = 0x00 + (column - 1);
    }
    else if (row == ROW2)
    {
        address = 0x40 + (column - 1);
    }
    LCD_Async_SendCommand_internal(Lcd_config, LCD_DDRAM_START | address);
}
void LCD_Async_Set_Cursor_postion(Lcd_config_t *Lcd_config, int row, int column)
{
    if ((LCD_init_seq != NO_ACTION) || (LCD_state != LCD_NO_ACTION) || (initState != DONE) || (LCD_write_seq != WRITE_NO_ACTION))
    {
        // Do nothing
    }
    else
    {
        if (row < 1)
            row = 1;
        if (row > NUMBER_OF_ROWS)
            row = NUMBER_OF_ROWS;

        if (column < 0)
            column = 0;
        if (column > NUMBER_OF_COLOMNS)
            column = NUMBER_OF_COLOMNS;

        uint8_t address = 0;
        if (row == ROW1)
        {
            address = 0x00 + (column - 1);
        }
        else if (row == ROW2)
        {
            address = 0x40 + (column - 1);
        }

        LCD_state = LCD_COMMAND;
        LCD_Commamd_seq = COMMAND_START;
        cmd = LCD_DDRAM_START | address;
    }
}

void LCD_Runnable(void *arg)
{
    LCD_Names_t lcd = (LCD_Names_t)arg;
    switch (LCD_state)
    {
    case LCD_NO_ACTION:
        break;
    case LCD_INIT:
        switch (LCD_init_seq)
        {
        case NO_ACTION:
            break;
        case FUNCTION_SET_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_FUNCTION_SET_8BIT_2LINE_5x8);
            LCD_init_seq = FUNCTION_SET_END;
            break;
        case FUNCTION_SET_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = DISPLAY_ON_START;
            break;
        case DISPLAY_ON_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_DISPLAY_OFF_UNDERLINE_OFF_CURSOR_OFF);
            LCD_init_seq = DISPLAY_ON_END;
            break;
        case DISPLAY_ON_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = CURSOR_HOME_START;
            break;
        case CURSOR_HOME_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_CURSOR_HOME);
            LCD_init_seq = CURSOR_HOME_END;
            break;
        case CURSOR_HOME_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = CLEAR_LCD_START;
            break;
        case CLEAR_LCD_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_CLEAR);
            LCD_init_seq = CLEAR_LCD_END;
            break;
        case CLEAR_LCD_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = ENTRY_MODE_START;
            break;
        case ENTRY_MODE_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_INCREMENT_SHIFT_OFF);
            LCD_init_seq = ENTRY_MODE_END;
            break;
        case ENTRY_MODE_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = INIT_MODE_START;
            break;
        case INIT_MODE_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON);
            LCD_init_seq = INIT_MODE_END;
            break;
        case INIT_MODE_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_init_seq = NO_ACTION;
            LCD_state = LCD_NO_ACTION;
            initState = DONE;
            break;
        default:
            break;
        }
        break;
    case LCD_WRITE:
        switch (LCD_write_seq)
        {
        case WRITE_NO_ACTION:
            break;
        case CURSUR_POSITION_START:
            if (stringIndex == 16)
            {
                LCD_Async_Set_Cursor_postion_internal(&lcd1[lcd], ROW2, stringIndex % 16);
            }
            LCD_write_seq = CURSUR_POSITION_END;
            break;
        case CURSUR_POSITION_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_write_seq = WRITE_CHAR_START;
            break;
        case WRITE_CHAR_START:
            LCD_Async_Write_Char(&lcd1[lcd], requestedString[stringIndex]);
            stringIndex++;
            LCD_write_seq = WRITE_CHAR_END;
            break;
        case WRITE_CHAR_END:
            LCD_EnLowFor(&lcd1[lcd]);
            if (((requestedString[stringIndex]) == '\0') || (stringIndex >= NUMBER_OF_ROWS * NUMBER_OF_COLOMNS))
            {
                LCD_state = LCD_NO_ACTION;
                LCD_write_seq = WRITE_NO_ACTION;
            }
            else
            {
                LCD_write_seq = CURSUR_POSITION_START;
            }
            break;

        default:
            break;
        }
    case LCD_COMMAND:
        switch (LCD_Commamd_seq)
        {
        case COMMAND_NO_ACTION:
            break;
        case COMMAND_START:
            LCD_Async_SendCommand_internal(&lcd1[lcd], cmd);
            LCD_Commamd_seq = COMMAND_END;
            break;
        case COMMAND_END:
            LCD_EnLowFor(&lcd1[lcd]);
            LCD_Commamd_seq = COMMAND_NO_ACTION;
            LCD_state = LCD_NO_ACTION;
            break;
        default:
            break;
        }
        break;
    }
}
