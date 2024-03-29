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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F4_DISCOVERY
#define BOARD_NAME              "PLUTO"

/*
 * Board frequencies.
 * NOTE: The LSE crystal is not fitted by default on the board.
 */
#define STM32_LSECLK            0
#define STM32_HSECLK            25000000//8000000

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD               300

/*
 * MCU type as defined in the ST header file stm32f4xx.h.
 */
#define STM32F4XX

/*
 * IO pins assignments.
 */
#define GPIOA_CS_SPI2           0      /* rrangan changed for PLUTO assignment. GPIOA_BUTTON            0 */ /* TODO - rrangan - It has been set to floating later on in the file - may need to pull it up ! */
#define GPIOA_LRCK              4
#define GPIOA_SPC               5
#define GPIOA_SDO               6
#define GPIOA_SDI               7
// #define GPIOA_VBUS_FS           9
#define GPIOA_OTG_FS_ID         10
#define GPIOA_OTG_FS_DM         11
#define GPIOA_OTG_FS_DP         12
#define GPIOA_SWDIO             13
#define GPIOA_SWCLK             14

#define GPIOB_SWO               3
#define GPIOB_SCL               6
#define GPIOB_SDA               9
#define GPIOB_SCK               10

#define GPIOC_OTG_FS_POWER_ON   0
#define GPIOC_DOUT              3
#define GPIOC_MCLK              7
#define GPIOC_SCLK              10
#define GPIOC_SDIN              12

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * Port A setup.
 * All input with pull-up except:
 * PA0  - GPIOA_CS_SPI2         (input floating).
 * PA4  - GPIOA_LRCK            (alternate 6).
 * PA5  - GPIOA_SPC             (alternate 5).
 * PA6  - GPIOA_SDO             (alternate 5).
 * PA7  - GPIOA_SDI             (alternate 5).
 * PA9  - GPIOA_VBUS_FS         (input floating).
 * PA10 - GPIOA_OTG_FS_ID       (alternate 10).
 * PA11 - GPIOA_OTG_FS_DM       (alternate 10).
 * PA12 - GPIOA_OTG_FS_DP       (alternate 10).
 * PA13 - GPIOA_SWDIO           (alternate 0).  // JTAG JTMS
 * PA14 - GPIOA_SWCLK           (alternate 0).  // JTAG JTCK
 * PA15 - GPIOA_SWCLK           (alternate 0).  // JTAG JTDI
 */

#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_CS_SPI2) |        \
                                     PIN_MODE_INPUT(1) |                    \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_INPUT(3) |                    \
                                     PIN_MODE_ALTERNATE(GPIOA_LRCK) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_SPC) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_SDO) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_SDI) |        \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_ALTERNATE(15))
#define VAL_GPIOA_OTYPER            0x00000000
#define VAL_GPIOA_OSPEEDR           0xFFFFFFFF
                                     /* PIN_PUDR_FLOATING(GPIOA_VBUS_FS) |     \ */

#define VAL_GPIOA_PUPDR             (PIN_PUDR_FLOATING(GPIOA_CS_SPI2) |      \
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_FLOATING(GPIOA_LRCK) |        \
                                     PIN_PUDR_FLOATING(GPIOA_SPC) |         \
                                     PIN_PUDR_FLOATING(GPIOA_SDO) |         \
                                     PIN_PUDR_FLOATING(GPIOA_SDI) |         \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_ID) |   \
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_DM) |   \
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_DP) |   \
                                     PIN_PUDR_PULLUP(GPIOA_SWDIO) |         \
                                     PIN_PUDR_PULLDOWN(GPIOA_SWCLK) |       \
                                     PIN_PUDR_FLOATING(15))
#define VAL_GPIOA_ODR               0xFFFFFFFF
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_LRCK, 6) |           \
                                     PIN_AFIO_AF(GPIOA_SPC, 5) |            \
                                     PIN_AFIO_AF(GPIOA_SDO, 5) |            \
                                     PIN_AFIO_AF(GPIOA_SDI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |     \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |          \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) | 			\
                                     PIN_AFIO_AF(15,0))

