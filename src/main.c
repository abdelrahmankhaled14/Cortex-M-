

// extern LED_Configuration_t LED_Config[LED_MAX];
// extern Lcd_config_t lcd1[LCD_MAX];
// extern SWITCH_Configuration_t SWITCH_Config[SWITCH_MAX];

// void Toggle_LED(void);
// // Runnable_t LED_Runnable_1 =
// // {
// //     .callback = Toggle_LED,
// //     .first_delay_ms = 0,
// //     .period_ms = 1,
// //     .priority = 0,
// //     .arg = (void*)LED_2,
// // };

// // Runnable_t LED_Runnable_2 =
// // {
// //     .callback = Toggle_LED,
// //     .first_delay_ms = 0,
// //     .period_ms = 2000,
// //     .priority = 1,
// //     .arg = LED_2,
// // };
// // Runnable_t LED_Runnable_3 =
// // {
// //     .callback = Toggle_LED,
// //     .first_delay_ms = 0,
// //     .period_ms = 2000,
// //     .priority = 2,
// //     .arg = LED_3,
// // };

// uint8_t heart2[8] =
// {
//     0b00110011,
//     0b00110011,
//     0b00110011,
//     0b00110011,
//     0b00110011,
//     0b00110011,
//     0b00110011,
//     0b00110011,
// };

// void Toggle_LED(void)
// {
//         // LED_Names_t led = (LED_Names_t)(uintptr_t)arg;
//         // Switch_State_t state;
//         // SWITCH_GetState(SWITCH_1,&state);
//         // if(state == SWITCH_PRESSED)
//         // {
//              LED_Toggle(1);
//         // }
//         // else
//         // {
//         //     LED_Off(led);
//         // }
//       //LCD_Async_Write_String(&lcd1[LCD_1],"ererrerer");

//         //LCD_Async_Set_Cursor_postion(&lcd1[LCD_1], ROW2, 1);
//         //LCD_Async_SendCommand(&lcd1[LCD_1],LCD_CLEAR);

//        // LCD_Async_Write_String(&lcd1[LCD_1],"111111111111  11111111111");

// }
// extern const SevenSeg_Config_t SevenSeg_Config[SEVENSEG_MAX];

// extern LedMatrix_Config_t LedMatrix_Config[LED_MATRIX_MAX];
// int main(void)
// {
//     rcc_clk_perpheral_enable(AHB1_BUS, GPIOA_EN);
//     rcc_clk_perpheral_enable(AHB1_BUS, GPIOB_EN);
//     rcc_clk_perpheral_enable(AHB1_BUS ,GPIOC_EN);
//     NVIC_EnableIRQ(USART1_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream0_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream1_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream2_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream3_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream4_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream5_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream6_IRQn);

//     NVIC_EnableIRQ(DMA2_Stream0_IRQn);

//     // // LCD_init(&lcd1[LCD_1]);
//     // Uart_Config_t Uart_Config =
//     // {
//     //   .Baud_rate = 9600,
//     //   .mode = INTERRUPT,
//     //   .Over_sampling = _16_OVERSAMPLING,
//     //   .Parity_bit = NO_PARTITY,
//     //   .stop_bit = _1BIT,
//     //   .Uart = UART_1,
//     //   .Rx_Callback = Toggle_LED,
//     //   .Tx_Callback = Toggle_LED
//     // };
//     // UART_Init(&Uart_Config);

//     // LED_Init(LED_Config);
//     // SWITCH_Init(SWITCH_Config, SWITCH_MAX);
//     // LedMatrix_Display(LED_MATRIX_1,heart2);
//     // LedMatrix_Init(&LedMatrix_Config[0]);
//     // Sched_Init(1);
//     // Sched_RegisterRunnable(&LED_Runnable_1);
//     // // // Sched_RegisterRunnable(&LED_Runnable_2);
//     // // // Sched_RegisterRunnable(&LED_Runnable_3);
//      __asm volatile ("cpsie i");
//     //uint8_t* buffer = "abcdefghijklmnopqursxyz";
//     //UART_SendBufferWithInturrupt(&Uart_Config , buffer,5);
//     // // //LCD_Async_init(&lcd1[LCD_1]);
//     //  Sched_Start();
//     // // //SysTick_StartBusyWait(10);
//     //        uint8_t* buffer;
//     // UART_ReadBufferWithInturrupt(&Uart_Config , &buffer,4);

