/****************************************************************************
 * @file hserial.h
 * @brief Hardware Serial Layer Interface - Abstraction over UART/SPI with DMA
 * @author Generated
 * @version 2.0
 * @details This layer provides a high-level interface for serial communication
 *          supporting UART and SPI with both interrupt-driven and DMA modes
 ****************************************************************************/

#ifndef HSERIAL_H
#define HSERIAL_H

#include "std_types.h"
#include "mcal/uart/uart.h"
#include "mcal/spi/spi.h"
#include "hal/hserial/hserial_cfg.h"
#include "mcal/dma/dma.h"

/* ==========================================================================
 * TYPE DEFINITIONS
 * ========================================================================== */


typedef enum
{
    HSERIAL_PERIPH_UART = 0,    /**< UART peripheral */
    HSERIAL_PERIPH_SPI,         /**< SPI peripheral (future) */
} HSerial_PeripheralType_t;




typedef enum
{
    HSERIAL_MODE_POLLING = 0,   /**< Polling mode (blocking) */
    HSERIAL_MODE_INTERRUPT,     /**< Interrupt-driven mode */
    HSERIAL_MODE_DMA            /**< DMA mode (recommended for large transfers) */
} HSerial_Mode_t;

typedef enum
{
    HSERIAL_OK = 0,             /**< Operation successful */
    HSERIAL_ERROR,              /**< General error */
    HSERIAL_BUSY,               /**< Channel is busy */
} HSerial_Status_t;


typedef void (*HSerial_AppCallback_t)(void);

typedef union 
{
    Uart_Config_t uart;
    Spi_Config_t  spi;
}device_config_t;


typedef struct
{
    HSerial_Channel_t channel;          /**< Serial channel to configure */
    HSerial_PeripheralType_t periph_type; /**< Peripheral type (UART/SPI) */
    HSerial_Mode_t mode;                /**< Operating mode (polling/interrupt/DMA) */
    HSerial_AppCallback_t tx_callback;  /**< TX complete callback (can be NULL) */
    HSerial_AppCallback_t rx_callback;  /**< RX complete callback (can be NULL) */

    HSerial_AppCallback_t dma_tx_callback;
    HSerial_AppCallback_t dma_rx_callback;

    device_config_t device_config;      /**< Device-specific configuration */
    DMA_Config_t  Dma_tx;
    DMA_Config_t  Dma_rx;

    /*spi*/
} HSerial_Config_t;
/****************************Ptr to Functions *******************************************/

typedef void (*HSerial_TransmitBuffer_t)(
    HSerial_Channel_t channel,
    const uint8_t *buffer,
    uint16_t length
);

typedef void (*HSerial_ReceiveBuffer_t)(
    HSerial_Channel_t channel,
    uint8_t *buffer,
    uint16_t length
);

typedef void (*HSerial_SendByte_t)(
    HSerial_Channel_t channel,
    uint8_t byte
);

typedef void (*HSerial_ReceiveByte_t)(
    HSerial_Channel_t channel,
    uint8_t *byte
);


typedef struct
{
    HSerial_TransmitBuffer_t TransmitBuffer;
    HSerial_ReceiveBuffer_t  ReceiveBuffer;
    HSerial_SendByte_t       SendByte;
    HSerial_ReceiveByte_t    ReceiveByte;
} HSerial_Interface_t;





/* ==========================================================================
 * FUNCTION PROTOTYPES
 * ========================================================================== */


HSerial_Status_t HSerial_Init(HSerial_Channel_t channel, HSerial_Interface_t *interface);




#endif