/*
 * Port B setup.
 * All input with pull-up except:
 * PB3  - GPIOB_SWO             (alternate 0). // JTAG JTDO
 * PB4  - GPIOB_SWO             (alternate 0). // JTAG NJTRST
 * PB6  - GPIOB_SCL             (alternate 4).
 * PB9  - GPIOB_SDA             (alternate 4).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(0) |                    \
                                     PIN_MODE_INPUT(1) |                    \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_ALTERNATE(GPIOB_SWO) |        \
                                     PIN_MODE_ALTERNATE(4) |                    \
                                     PIN_MODE_INPUT(5) |                    \
                                     PIN_MODE_INPUT(GPIOB_SCL) |        \
                                     PIN_MODE_INPUT(7) |                    \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(GPIOB_SDA) |        \
                                     PIN_MODE_INPUT(10) |                   \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_INPUT(12) |                   \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_OPENDRAIN(GPIOB_SCL) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_SDA))
#define VAL_GPIOB_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOB_PUPDR             (PIN_PUDR_PULLUP(0) |                   \
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_FLOATING(3) |         \
                                     PIN_PUDR_FLOATING(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(GPIOB_SCL) |         \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(GPIOB_SDA) |         \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOB_ODR               0xFFFFFFFF
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(3,0) | PIN_AFIO_AF(4,0))
#define VAL_GPIOB_AFRH              0x00000000

/*
 * Port C setup.
 * All input with pull-up except:
 * PC0  - GPIOC_OTG_FS_POWER_ON (output push-pull).
 * PC4  - Output LED
 * PC5  - Output LED
 * PC7  - GPIOC_MCLK            (alternate 6).
 * PC10 - GPIOC_SCLK            (alternate 6).
 * PC12 - GPIOC_SDIN            (alternate 6).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(GPIOC_OTG_FS_POWER_ON) |\
                                     PIN_MODE_INPUT(1) |                    \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_INPUT(3) |                    \
                                     PIN_MODE_OUTPUT(4) |                    \
                                     PIN_MODE_OUTPUT(5) |                    \
                                     PIN_MODE_INPUT(6) |                    \
                                     PIN_MODE_INPUT(GPIOC_MCLK) |       \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(9) |                    \
                                     PIN_MODE_ALTERNATE(GPIOC_SCLK) |       \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_ALTERNATE(GPIOC_SDIN) |       \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER            0x00000000
#define VAL_GPIOC_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOC_PUPDR             (PIN_PUDR_FLOATING(GPIOC_OTG_FS_POWER_ON) |\
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_FLOATING(GPIOC_MCLK)  |       \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_FLOATING(GPIOC_SCLK)  |       \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_FLOATING(GPIOC_SDIN)  |       \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOC_ODR               0xFFFFFFFF
#define VAL_GPIOC_AFRL              0x00000000
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SCLK, 6) |           \
                                     PIN_AFIO_AF(GPIOC_SDIN, 6))

/*
 * Port D setup.
 * All input with pull-up
 */
#define VAL_GPIOD_MODER             0x00000000
#define VAL_GPIOD_OTYPER            0x00000000
#define VAL_GPIOD_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOD_PUPDR             0xFFFFFFFF
#define VAL_GPIOD_ODR               0xFFFFFFFF
#define VAL_GPIOD_AFRL              0x00000000
#define VAL_GPIOD_AFRH              0x00000000

/*
 * Port E setup.
 * All input with pull-up
 */
#define VAL_GPIOE_MODER             0x00000000
#define VAL_GPIOE_OTYPER            0x00000000
#define VAL_GPIOE_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOE_PUPDR             0xFFFFFFFF
#define VAL_GPIOE_ODR               0xFFFFFFFF
#define VAL_GPIOE_AFRL              0x00000000
#define VAL_GPIOE_AFRH              0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER             0x00000000
#define VAL_GPIOF_OTYPER            0x00000000
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             0xFFFFFFFF
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL              0x00000000
#define VAL_GPIOF_AFRH              0x00000000

/*
 * Port G setup.
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER             0x00000000
#define VAL_GPIOG_OTYPER            0x00000000
#define VAL_GPIOG_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOG_PUPDR             0xFFFFFFFF
#define VAL_GPIOG_ODR               0xFFFFFFFF
#define VAL_GPIOG_AFRL              0x00000000
#define VAL_GPIOG_AFRH              0x00000000

/*
 * Port H setup.
 * All input with pull-up
 */
#define VAL_GPIOH_MODER             0x00000000
#define VAL_GPIOH_OTYPER            0x00000000
#define VAL_GPIOH_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOH_PUPDR             0xFFFFFFFF
#define VAL_GPIOH_ODR               0xFFFFFFFF
#define VAL_GPIOH_AFRL              0x00000000
#define VAL_GPIOH_AFRH              0x00000000

/*
 * Port I setup.
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER             0x00000000
#define VAL_GPIOI_OTYPER            0x00000000
#define VAL_GPIOI_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOI_PUPDR             0xFFFFFFFF
#define VAL_GPIOI_ODR               0xFFFFFFFF
#define VAL_GPIOI_AFRL              0x00000000
#define VAL_GPIOI_AFRH              0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
