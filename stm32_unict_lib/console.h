/*
 * console.h
 */

#ifndef __CONSOLE_H
#define __CONSOLE_H

#ifdef STM32F4
#include <stm32f4xx.h>
#endif

#ifdef STM32F1
#include <stm32f1xx.h>
#endif



void CONSOLE_init(void);
int kbhit(void);
char readchar(void);

#endif
