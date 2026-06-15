/*
 * systick.c
 * Created on: Nov 29, 2024
 * Author: abdelrahman Khaled
 */

#include "mcal/systic/systick.h"
#include "macros.h"
#include "std_types.h"

#define SYSTICK_CTRL_REG          (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_RELOAD_REG        (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_CURRENT_REG       (*((volatile uint32_t *)0xE000E018))

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL;

/*******************************************************************************
 *                       Interrupt Handler                             *
 *******************************************************************************/

/*********************************************************************
 * Service Name: SysTick_Handler
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Handler for SysTick interrupt use to call the call-back function
 **********************************************************************/
void SysTick_Handler(void)
{
    if(g_callBackPtr != NULL )
    {
        (*g_callBackPtr)();
    }

}

/*******************************************************************************
 *                            Functions Definations                             *
 *******************************************************************************/

/*********************************************************************
 * Service Name: SysTick_Init
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to initiate the systick
 **********************************************************************/
void SysTick_Init(uint16_t a_TimeInMilliSeconds){

    SYSTICK_CTRL_REG &= 0;


    if( ( (a_TimeInMilliSeconds * TICKS_PER_MILLISECOND) - 1 ) <= MAX_RELOAD_VALUE)
    {
        SYSTICK_RELOAD_REG = ((a_TimeInMilliSeconds * TICKS_PER_MILLISECOND)-1) ;
    }
    else
    {
        while(1){}
    }
    SET_BIT(SYSTICK_CTRL_REG,SYSTICK_CONTROL_CLK_SRC_BIT);
    SET_BIT(SYSTICK_CTRL_REG,SYSTICK_CONTROL_INTEN_BIT);
}


/*********************************************************************
 * Service Name: SysTick_StartBusyWait
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the SysTick timer with the specified time in milliseconds
 **********************************************************************/
void SysTick_StartBusyWait(uint16_t a_TimeInMilliSeconds)
{
    uint32_t maxMsPerChunk = MAX_RELOAD_VALUE / TICKS_PER_MILLISECOND;

    while(a_TimeInMilliSeconds > 0)
    {
        
        uint32_t currentMs = (a_TimeInMilliSeconds > maxMsPerChunk) ? maxMsPerChunk : a_TimeInMilliSeconds;
        uint32_t ticks = currentMs * TICKS_PER_MILLISECOND - 1;

        
        SYSTICK_CTRL_REG = 0; 
        SYSTICK_RELOAD_REG = ticks;

        SET_BIT(SYSTICK_CTRL_REG, SYSTICK_CONTROL_CLK_SRC_BIT); 
        CLEAR_BIT(SYSTICK_CTRL_REG, SYSTICK_CONTROL_INTEN_BIT);   
        SET_BIT(SYSTICK_CTRL_REG, SYSTICK_CONTROL_ENABLE_BIT);  

        // Wait until COUNTFLAG is set
        while(BIT_IS_CLEAR(SYSTICK_CTRL_REG, SYSTICK_CONTROL_COUNT_FLAG_BIT));

        // Decrement remaining time
        a_TimeInMilliSeconds -= currentMs;

        SysTick_DeInit(); // Optional: reset SysTick
    }

}



/*********************************************************************
 * Service Name: SysTick_SetCallBack
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): Pointer to function
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to setup the SysTick Timer call back to be executed in
SysTick Handler.
 **********************************************************************/
void SysTick_SetCallBack(volatile void (*Ptr2Func) (void))
{
    g_callBackPtr = Ptr2Func;

}

/*********************************************************************
 * Service Name: SysTick_Stop
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Stop the SysTick timer.
 **********************************************************************/
void SysTick_Stop(void)
{

    SET_BIT(SYSTICK_CTRL_REG,SYSTICK_CONTROL_CLK_SRC_BIT);
    SET_BIT(SYSTICK_CTRL_REG,SYSTICK_CONTROL_INTEN_BIT);
    SET_BIT(SYSTICK_CTRL_REG,SYSTICK_CONTROL_ENABLE_BIT);
}

/*********************************************************************
 * Service Name: SysTick_Start
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Start the SysTick timer.
 **********************************************************************/
void SysTick_Start(void){

    SYSTICK_CTRL_REG |= 0X7 ;

}

/*********************************************************************
 * Service Name: SysTick_DeInit
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to De-initialize the SysTick Timer.
 **********************************************************************/
void SysTick_DeInit(void){

    SYSTICK_CTRL_REG    &= 0;
    SYSTICK_CURRENT_REG &= 0;

}
