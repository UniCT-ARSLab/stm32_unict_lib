/*
 * unict_lib.h
 */

#if !defined(STM32F4)
#error Processor not defined, STM32F4 is supported
#endif

#include "gpio.h"
#include "systick.h"
#include "timers.h"
#include "usart.h"
#include "console.h"
#include "display.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "rtc.h"
