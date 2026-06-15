#ifndef CLCD_H
#define CLCD_H
#include "std_types.h"
#include "lcd_cfg.h"

typedef enum
{
    NO_ACTION,
    FUNCTION_SET_START,
    FUNCTION_SET_END,
    DISPLAY_ON_START,
    DISPLAY_ON_END,
    CURSOR_HOME_START,
    CURSOR_HOME_END,
    CLEAR_LCD_START,
    CLEAR_LCD_END,
    ENTRY_MODE_START,
    ENTRY_MODE_END,
    INIT_MODE_START,
    INIT_MODE_END
}LCD_init_seq_t;
typedef enum
{
    WRITE_NO_ACTION,
    CURSUR_POSITION_START,
    CURSUR_POSITION_END,
    WRITE_CHAR_START,
    WRITE_CHAR_END
}LCD_write_seq_t;

typedef enum
{
    COMMAND_NO_ACTION,
    COMMAND_START,
    COMMAND_END
}LCD_Commamd_seq_t;


typedef enum
{
    LCD_INIT,
    LCD_WRITE,
    LCD_NO_ACTION,
    LCD_COMMAND
}LCD_state_t;

typedef enum
{
    NOT_DONE,
    DONE
}Init_t;

typedef enum
{
    LCD_CLEAR = 0x01,
    LCD_CURSOR_HOME = 0x02,
    LCD_DECREMENT_SHIFT_OFF = 0x04,
    LCD_DECREMENT_SHIFT_ON = 0x05,
    LCD_INCREMENT_SHIFT_OFF = 0x06,
    LCD_INCREMENT_SHIFT_ON = 0x07,
    LCD_DISPLAY_OFF_UNDERLINE_OFF_CURSOR_OFF = 0x08,
    LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF = 0x0C,
    LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON = 0x0D,
    LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF = 0x0E,
    LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON = 0x0F,
    LCD_MOVE_CURSOR_SHIFT_LEFT = 0x10,
    LCD_MOVE_CURSOR_SHIFT_RIGHT = 0x14,
    LCD_SHIFT_DISPLAY_SHIFT_LEFT = 0x18,
    LCD_SHIFT_DISPLAY_SHIFT_RIGHT = 0x1C,
    LCD_FUNCTION_SET_8BIT_2LINE_5x8 = 0x38,
}LCD_Commands_t;


#define LCD_CGRAM_START                             0x40
#define LCD_DDRAM_START                             0x80

#define NUMBER_OF_ROWS          2
#define NUMBER_OF_COLOMNS       16

#define ROW1    1
#define ROW2    2



void LCD_SendCommand(Lcd_config_t* Lcd_config, LCD_Commands_t command);
void LCD_init(Lcd_config_t* Lcd_config);
void LCD_Write_Char(Lcd_config_t* Lcd_config,char c);
void LCD_Write_String(Lcd_config_t* Lcd_config,char *Buffer);
void LCD_Clear(Lcd_config_t* Lcd_config);
void LCD_Set_Cursor_postion(Lcd_config_t* Lcd_config ,int row , int column);

void LCD_Async_init(Lcd_config_t* Lcd_config);
void LCD_Async_SendCommand(Lcd_config_t* Lcd_config, LCD_Commands_t command);
status_t LCD_Async_Write_String(Lcd_config_t* Lcd_config,char *Buffer);
void LCD_Async_Clear(Lcd_config_t* Lcd_config);
void LCD_Async_Set_Cursor_postion(Lcd_config_t* Lcd_config ,int row , int column);
void LCD_Runnable(void *arg);

#endif  /* CLCD_H */