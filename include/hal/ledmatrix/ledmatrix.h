#ifndef LED_MATRIX_H
#define LED_MATRIX_H
#include "std_types.h"
#include "hal/ledmatrix/ledmatrix_cfg.h"

typedef enum
{
    MATRIX_PORTA, MATRIX_PORTB, MATRIX_PORTC, MATRIX_PORTD, MATRIX_PORTE, MATRIX_PORTH
}Matrix_Port_t;

typedef enum
{
    MATRIX_PIN0, MATRIX_PIN1, MATRIX_PIN2, MATRIX_PIN3, MATRIX_PIN4,
    MATRIX_PIN5, MATRIX_PIN6, MATRIX_PIN7, MATRIX_PIN8, MATRIX_PIN9,
    MATRIX_PIN10, MATRIX_PIN11, MATRIX_PIN12, MATRIX_PIN13, MATRIX_PIN14, MATRIX_PIN15
}Matrix_Pin_t;

typedef enum
{
    ROW,
    COLUMN
}Matrix_Anode_t;

typedef struct
{
    Matrix_Port_t row_port[8];
    Matrix_Pin_t  row_pins[8];

    Matrix_Port_t col_port[8];
    Matrix_Pin_t  col_pins[8];
    Matrix_Anode_t anode;

}LedMatrix_Config_t;

typedef enum
{
    _ROW0,
    _ROW1,
    _ROW2,
    _ROW3,
    _ROW4,
    _ROW5,
    _ROW6,
    _ROW7
}PatternRow;


void LedMatrix_Init(LedMatrix_Config_t* LedMatrix_Config);
void LedMatrix_Display(LedMatrix_Names_t matrix, uint8_t pattern[8]);
void LedMatriz_Runnable(void * arg);

#endif