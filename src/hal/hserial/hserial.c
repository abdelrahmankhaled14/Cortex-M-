#include "mcal/rcc/rcc.h"
#include "hal/hserial/hserial.h"
#include "mcal/gpio/gpio.h"
#include "mcal/uart/uart.h"
#include "macros.h"
#include "mcal/nvic/nvic.h"
#include "mcal/dma/dma.h"

typedef struct 
{
    uint8_t tx_buffer[1024];
    uint8_t rx_buffer[1024];
    uint16_t tx_count;
    uint16_t rx_count;
    uint16_t tx_length;
    uint16_t rx_length;
    uint8_t in_use;
    uint8_t tx_complete;
    uint8_t rx_complete;
} Channel_data_t;

Channel_data_t UartChannelData[3];
Channel_data_t SpiChannelData[3];


extern const HSerial_Config_t HSerial_ConfigTable[HSERIAL_CHANNEL_COUNT];

/* ============================================================
 * HELPER FUNCTION PROTOTYPES
 * ============================================================ */
static HSerial_Status_t HSERIAL_EnableRccPeripheral(HSerial_Channel_t channel);
static HSerial_Status_t HSERIAL_EnableNvicInterrupt(HSerial_Channel_t channel, HSerial_Mode_t mode);
static HSerial_Status_t HSERIAL_InitPeripheral(HSerial_Channel_t channel);
static HSerial_Status_t HSERIAL_InitDma(HSerial_Channel_t channel);
static void HSERIAL_SPI_IRQHANDLER(spi_t spi);
static void HSERIAL_UART_IRQHANDLER(Uart_t uart);




/* UART – Polling */
static void UART_Polling_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void UART_Polling_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);
static void UART_Polling_SendByte      (HSerial_Channel_t channel, uint8_t byte);
static void UART_Polling_ReceiveByte   (HSerial_Channel_t channel, uint8_t *byte);

/* UART – Interrupt */
static void UART_IT_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void UART_IT_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);


/* UART – DMA */
static void UART_DMA_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void UART_DMA_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);



/* SPI – Polling */
static void SPI_Polling_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void SPI_Polling_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);
static void SPI_Polling_SendByte      (HSerial_Channel_t channel, uint8_t byte);
static void SPI_Polling_ReceiveByte   (HSerial_Channel_t channel, uint8_t *byte);

/* SPI – Interrupt */
static void SPI_IT_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void SPI_IT_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);


/* SPI – DMA */
static void SPI_DMA_TransmitBuffer(HSerial_Channel_t channel, const uint8_t *buffer, uint16_t length);
static void SPI_DMA_ReceiveBuffer (HSerial_Channel_t channel, uint8_t *buffer, uint16_t length);



/* ============================================================
 * MAIN INIT FUNCTION
 * ============================================================ */
