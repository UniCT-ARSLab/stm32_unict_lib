/*
 * spi.c
 */

#include "stm32_unict_lib.h"

void SPI_init(SPI_TypeDef * spi, int speed, int clock_pol, int clock_phase)
{
	if (spi == SPI1) {
		// enable SPI1 clock
		// Accoring to system_init, SPI2 works using 84 MHz clock
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if (spi == SPI2) {
		// enable SPI2 clock
		// Accoring to system_init, SPI2 works using 84 MHz clock
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	}

    // SPI Configuration
    // 0 1 0 0 0 0 1 1 0 0 000 1 0 0 = 0100 0011 0000 0100 = 0x4304
    // | | | | | | | | | |  |  | | |
    // | | | | | | | | | |  |  | | +----------- CPHA = 0
    // | | | | | | | | | |  |  | +------------- CPOL = 0
    // | | | | | | | | | |  |  +--------------- SPI Master
    // | | | | | | | | | |  +------------------ Baud Rate fCLK/2
    // | | | | | | | | | +--------------------- SPI Enable (not enabled)
    // | | | | | | | | +----------------------- MSB first
    // | | | | | | | +------------------------- SSI = 1
    // | | | | | | +--------------------------- SSM = 1
    // | | | | | +----------------------------- RXONLY = 0, not receive-only
    // | | | | +------------------------------- DATAFRAME = 0, 8-bit
    // | | | +--------------------------------- CRCNEXT = 0
    // | | +----------------------------------- CRCEN = 0, no crc
    // | +------------------------------------- BIDIOE = 1, output enabled
    // +--------------------------------------- BIDIMODE = 0, unidirectional
    spi->CR1 = 0x4304;
    spi->CR2 = 0;

    spi->CR1 |= (speed << 3);
    spi->CR1 |= (clock_pol << 1);
    spi->CR1 |= (clock_phase);
    spi->CR1 |= SPI_CR1_SPE;

    spi->I2SCFGR = 0;
}

void SPI_write(SPI_TypeDef * spi, uint8_t * buf, int len)
{
	while (len > 0) {
		spi->DR = *buf;
		while ((spi->SR & SPI_SR_TXE) == 0) {}
		++buf;
		--len;
	}
	while ((spi->SR & SPI_SR_BSY) != 0) {}
}
