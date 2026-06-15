/*
 * systick.c
 * Created on: Nov 29, 2024
 * Author: Abdelrahman Khaled
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "std_types.h"



/*******************************************************************************
 *                             PreProcessor Macros                             *
 *******************************************************************************/
#define MAX_RELOAD_VALUE                     0X00FFFFFF
#define TICKS_PER_MILLISECOND                16000
#define SYSTICK_CONTROL_COUNT_FLAG_BIT       16
#define SYSTICK_CONTROL_CLK_SRC_BIT          2
#define SYSTICK_CONTROL_INTEN_BIT            1
#define SYSTICK_CONTROL_ENABLE_BIT           0

/*******************************************************************************
 *                            Functions Prototypes                             *
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
void SysTick_Init(uint16_t a_TimeInMilliSeconds);


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
void SysTick_StartBusyWait(uint16_t a_TimeInMilliSeconds);

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
void SysTick_Handler(void);


/*********************************************************************
 * Service Name: SysTick_SetCallBack
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to setup the SysTick Timer call back to be executed in
SysTick Handler.
 **********************************************************************/
void SysTick_SetCallBack(volatile void (*Ptr2Func) (void));

/*********************************************************************
 * Service Name: SysTick_Stop
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Stop the SysTick timer.
 **********************************************************************/
void SysTick_Stop(void);

/*********************************************************************
 * Service Name: SysTick_Start
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Start the SysTick timer.
 **********************************************************************/
void SysTick_Start(void);

/*********************************************************************
 * Service Name: SysTick_DeInit
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): time in mille second
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to De-initialize the SysTick Timer.
 **********************************************************************/
void SysTick_DeInit(void);










#endif /* SYSTICK_H_ */
