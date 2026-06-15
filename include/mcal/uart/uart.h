#ifndef _UART_H
#define _UART_H

#include "std_types.h"

/*************************************************************************
 ****************************   Data type   ******************************
 *************************************************************************
*/

typedef enum
{
    NO_PARTITY,ODD_PARITY,EVEN_PARITY
}Parity_bit_t;

typedef enum
{
    _1BIT,_0_5BIT,_2BIT,_1_5BIT
}stop_bit_t;
typedef enum
{
    NORMAL,INTERRUPT,DMA
}mode_t;

typedef enum
{
    UART_1,UART_2,UART_6
}Uart_t;

typedef struct 
{ 
   uint32_t Baud_rate;
   Parity_bit_t Parity_bit;
   stop_bit_t stop_bit;
   mode_t mode; 
   Uart_t Uart;
   
}Uart_Config_t;

typedef enum
{
    SEND,                                      // Check send busy flag
    RECEIVE                                    // Check receive busy flag
} send_receiveflag_t;
/********************************************************************** 
***************************Function Prototype***************************
***********************************************************************/
void UART_Init(Uart_Config_t *Uart_Config);


#endif
