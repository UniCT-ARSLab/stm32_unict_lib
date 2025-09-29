/*
 * systick.h
 */

#ifndef __SYSTICK_H
#define __SYSTICK_H

void systick_init(void);
void delay_ms(uint32_t ms);
uint32_t get_ticks(void);
void ClockConfig(void);


#endif