HSerial_Status_t HSerial_Init(HSerial_Channel_t channel, HSerial_Interface_t *interface)
{
    HSerial_Status_t state = HSERIAL_OK;
    const HSerial_Config_t *config = &HSerial_ConfigTable[channel];
    
    /* Validate channel */
    if (channel >= HSERIAL_CHANNEL_COUNT)
    {
        return HSERIAL_ERROR;
    }
    
    /* Step 1: Enable RCC for peripheral */
    state = HSERIAL_EnableRccPeripheral(channel);
    if (state != HSERIAL_OK)
    {
        return state;
    }
    
    /* Step 2: Enable NVIC interrupts based on mode */
    state = HSERIAL_EnableNvicInterrupt(channel, config->mode);
    if (state != HSERIAL_OK)
    {
        return state;
    }
    
    /* Step 3: Initialize DMA if mode is DMA */
    if (config->mode == HSERIAL_MODE_DMA)
    {
        state = HSERIAL_InitDma(channel);
        if (state != HSERIAL_OK)
        {
            return state;
        }
    }
    
    /* Step 4: Initialize the peripheral (UART/SPI/I2C) */
    state = HSERIAL_InitPeripheral(channel);
    if (state != HSERIAL_OK)
    {
        //return state;
    }
    /* Step 5: Set up interface function pointers */
    if (config->periph_type == HSERIAL_PERIPH_UART)
    {
        /* Set function pointers based on mode */
        switch (config->mode)
        {
            case HSERIAL_MODE_POLLING:
                interface->TransmitBuffer = UART_Polling_TransmitBuffer;
                interface->ReceiveBuffer  = UART_Polling_ReceiveBuffer;
                interface->SendByte       = UART_Polling_SendByte;
                interface->ReceiveByte    = UART_Polling_ReceiveByte;
                break;
            case HSERIAL_MODE_INTERRUPT:   
                UART_SetCallback(channel,HSERIAL_UART_IRQHANDLER); 
                interface->TransmitBuffer = UART_IT_TransmitBuffer;
                interface->ReceiveBuffer  = UART_IT_ReceiveBuffer;
                interface->SendByte       = NULL; /* Implement if needed */
                interface->ReceiveByte    = NULL; /* Implement if needed */
                break;
            case HSERIAL_MODE_DMA:
                interface->TransmitBuffer = UART_DMA_TransmitBuffer; /* Implement if needed */
                interface->ReceiveBuffer  = UART_DMA_ReceiveBuffer; /* Implement if needed */
                interface->SendByte       = NULL; /* Implement if needed */
                interface->ReceiveByte    = NULL; /* Implement if needed */
            default:
                return HSERIAL_ERROR;
        }
    }
    else if (config->periph_type == HSERIAL_PERIPH_SPI)
    {
        switch (config->mode)
        {
        case HSERIAL_MODE_POLLING:
            interface->TransmitBuffer = SPI_Polling_TransmitBuffer; 
            interface->ReceiveBuffer  = SPI_Polling_ReceiveBuffer; 
            interface->SendByte       = SPI_Polling_SendByte;
            interface->ReceiveByte    = SPI_Polling_ReceiveByte;
            break;
        case HSERIAL_MODE_INTERRUPT:
            SPI_SetCallback(channel - 3,HSERIAL_SPI_IRQHANDLER);
            interface->TransmitBuffer = SPI_IT_TransmitBuffer;
            interface->ReceiveBuffer  = SPI_IT_ReceiveBuffer;
            interface->SendByte       = NULL; /* Implement if needed */
            interface->ReceiveByte    = NULL; /* Implement if needed */
            break;
        case HSERIAL_MODE_DMA:
            interface->TransmitBuffer = SPI_DMA_TransmitBuffer;
            interface->ReceiveBuffer  = SPI_DMA_ReceiveBuffer;
            interface->SendByte       = NULL; /* Implement if needed */
            interface->ReceiveByte    = NULL; /* Implement if needed */
            break;
        default:
            break;
        }
    }
    
    return HSERIAL_OK;
}

/* ============================================================
 * HELPER FUNCTION: Enable RCC Peripheral Clock
 * ============================================================ */
static HSerial_Status_t HSERIAL_EnableRccPeripheral(HSerial_Channel_t channel)
{
    switch (channel)
    {
        case HSERIAL_UART_1:
            rcc_clk_perpheral_enable(APB2_BUS, USART1_EN);
            break;
        case HSERIAL_UART_2:
            rcc_clk_perpheral_enable(APB1_BUS, USART2_EN);
            break;
        case HSERIAL_UART_6:
            rcc_clk_perpheral_enable(APB2_BUS, USART6_EN);
            break;
        case HSERIAL_SPI_1:
            rcc_clk_perpheral_enable(APB2_BUS, SPI1_EN);
            break;
        case HSERIAL_SPI_2:
            rcc_clk_perpheral_enable(APB1_BUS, SPI2_EN);
            break;
        case HSERIAL_SPI_3:
            rcc_clk_perpheral_enable(APB1_BUS, SPI3_EN);
            break;
        default:
            return HSERIAL_ERROR;
    }
    return HSERIAL_OK;
}

/* ============================================================
 * HELPER FUNCTION: Enable NVIC Interrupts
 * ============================================================ */