//     // while(1)
//     // {

//     // // UART_SendBufferWithInturrupt(&Uart_Config , buffer,28);

//     //     // GPIO_SetPinValue(LedMatrix_Config[0].col_port[0], LedMatrix_Config[0].col_pins[0], LOW);
//     //     // GPIO_SetPinValue(LedMatrix_Config[0].col_port[1], LedMatrix_Config[0].col_pins[1], LOW);
//     //     // GPIO_SetPinValue(LedMatrix_Config[0].col_port[2], LedMatrix_Config[0].col_pins[2], LOW);
//     //     // GPIO_SetPinValue(LedMatrix_Config[0].col_port[3], LedMatrix_Config[0].col_pins[3], LOW);
//     //     // GPIO_SetPinValue(LedMatrix_Config[0].col_port[5], LedMatrix_Config[0].col_pins[5], LOW);

//     // }

// }

//     //LCD_Write_String(&lcd1[LCD_1],"hell0");

//         //LCD_SendCommand(&lcd1[LCD_1],0x1C);
//         //SysTick_StartBusyWait(1000);
//         //LCD_SendCommand(&lcd1[LCD_1],0x1);

//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_0);
//         //         SysTick_StartBusyWait(1000);
//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_1);
//         //         SysTick_StartBusyWait(1000);
//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_3);
//         //         SysTick_StartBusyWait(1000);
//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_4);
//         //         SysTick_StartBusyWait(1000);
//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_5);
//         //         SysTick_StartBusyWait(1000);
//         // SEVENSEG_Display(SEVENSEG_1 , SEVEN_SEG_6);
//         //         SysTick_StartBusyWait(1000);

// // int main(void)
// // {
// //     rcc_clk_perpheral_enable(AHB1_BUS, GPIOB_EN);

// //     LCD_init(&lcd1[LCD_1]);

// //     SysTick_StartBusyWait(100);  // Wait 100ms after init

// //     LCD_Write_String(&lcd1[LCD_1], "HELLO");

// //     while(1);
// // }

// /*
// #define DELAY 500000
// extern LED_Configuration_t LED_Config[LED_MAX];
// extern SWITCH_Configuration_t SWITCH_Config[SWITCH_MAX];

// void func (void);
// int main(void)
// {
//     rcc_init();
//     rcc_clk_perpheral_enable(AHB1_BUS, GPIOA_EN);
//     LED_Init(LED_Config, LED_MAX);
//     SWITCH_Init(SWITCH_Config, SWITCH_MAX);
//     // SysTick_SetCallBack(func);
//     // SysTick_Init(1000);
//     NVIC_EnableIRQ(WWDG_IRQn);
//     NVIC_SetPendingIRQ(WWDG_IRQn);

//     __asm volatile ("cpsie i");   // Clear PRIMASK → enable all interrupts

//     while (1)
//     {
//         //SysTick_Start();
//       uint8_t switch_state;
//         SWITCH_GetState(SWITCH_1, &switch_state);
//         if(switch_state == SWITCH_PRESSED)
//         {
//             for (volatile int i = 0; i < 100; i++);
//             if (switch_state == SWITCH_PRESSED)
//             {
//                             LED_On(LED_1);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_1);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_2);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_2);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_3);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_3);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_4);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_4);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_5);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_5);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_6);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_6);
//                             SysTick_StartBusyWait(1000);

//             }
//         }
//     }
// }

