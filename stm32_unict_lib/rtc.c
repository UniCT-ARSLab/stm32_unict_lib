/*
 * rtc.c
 */

#include "stm32_unict_lib.h"
#include <stdio.h>

// FIXME! NOT COMPLETED!!!

#define RTC_WP_DISABLE() { \
	RTC->WPR = 0xca; \
	RTC->WPR = 0x53; \
}

#define RTC_WP_ENABLE() { \
	RTC->WPR = 0xff; \
}

#define RTC_INIT_MASK           ((uint32_t)0xFFFFFFFFU)

#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00U
#define PWR_CSR_OFFSET           0x04U
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)

/* --- CR Register ---*/
/* Alias word address of DBP bit */
#define DBP_BIT_NUMBER   POSITION_VAL(PWR_CR_DBP)
#define CR_DBP_BB        (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (DBP_BIT_NUMBER * 4U))

/* Alias word address of PVDE bit */
#define PVDE_BIT_NUMBER  POSITION_VAL(PWR_CR_PVDE)
#define CR_PVDE_BB       (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (PVDE_BIT_NUMBER * 4U))

/* Alias word address of PMODE bit */
#define PMODE_BIT_NUMBER  POSITION_VAL(PWR_CR_PMODE)
#define CR_PMODE_BB      (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (PMODE_BIT_NUMBER * 4U))


void RTC_init(void)
{
	*(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;
	//PWR->CR |= PWR_CR_DBP;
	RTC_WP_DISABLE();
	delay_ms(2000);
    // RTC clock
    RCC->BDCR |= RCC_BDCR_RTCEN;
    RCC->BDCR &= ~(RCC_BDCR_RTCSEL);
    RCC->BDCR |= RCC_BDCR_RTCSEL_1; // LSI selected

    RTC->ISR = (uint32_t)RTC_INIT_MASK;
	delay_ms(1);

    /* Clear RTC_CR FMT, OSEL and POL Bits */
    RTC->CR &= ((uint32_t)~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL));
    /* Set RTC_CR register */
    RTC->CR |= (uint32_t)(RTC_HOURFORMAT_24); //| hrtc->Init.OutPut | hrtc->Init.OutPutPolarity);

    /* Configure the RTC PRER */
    RTC->PRER = (uint32_t)(255);
    RTC->PRER |= (uint32_t)(127 << 16U);

    /* Exit Initialization mode */
    RTC->ISR &= (uint32_t)~RTC_ISR_INIT;

    RTC_WP_ENABLE();
	//PWR->CR &= ~(uint32_t)PWR_CR_DBP;
	*(__IO uint32_t *) CR_DBP_BB = (uint32_t)DISABLE;
}

void RTC_gettime(int *ampm, int * hh, int * mm, int * ss)
{
	uint32_t tm = RTC->TR;

	if (ampm != NULL) *ampm = (tm & (1 << 22)) != 0;
	if (hh != NULL) {
		int ht = (tm >> 20) & 3;
		int hu = (tm >> 16) & 0xf;
		*hh = ht * 10 + hu;
	}
	if (mm != NULL) {
		int mt = (tm >> 12) & 7;
		int mu = (tm >> 8) & 0xf;
		*mm = mt * 10 + mu;
	}
	if (ss != NULL) {
		int st = (tm >> 4) & 7;
		int su = tm & 0xf;
		*ss = st * 10 + su;
	}
}
