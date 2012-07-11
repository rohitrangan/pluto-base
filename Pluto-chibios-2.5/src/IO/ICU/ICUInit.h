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
 *RC1 (PA7 )
 *RC2 (PA6 )
 *RC3 (PB14)
 *RC4 (PB15)
 *RC5 (PC6 )
 *RC6 (PC7 )
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

#define ICU_CLK_FREQUENCY		100000

extern icucnt_t last_width[6], last_period[6] ;

void initICU(void) ;
#endif	/*HAL_USE_ICU */

#endif	/*ICUINIT_H_ */