// void func (void)
// {
//                             LED_On(LED_1);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_1);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_2);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_2);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_3);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_3);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_4);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_4);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_5);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_5);
//                             SysTick_StartBusyWait(1000);
//                             LED_On(LED_6);
//                             SysTick_StartBusyWait(1000);
//                             LED_Off(LED_6);
//                             SysTick_StartBusyWait(1000);
// }

// void WWDG_IRQHandler(void)

// {
//     LED_On(LED_6);
//     SysTick_StartBusyWait(1000);
//     LED_Off(LED_6);
//     SysTick_StartBusyWait(1000);
// }
// */

// #define BUFFER_SIZE 16

// uint32_t srcBuffer[BUFFER_SIZE];
// uint32_t dstBuffer[BUFFER_SIZE];

// volatile uint8_t dmaDone = 0;

// /* Optional callback */
// void DMA_TC_Callback(void)
// {
//     dmaDone = 1;
// }

// int main(void)
// {
//     NVIC_EnableIRQ(USART1_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream0_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream1_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream2_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream3_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream4_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream5_IRQn);
//     NVIC_EnableIRQ(DMA1_Stream6_IRQn);

//     NVIC_EnableIRQ(DMA2_Stream0_IRQn);
//     NVIC_EnableIRQ(DMA2_Stream1_IRQn);
//     __asm volatile ("cpsie i");

//     /* Fill source buffer with test pattern */
//     for (uint32_t i = 0; i < BUFFER_SIZE; i++)
//     {
//         srcBuffer[i] = 0xA5A50000 + i;
//         dstBuffer[i] = 0x00000000;
//     }

//     DMA_Config_t dmaConfig =
//     {
//         .Direction               = DMA_DIR_MEM_TO_MEM,
//         .MemoryDataWidth         = DMA_DATA_WIDTH_32BIT,
//         .PeripheralDataWidth     = DMA_DATA_WIDTH_32BIT,
//         .MemoryInc               = DMA_INC_ENABLE,
//         .PeripheralInc           = DMA_INC_ENABLE,
//         .Priority                = DMA_PRIORITY_HIGH,
//         .Mode                    = DMA_MODE_NORMAL,
//         .MemoryAddress           = dstBuffer,
//         .PeripheralAddress       = srcBuffer,
//         .DataLength              = BUFFER_SIZE,
//         .Request                 = DMA_MEM2MEM_DMA2_STREAM1_CHANNEL1,
//         .TransferCompleteCallback = DMA_TC_Callback
//     };

//     DMA_Init(&dmaConfig);
//     DMA_Start(&dmaConfig);

//     /* Polling method (simple test) */

//     /* Or interrupt-based */
//     // while (!dmaDone);

//     /* At this point:
//        dstBuffer[i] MUST == srcBuffer[i]
//     */

//     while (1)
//     {
//         /* Place breakpoint here and inspect buffers */
//     }
// }

#include "mcal/rcc/rcc.h"
#include "hal/led/led.h"
#include "hal/switch/switch.h"
#include "mcal/gpio/gpio.h"
#include "mcal/systic/systick.h"
#include "mcal/nvic/nvic.h"
#include "hal/lcd/lcd.h"
#include "hal/sevensegment/sevensegment.h"
#include "hal/ledmatrix/ledmatrix.h"
#include "mcal/uart/uart.h"
#include "mcal/dma/dma.h"
#include "OS/sched.h"
#include "hal/hserial/hserial.h"

