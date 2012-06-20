/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    STM32/RTCv2/rtc_lld.c
 * @brief   STM32L1xx/STM32F2xx/STM32F4xx RTC low level driver.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/**
 * @brief   Wait for synchronization of RTC registers with APB1 bus.
 * @details This function must be invoked before trying to read RTC registers.
 *
 * @notapi
 */
#define rtc_lld_apb1_sync() {while ((RTCD1.id_rtc->ISR & RTC_ISR_RSF) == 0);}

/**
 * @brief   Beginning of configuration procedure.
 *
 * @notapi
 */
#define rtc_lld_enter_init() {                                                \
  RTCD1.id_rtc->ISR |= RTC_ISR_INIT;                                          \
  while ((RTCD1.id_rtc->ISR & RTC_ISR_INITF) == 0)                            \
    ;                                                                         \
}

/**
 * @brief   Finalizing of configuration procedure.
 *
 * @notapi
 */
#define rtc_lld_exit_init() {RTCD1.id_rtc->ISR &= ~RTC_ISR_INIT;}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @api
 */
void rtc_lld_init(void){
  RTCD1.id_rtc = RTC;

  /* Asynchronous part of preloader. Set it to maximum value. */
  uint32_t prediv_a = 0x7F;

  /* Disable write protection. */
  RTCD1.id_rtc->WPR = 0xCA;
  RTCD1.id_rtc->WPR = 0x53;

  /* If calendar not init yet. */
  if (!(RTC->ISR & RTC_ISR_INITS)){
    rtc_lld_enter_init();

    /* Prescaler register must be written in two SEPARATE writes. */
    prediv_a = (prediv_a << 16) |
                (((STM32_RTCCLK / (prediv_a + 1)) - 1) & 0x7FFF);
    RTCD1.id_rtc->PRER = prediv_a;
    RTCD1.id_rtc->PRER = prediv_a;
    rtc_lld_exit_init();
  }
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to set it on STM32 platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
  (void)rtcp;

  rtc_lld_enter_init();
  if (timespec->h12)
    RTCD1.id_rtc->CR |= RTC_CR_FMT;
  else
    RTCD1.id_rtc->CR &= ~RTC_CR_FMT;
  RTCD1.id_rtc->TR = timespec->tv_time;
  RTCD1.id_rtc->DR = timespec->tv_date;
  rtc_lld_exit_init();
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  (void)rtcp;

  rtc_lld_apb1_sync();

#if STM32_RTC_HAS_SUBSECONDS
  timespec->tv_msec =
      (1000 * ((RTCD1.id_rtc->PRER & 0x7FFF) - RTCD1.id_rtc->SSR)) /
      ((RTCD1.id_rtc->PRER & 0x7FFF) + 1);
#endif /* STM32_RTC_HAS_SUBSECONDS */
  timespec->tv_time = RTCD1.id_rtc->TR;
  timespec->tv_date = RTCD1.id_rtc->DR;
}

/**
 * @brief     Set alarm time.
 *
 * @note      Default value after BKP domain reset for both comparators is 0.
 * @note      Function does not performs any checks of alarm time validity.
 *
 * @param[in] rtcp      Pointer to RTC driver structure.
 * @param[in] alarm     Alarm identifier. Can be 1 or 2.
 * @param[in] alarmspec Pointer to a @p RTCAlarm structure.
 *
 * @api
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  if (alarm == 1){
    if (alarmspec != NULL){
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
      while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRAWF))
        ;
      rtcp->id_rtc->ALRMAR = alarmspec->tv_datetime;
      rtcp->id_rtc->CR |= RTC_CR_ALRAE;
      rtcp->id_rtc->CR |= RTC_CR_ALRAIE;
    }
    else {
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAIE;
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
    }
  }
  else{
    if (alarmspec != NULL){
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
      while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRBWF))
        ;
      rtcp->id_rtc->ALRMBR = alarmspec->tv_datetime;
      rtcp->id_rtc->CR |= RTC_CR_ALRBE;
      rtcp->id_rtc->CR |= RTC_CR_ALRBIE;
    }
    else {
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBIE;
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
    }
  }
}

/**
 * @brief   Get alarm time.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @api
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {
  if (alarm == 1)
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMAR;
  else
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMBR;
}

/**
 * @brief     Sets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcSetPeriodicWakeup_v2(RTCDriver *rtcp, RTCWakeup *wakeupspec){
  chDbgCheck((wakeupspec->wakeup != 0x30000),
              "rtc_lld_set_periodic_wakeup, forbidden combination");

  if (wakeupspec != NULL){
    rtcp->id_rtc->CR &= ~RTC_CR_WUTE;
    while(!(rtcp->id_rtc->ISR & RTC_ISR_WUTWF))
      ;
    rtcp->id_rtc->WUTR = wakeupspec->wakeup & 0xFFFF;
    rtcp->id_rtc->CR   = (wakeupspec->wakeup >> 16) & 0x7;
    rtcp->id_rtc->CR |= RTC_CR_WUTIE;
    rtcp->id_rtc->CR |= RTC_CR_WUTE;
  }
  else {
    rtcp->id_rtc->CR &= ~RTC_CR_WUTIE;
    rtcp->id_rtc->CR &= ~RTC_CR_WUTE;
  }
}

/**
 * @brief     Gets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp        pointer to RTC driver structure
 * @param[out] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcGetPeriodicWakeup_v2(RTCDriver *rtcp, RTCWakeup *wakeupspec){
  wakeupspec->wakeup  = 0;
  wakeupspec->wakeup |= rtcp->id_rtc->WUTR;
  wakeupspec->wakeup |= (((uint32_t)rtcp->id_rtc->CR) & 0x7) << 16;
}

#endif /* HAL_USE_RTC */

/** @} */
