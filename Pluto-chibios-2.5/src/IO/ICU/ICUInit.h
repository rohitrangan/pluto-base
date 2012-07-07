/*Created By - Rohit Rangan
 *Created On - 06-07-2012
 *
 *This file contains all the functions to initialize
 *the ICU driver.
 */

#ifndef ICUINIT_H_
#define ICUINIT_H_

#if HAL_USE_ICU
/*The definition of the RC inputs and which pins they are mapped to.
 *
 *RC1 (PA7 ) --> Timer 3 Channel 2 (Alternate Function 2)
 *RC2 (PA6 ) --> Timer 1 Channel 3 (Alternate Function 1)
 *RC3 (PB14) --> Timer 8 Channel 3 (Alternate Function 3)
 *RC4 (PB15) --> Timer 2 Channel 4 (Alternate Function 1)
 *RC5 (PC6 ) --> Timer 3 Channel 4 (Alternate Function 2)
 *RC6 (PC7 ) --> Timer 3 Channel 3 (Alternate Function 2)
 */
#define RC1_PORT		GPIOA
#define RC1_PIN			7
#define RC2_PORT		GPIOA
#define RC2_PIN			6
#define RC3_PORT		GPIOB
#define RC3_PIN			14
#define RC4_PORT		GPIOB
#define RC4_PIN			15
#define RC5_PORT		GPIOC
#define RC5_PIN			6
#define RC6_PORT		GPIOC
#define RC6_PIN			7

#define ICU_CLK_FREQUENCY		1000000
#endif	/*HAL_USE_ICU */

#endif	/*ICUINIT_H_ */