static HSerial_Status_t HSERIAL_EnableNvicInterrupt(HSerial_Channel_t channel, HSerial_Mode_t mode)
{
    switch (mode)
    {
        case HSERIAL_MODE_POLLING:
            /* No interrupt needed */
            break;
            
        case HSERIAL_MODE_INTERRUPT:
            /* Enable peripheral interrupt */
            switch (channel)
            {
                case HSERIAL_UART_1:
                    NVIC_EnableIRQ(USART1_IRQn);
                    break;
                case HSERIAL_UART_2:
                    NVIC_EnableIRQ(USART2_IRQn);
                    break;
                case HSERIAL_UART_6:
                    NVIC_EnableIRQ(USART6_IRQn);
                    break;
                case HSERIAL_SPI_1:
                    NVIC_EnableIRQ(SPI1_IRQn);
                    break;
                    
                case HSERIAL_SPI_2:
                    NVIC_EnableIRQ(SPI2_IRQn);
                    break;
                case HSERIAL_SPI_3:
                    NVIC_EnableIRQ(SPI3_IRQn);
                    break;
                default:
                    return HSERIAL_ERROR;
            }
            break;
            
        case HSERIAL_MODE_DMA:
            /* Enable DMA clock */
            rcc_clk_perpheral_enable(AHB1_BUS, DMA1_EN);
            rcc_clk_perpheral_enable(AHB1_BUS, DMA2_EN);
            
            /* Enable DMA interrupts based on channel configuration */
            const HSerial_Config_t *config = &HSerial_ConfigTable[channel];
            
            /* Enable DMA TX stream interrupt */
            switch (config->Dma_tx.Request)
            {
                case DMA_USART1_TX_DMA2_STREAM7_CHANNEL4:
                    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
                    break;
                case DMA_USART2_TX_DMA1_STREAM6_CHANNEL4:
                    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
                    break;
                case DMA_USART6_TX_DMA2_STREAM6_CHANNEL5:
                    NVIC_EnableIRQ(DMA2_Stream6_IRQn);
                    break;
                case DMA_SPI1_TX_DMA2_STREAM5_CHANNEL3:
                    NVIC_EnableIRQ(DMA2_Stream5_IRQn);
                    break;
                case DMA_SPI2_TX_DMA1_STREAM4_CHANNEL0:
                    NVIC_EnableIRQ(DMA1_Stream4_IRQn);
                    break;
                case DMA_SPI3_TX_DMA1_STREAM5_CHANNEL0:
                    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
                    break;
                /* Add more cases based on your DMA request mappings */
                default:
                    break;
            }
            
            /* Enable DMA RX stream interrupt */
            switch (config->Dma_rx.Request)
            {
                case DMA_USART1_RX_DMA2_STREAM2_CHANNEL4:
                    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
                    break;
                case DMA_USART2_RX_DMA1_STREAM5_CHANNEL4:
                    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
                    break;
                case DMA_USART6_RX_DMA2_STREAM1_CHANNEL5:
                    NVIC_EnableIRQ(DMA2_Stream1_IRQn);
                    break;
                case DMA_SPI1_RX_DMA2_STREAM2_CHANNEL3:
                    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
                    break;
                case DMA_SPI2_RX_DMA1_STREAM3_CHANNEL0:
                    NVIC_EnableIRQ(DMA1_Stream3_IRQn);
                    break;
                case DMA_SPI3_RX_DMA1_STREAM2_CHANNEL0:
                    NVIC_EnableIRQ(DMA1_Stream2_IRQn);
                    break;
                /* Add more cases based on your DMA request mappings */
                default:
                    break;
            }
            break;
            
        default:
            return HSERIAL_ERROR;
    }
    
    return HSERIAL_OK;
}

/* ============================================================
 * HELPER FUNCTION: Initialize DMA
 * ============================================================ */
static HSerial_Status_t HSERIAL_InitDma(HSerial_Channel_t channel)
{
    const HSerial_Config_t *config = &HSerial_ConfigTable[channel];
    DMA_Status_t dma_state;
    
    /* Initialize DMA TX */
    DMA_Init(&config->Dma_tx);
    DMA_Init(&config->Dma_rx);
    
    return HSERIAL_OK;
}

/* ============================================================
 * HELPER FUNCTION: Initialize Peripheral (UART/SPI/I2C)
 * ============================================================ */
static HSerial_Status_t HSERIAL_InitPeripheral(HSerial_Channel_t channel)
{
    const HSerial_Config_t *config = &HSerial_ConfigTable[channel];
    
    switch (config->periph_type)
    {
        case HSERIAL_PERIPH_UART:
        { 
             UART_Init(&config->device_config.uart);
            break;
        }
        
        case HSERIAL_PERIPH_SPI:
        {
            SPI_Init(&config->device_config.spi);
            break;
        }
        default:
            return HSERIAL_ERROR;
    }
    
    return HSERIAL_OK;
}

/* ============================================================
 * TRANSMIT AND RECEIVE FUNCTIONS FOR UART
 * ============================================================ */

/* ================= POLLING ================= */


static void UART_Polling_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        UART_Polling_SendByte(channel, buffer[i]);
    }
}

static void UART_Polling_ReceiveBuffer(HSerial_Channel_t channel,uint8_t *buffer,uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        UART_Polling_ReceiveByte(channel, &buffer[i]);
    }
}