HSerial_Interface_t serial_interface;
HSerial_Interface_t serial_interface2;
int main(void)
{
    rcc_clk_perpheral_enable(AHB1_BUS, GPIOA_EN);
    GPIO_Configuration_t gpio_cfg;
    gpio_cfg.port = PORTA;
    gpio_cfg.pin = PIN5;
    gpio_cfg.direction = ALTERNATE_FUNCTION;
    gpio_cfg.mode = AF_5;
    gpio_cfg.output_type = PUSH_PULL; // ADD THIS
    gpio_cfg.speed = HIGH_SPEED;      // ADD THIS
    GPIO_Init(&gpio_cfg, 1);

    // PA6 (MISO)
    gpio_cfg.port = PORTA;
    gpio_cfg.pin = PIN6;
    gpio_cfg.direction = ALTERNATE_FUNCTION;
    gpio_cfg.mode = AF_5;
    gpio_cfg.output_type = INPUT; // INPUT for MISO
    gpio_cfg.speed = HIGH_SPEED;
    GPIO_Init(&gpio_cfg, 1);

    // PA7 (MOSI)
    gpio_cfg.port = PORTA;
    gpio_cfg.pin = PIN7;
    gpio_cfg.direction = ALTERNATE_FUNCTION;
    gpio_cfg.mode = AF_5;
    gpio_cfg.output_type = PUSH_PULL; // OUTPUT
    gpio_cfg.speed = HIGH_SPEED;
    GPIO_Init(&gpio_cfg, 1);

   // HSerial_Init(HSERIAL_UART_1, &serial_interface);
    HSerial_Init(HSERIAL_SPI_1, &serial_interface2);
    char buffer[9];
    //serial_interface.TransmitBuffer(HSERIAL_UART_1, (uint8_t *)"Hello, salah! I love you so much!\t", 45);
    uint8_t buffer2[10] = "BBBBBBBB";
    //serial_interface2.ReceiveBuffer(HSERIAL_SPI_1, &buffer,8);

    while (1)
    {
    serial_interface2.TransmitBuffer(HSERIAL_SPI_1, (uint8_t *)"hhhh", 5);
    }
}
// /* ================= Buffers ================= */
// uint8_t TxBuffer[] = " Hello, i love you !";

// /* ================= Callbacks ================= */
// void DMA_TX_Done(void)
// {
//     /* DMA finished sending data to USART1 */
//       /* put breakpoint here */
// }

// /* ================= UART Config ================= */
// Uart_Config_t uart1_cfg =
// {
//     .Uart          = UART_1,
//     .Baud_rate     = 115200,
//     .Parity_bit    = NO_PARTITY,
//     .stop_bit      = _1BIT,
//     .mode          = DMA,

// };

// /* ================= DMA Config ================= */
// DMA_Config_t dma_uart1_tx_cfg =
// {
//     .Direction               = DMA_DIR_MEM_TO_PERIPH,
//     .MemoryDataWidth         = DMA_DATA_WIDTH_8BIT,
//     .PeripheralDataWidth     = DMA_DATA_WIDTH_8BIT,
//     .MemoryInc               = DMA_INC_ENABLE,
//     .PeripheralInc           = DMA_INC_DISABLE,
//     .Priority                = DMA_PRIORITY_HIGH,
//     .Mode                    = DMA_MODE_NORMAL,

//     .MemoryAddress           = (uint32_t *)TxBuffer,
//     .PeripheralAddress       = ((uint32_t *)0x40011004), // USART1_DR address
//     .DataLength              = sizeof(TxBuffer) - 1,
//     .FlowController          = DMA_FLOW_CTRL_DMA,
//     .Request                 = DMA_USART1_TX_DMA2_STREAM7_CHANNEL4,
//     .TransferCompleteCallback = DMA_TX_Done
// };

// int main(void)
// {
//     NVIC_EnableIRQ(USART1_IRQn);
//     NVIC_EnableIRQ(DMA2_Stream7_IRQn);

//     /* ================= Init UART ================= */
//     UART_Init(&uart1_cfg);

//     /* ================= Enable USART DMA TX ================= */

//     /* ================= Init DMA ================= */
//     DMA_Init(&dma_uart1_tx_cfg);

//     /* ================= Start DMA ================= */
//     DMA_Start(&dma_uart1_tx_cfg);

//     while (1)
//     {
//            // DMA_Start(&dma_uart1_tx_cfg);
//     }
// }
