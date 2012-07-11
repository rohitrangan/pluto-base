/*Created By - Rohit Rangan
 *Created On - 06-07-2012
 *
 *This file contains all the functions to initialize
 *the ICU driver.
 */

#include "pluto.h"

#if HAL_USE_ICU
icucnt_t last_width[6], last_period[6] ;
#if STM32_ICU_USE_TIM1
static void icuwidthcb1(ICUDriver *icup) {
	last_width[0] = icuGetWidth(icup) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "width:- %d", last_width[0]) ;
}

static void icuperiodcb1(ICUDriver *icup) {
	last_period[0] = icuGetPeriod(icup) ;
}
/*icucfg1 - To be used for ICU1 */
static ICUConfig icucfg1 = {
	ICU_INPUT_ACTIVE_HIGH,
	ICU_CLK_FREQUENCY,
	icuwidthcb1,
	icuperiodcb1,
	NULL,
	ICU_CHANNEL_1
};
#endif	/*STM32_ICU_USE_TIM1 */

#if	STM32_ICU_USE_TIM3
static void icuwidthcb3(ICUDriver *icup) {
	last_width[1] = icuGetWidth(icup) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "width:- %d\r\n", last_width[1]) ;
	palSetPad(GPIOC, 4) ;
}

static void icuperiodcb3(ICUDriver *icup) {
	last_period[1] = icuGetPeriod(icup) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "period:- %d\r\n", last_period[1]) ;
	palClearPad(GPIOC, 4) ;
}
/*icucfg3 - To be used for ICU3 */
static ICUConfig icucfg3 = {
	ICU_INPUT_ACTIVE_HIGH,
	ICU_CLK_FREQUENCY,
	icuwidthcb3,
	icuperiodcb3,
	NULL,
	ICU_CHANNEL_1
};
#endif	/*STM32_ICU_USE_TIM3 */

#if STM32_ICU_USE_TIM8
static void icuwidthcb8(ICUDriver *icup) {
	last_width[2] = icuGetWidth(icup) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "width:- %d", last_width[2]) ;
	palSetPad(GPIOC, 4) ;
}

static void icuperiodcb8(ICUDriver *icup) {
	last_period[2] = icuGetPeriod(icup) ;
	chprintf((BaseSequentialStream *)&OUTPUT, "period:- %d\r\n", last_period[2]) ;
	palClearPad(GPIOC, 4) ;
}
/*icucfg8 - To be used for ICU8 */
static ICUConfig icucfg8 = {
	ICU_INPUT_ACTIVE_HIGH,
	ICU_CLK_FREQUENCY,
	icuwidthcb8,
	icuperiodcb8,
	NULL,
	ICU_CHANNEL_2
};
#endif	/*STM32_ICU_USE_TIM8 */

void initICU() {
	/*
	 *Start ICU1 function associated with TIM1.
	 */
#if STM32_ICU_USE_TIM1
	icuStart(&ICUD1, &icucfg1) ;
	palSetPadMode(RC1_PORT, RC1_PIN, PAL_MODE_ALTERNATE(1)) ;
	chThdSleepMilliseconds(10) ;
	icuEnable(&ICUD1) ;
#endif	/*STM32_ICU_USE_TIM1 */
	/*
	 *Start ICU3 function associated with TIM3.
	 */
#if STM32_ICU_USE_TIM3
	icuStart(&ICUD3, &icucfg3) ;
	palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2)) ;
	icuEnable(&ICUD3) ;
#endif	/*STM32_ICU_USE_TIM3 */
	/*
	 *Start ICU8 function associated with TIM8.
	 */
#if STM32_ICU_USE_TIM8
	icuStart(&ICUD8, &icucfg8) ;
	palSetPadMode(RC6_PORT, RC6_PIN, PAL_MODE_ALTERNATE(3)) ;
	chThdSleepMilliseconds(10) ;
	icuEnable(&ICUD8) ;
#endif	/*STM32_ICU_USE_TIM8 */
}

#endif	/*HAL_USE_ICU */
