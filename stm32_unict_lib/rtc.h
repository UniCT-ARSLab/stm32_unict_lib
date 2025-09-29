/*
 * rtc.h
 *
 *  Created on: Dec 16, 2018
 *      Author: corrado
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#if defined(STM32F4)
#include <stm32f4xx.h>
#endif

#if defined(STM32L4)
#include <stm32l4xx.h>
#endif

#define RTC_HOURFORMAT_24              ((uint32_t)0x00000000U)
#define RTC_HOURFORMAT_12              ((uint32_t)0x00000040U)


void RTC_init(void);
void RTC_gettime(int *ampm, int * hh, int * mm, int * ss);



#endif /* INC_RTC_H_ */