static void UART_Polling_SendByte(HSerial_Channel_t channel, uint8_t byte)
{
        while (UART_ReadTXEmptyFlag(channel) == 0)
        {
            /* Wait until transmit data register is empty */
        }
        UART_WriteDR(channel, byte);
        while (UART_ReadTransmitCompleteFlag(channel) == 0)
        {   
            /* Wait until transmission is complete */
        }

}

static void UART_Polling_ReceiveByte(HSerial_Channel_t channel,uint8_t *byte)
{
    while (UART_ReadRXNotEmptyFlag(channel) == 0)
    {
        /* Wait until data is received */
    }
    *byte = UART_ReadDR(channel);
}

/* ================= INTERRUPT ================= */

static void UART_IT_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)    
{

}

static void UART_IT_ReceiveBuffer(HSerial_Channel_t channel,uint8_t *buffer,uint16_t length)
{

}

/* ================= DMA ================= */

static void UART_DMA_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)
{

}

static void UART_DMA_ReceiveBuffer(HSerial_Channel_t channel,uint8_t *buffer,uint16_t length)
{

}


/* ==========================================================================
 * SPI TRANSMIT AND RECEIVE FUNCTIONS
 * ========================================================================== */
void SPI_Polling_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        SPI_Polling_SendByte(channel, buffer[i]);
    }
}

void SPI_Polling_ReceiveBuffer(HSerial_Channel_t channel,uint8_t *buffer,uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        SPI_Polling_ReceiveByte(channel, &buffer[i]);
    }
}

void SPI_Polling_SendByte(HSerial_Channel_t channel, uint8_t byte)
{
    volatile uint8_t dummy;
    channel -= 3;

    /* Wait until TX buffer empty */
    while (SPI_IsTransmitBufferEmpty(channel) == 0);

    /* Send byte */
    SPI_SendData(channel, byte);

    /* Wait until RX buffer NOT empty */
    while (SPI_IsReceiveBufferNotEmpty(channel) == 0)
    {
        dummy = SPI_ReceiveData(channel);
    }

    /* Read received data to clear RXNE */
    (void)dummy;
}


void SPI_Polling_ReceiveByte(HSerial_Channel_t channel, uint8_t *byte)
{
    channel = channel - 3;
    /* Wait until transmit buffer is empty */
    while(SPI_IsTransmitBufferEmpty(channel) == 0)
    {
        /* Wait */
    }
    
    /* Send dummy byte (0x00) to generate clock */
    SPI_SendData(channel, 0x00);
    
    /* Wait until data is received */
    while (SPI_IsReceiveBufferNotEmpty(channel) == 0)
    {
        /* Wait */
    }
    
    /* Store received data in the pointer */
    *byte = SPI_ReceiveData(channel);
}

/* ================= INTERRUPT ================= */

void SPI_IT_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)
{
    int spi = channel - 3;

    if (length == 0 || buffer == NULL)
    {
        return;
    }
    
    if (SpiChannelData[spi].in_use == 1)
    {
        return; /* Channel busy */
    }
    
    /* Copy buffer to internal buffer */
    for (uint16_t i = 0; i < length; i++)
    {
        SpiChannelData[spi].tx_buffer[i] = buffer[i];
    }
    
    SpiChannelData[spi].tx_length = length;
    SpiChannelData[spi].tx_count = 0;
    SpiChannelData[spi].rx_count = 0;
    SpiChannelData[spi].in_use = 1;
    SpiChannelData[spi].tx_complete = 0;
    
    /* Enable both TXE and RXNE interrupts */
    SPI_SendData(spi, SpiChannelData[spi].tx_buffer[0]);
    SPI_EnableTxInterrupt(spi);
    SPI_EnableRxInterrupt(spi);
    
    /* Send first byte to start the transmission */
    SpiChannelData[spi].tx_count = 1;


}

void SPI_IT_ReceiveBuffer( HSerial_Channel_t channel, uint8_t *buffer, uint16_t length)
{
        channel -= 3;
    
    if (length == 0 || buffer == NULL)
    {
        return;
    }
    
    if (SpiChannelData[channel].in_use == 1)
    {
        return; /* Channel busy */
    }
    
    SpiChannelData[channel].rx_length = length;
    SpiChannelData[channel].tx_count = 0;
    SpiChannelData[channel].rx_count = 0;
    SpiChannelData[channel].in_use = 1;
    SpiChannelData[channel].rx_complete = 0;
    
    /* Enable both TXE and RXNE interrupts */
    SPI_EnableTxInterrupt(channel);
    SPI_EnableRxInterrupt(channel);
    
    /* Send first dummy byte to generate clock */
    SPI_SendData(channel, 0x00);
    SpiChannelData[channel].tx_count = 1;
}

