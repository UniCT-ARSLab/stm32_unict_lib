/*
 * spi.h
 *
 *  Created on: Jan 10, 2023
 *      Author: corrado
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#ifdef STM32F4
#include <stm32f4xx.h>
#endif

#ifdef STM32F1
#include <stm32f1xx.h>
#endif

#define SPI_SPEED_2		0b000
#define SPI_SPEED_4		0b001
#define SPI_SPEED_8		0b010
#define SPI_SPEED_16	0b011
#define SPI_SPEED_32	0b100
#define SPI_SPEED_64	0b101
#define SPI_SPEED_128	0b110
#define SPI_SPEED_256	0b111

#define SPI_CLOCK_POL_0	0
#define SPI_CLOCK_POL_1	1

#define SPI_CLOCK_PHASE_0	0
#define SPI_CLOCK_PHASE_1	1


void SPI_init(SPI_TypeDef * spi, int speed, int clock_pol, int clock_phase);
void SPI_write(SPI_TypeDef * spi, uint8_t * buf, int len);


#endif /* INC_SPI_H_ */
