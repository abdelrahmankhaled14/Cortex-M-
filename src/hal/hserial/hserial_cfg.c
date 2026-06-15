#include "hal/hserial/hserial.h"
#include "mcal/dma/dma.h"

/* ============================================================
 * HSERIAL CHANNEL CONFIGURATIONS
 * ============================================================ */
const HSerial_Config_t HSerial_ConfigTable[HSERIAL_CHANNEL_COUNT] =
    {

        [HSERIAL_UART_1] =
            {
                .channel = HSERIAL_UART_1,
                .periph_type = HSERIAL_PERIPH_UART,
                .mode = HSERIAL_MODE_POLLING,

                .tx_callback = NULL,
                .rx_callback = NULL,
                .dma_rx_callback = NULL,
                .dma_tx_callback = NULL,
                .device_config.uart =
                    {
                        .Uart = UART_1,
                        .Baud_rate = 115200,
                        .Parity_bit = NO_PARTITY,
                        .stop_bit = _1BIT,
                        .mode = DMA,
                    },

            },
        [HSERIAL_SPI_1] =
            {
                .channel = HSERIAL_SPI_1,
                .periph_type = HSERIAL_PERIPH_SPI,
                .mode = HSERIAL_MODE_INTERRUPT,

                .tx_callback = NULL,
                .rx_callback = NULL,
                .dma_rx_callback = NULL,
                .dma_tx_callback = NULL,
                .device_config.spi =
                    {
                        .Spi_Number = SPI_1,

                         .mode = MASTER_SPI,

                         .data_size = _8_BIT,

                         .baud_rate = BAUD_RATE_16,

                         .frame_format = MSB,

                         .clock_polarity = IDLE_ON_LOW, // CPOL = 0

                         .clock_phase = FIRST_EDGE, // CPHA = 0

                         .crc_state = CRC_DISABLED,

                         .transtion_mode = SPI_INTERRUPT,
                    },
            
                }
                
                
};