/* ================= DMA ================= */

void SPI_DMA_TransmitBuffer(HSerial_Channel_t channel,const uint8_t *buffer,uint16_t length)
{

}

void SPI_DMA_ReceiveBuffer(HSerial_Channel_t channel,uint8_t *buffer,uint16_t length)
{

}



/******************************IRQ**************************************/
void HSERIAL_UART_IRQHANDLER(Uart_t uart)
{
    switch (uart)
    {
        case UART_1:
            break;
        case UART_2:
            break;
        case UART_6:
            break;
        default:
            break;
    }
}

void HSERIAL_SPI_IRQHANDLER(spi_t spi)
{
    /* IMPORTANT: Check RXNE first to avoid overrun errors */
    if (SPI_IsRxInterruptEnabled(spi) && SPI_IsReceiveBufferNotEmpty(spi))
    {
        if (SpiChannelData[spi].rx_count < SpiChannelData[spi].rx_length)
        {
            /* Store received byte */
            SpiChannelData[spi].rx_buffer[SpiChannelData[spi].rx_count] = SPI_ReceiveData(spi);
            SpiChannelData[spi].rx_count++;
            
            /* Check if all bytes received */
            if (SpiChannelData[spi].rx_count >= SpiChannelData[spi].rx_length)
            {
                /* Disable RXNE interrupt */
                SPI_DisableRxInterrupt(spi);
                SpiChannelData[spi].rx_complete = 1;
                
                /* If TX is also complete, mark channel as free */
                if (SpiChannelData[spi].tx_complete)
                {
                    SpiChannelData[spi].in_use = 0;
                }
            }
        }
        else
        {
            /* Buffer overflow protection - read and discard */
            volatile uint8_t dummy = SPI_ReceiveData(spi);
            (void)dummy;
        }
    }
    
    /* Check TXE after RXNE */
    if (SPI_IsTransmitBufferEmpty(spi) && SPI_IsTxInterruptEnabled(spi) )
    {
        if (SpiChannelData[spi].tx_count < SpiChannelData[spi].tx_length)
        {
            /* Send next byte (or dummy for RX-only mode) */
            if (SpiChannelData[spi].tx_length > 0)
            {
                SPI_SendData(spi, SpiChannelData[spi].tx_buffer[SpiChannelData[spi].tx_count]);
            }
            else
            {
                /* RX-only mode: send dummy byte to generate clock */
                SPI_SendData(spi, 0x00);
            }
            SpiChannelData[spi].tx_count++;
        }
        else
        {
            /* All bytes sent, disable TXE interrupt */
            SPI_DisableTxInterrupt(spi);
            SpiChannelData[spi].tx_complete = 1;
            
            /* If RX is also complete, mark channel as free */
            if (SpiChannelData[spi].rx_complete || SpiChannelData[spi].rx_length == 0)
            {
                SPI_DisableRxInterrupt(spi);
                SpiChannelData[spi].in_use = 0;
            }
        }
    }
}




static void HSERIAL_DMA_IRQHANDLER(uint8_t Dma, uint8_t Stream)
{

    if (DMA_ReadTransferCompleteFlag(Dma, Stream))
    {
        DMA_ClearTransferCompleteFlag(Dma, Stream);
    }
    if (DMA_ReadTransferErrorFlag(Dma, Stream))
    {
        DMA_ClearTransferErrorFlag(Dma, Stream);
    }
    if (DMA_ReadHalfTransferCompleteFlag(Dma, Stream))
    {
        DMA_ClearHalfTransferCompleteFlag(Dma, Stream);
    }

    if (DMA_ReadDirectModeErrorFlag(Dma, Stream))
    {
        DMA_ClearDirectModeErrorFlag(Dma, Stream);
    }
    if (DMA_ReadFIFOErrorFlag(Dma, Stream))
    {
        DMA_ClearFIFOErrorFlag(Dma, Stream);
    }
}

/*********IS BUSY***********/

static void __is_identifierUartBusy(HSerial_Channel_t channel)
{
    return UartChannelData[channel].in_use;
}
static void __is_identifierSpiBusy(HSerial_Channel_t channel)
{
    channel-=3;
    return SpiChannelData[channel].in_use;
}






/* ==========================================================================
 * SPI INTERRUPT HANDLER - CRITICAL IMPLEMENTATION
 * ========================================================================== */





