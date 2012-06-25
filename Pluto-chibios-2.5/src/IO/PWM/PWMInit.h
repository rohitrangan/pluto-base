/*Created By - Rohit Rangan
 *Created On - 25-06-2012
 *
 *This file contains all the functions to start
 *the PWM driver.
 */

#ifndef PWMINIT_H_
#define PWMINIT_H_

#define SERVO1_PORT			GPIOB
#define SERVO1_PIN			5
#define SERVO2_PORT			GPIOA
#define SERVO2_PIN			10
#define SERVO3_PORT			GPIOC
#define SERVO3_PIN			8
#define SERVO4_PORT			GPIOB
#define SERVO4_PIN			11
#define SERVO5_PORT			GPIOB
#define SERVO5_PIN			1
#define SERVO6_PORT			GPIOB
#define SERVO6_PIN			0

#define PWM_CLK_FREQUENCY			1000000 /* Clock Frequency of PWM Timer. Higher Frequency better resolution. 1000000 = 1MHz */
#define PWM_PERIOD					20000	/* Time Period of PWM Cycle. 20000 = 20 ms with PWM_Clock_Frequency = 1 MHz */

#define SERVO1		2
#define SERVO2		3
#define SERVO3		3
#define SERVO4		2
#define SERVO5		2
#define SERVO6		1


void initPWM(void) ;

#endif /* PWMINIT_H_ */
