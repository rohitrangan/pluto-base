/*Created By - Rohit Rangan
 *Created On - 25-06-2012
 *
 *This file contains all the functions to start
 *the PWM driver.
 */

#include "ch.h"
#include "hal.h"
#include "PWMInit.h"

#define PWM_DEBUG		/* For Debug Purpose only. Disable this during Normal Operation. */

/*
 *PWM configuration structures.
 */
/*pwmcfg1 - to be used for PWM1. */
static PWMConfig pwmcfg1 = {
		PWM_CLK_FREQUENCY,
		PWM_PERIOD,
		NULL,
		{											/* CH3 of Timer1 for is available for PWM. Enable it and disable the rest */
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL},
				{PWM_OUTPUT_DISABLED, NULL}
		},
		/*HW Dependent Part */
		0,
		0
};
/*pwmcfg2 - to be used for PWM2. */
static PWMConfig pwmcfg2 = {
		PWM_CLK_FREQUENCY,
		PWM_PERIOD,
		NULL,
		{											/* CH4 of Timer for is available for PWM. Enable it and disable the rest. */
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL}
		},
		/*HW Dependent Part*/
		0,
		0
};

/*pwmcfg3 - to be used for PWM3. */
/*static PWMConfig pwmcfg3 = {
		PWM_CLK_FREQUENCY,
		PWM_PERIOD,
		NULL,
		{											CH2, CH3 and CH4 of Timer for are available for PWM. Enable them and disable the rest.
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL}
		},
		HW Dependent Part
		0,
		0
};*/

/*pwmcfg8 - to be used for PWM8..i.e timer 8 */
static PWMConfig pwmcfg8 = {
		PWM_CLK_FREQUENCY,
		PWM_PERIOD,

		NULL,
		{											/*CH3 of Timer 8 are available for PWM. Enable it and disable the rest. */
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_DISABLED, NULL},
				{PWM_OUTPUT_ACTIVE_HIGH, NULL},
				{PWM_OUTPUT_DISABLED, NULL}
		},
		/* HW dependent part.*/
		0,
		0   /* STM32_PWM_USE_ADVANCED is enabled which adds an additional field. */
};



/*
 * This function initializes Servo Output Pins and Connects them to PMW outputs.
 * SERVO1 (PB5 ) --> Timer 3 Channel 2 (Alternate Function 2)
 * SERVO2 (PA10) --> Timer 1 Channel 3 (Alternate Function 1)
 * SERVO3 (PC8 ) --> Timer 8 Channel 3 (Alternate Function 3)
 * SERVO4 (PB11) --> Timer 2 Channel 4 (Alternate Function 1)
 * SERVO5 (PB1 ) --> Timer 3 Channel 4 (Alternate Function 2)
 * SERVO6 (PB0 ) --> Timer 3 Channel 3 (Alternate Function 2)
 */

void initPWM(void){

	/*
	 *Start PWM1 function associated with Timer 1.
	 */
	pwmStart(&PWMD1, &pwmcfg1) ;
	palSetPadMode(SERVO2_PORT, SERVO2_PIN, PAL_MODE_ALTERNATE(1)) ;
	chThdSleepMilliseconds(10) ;

	/*
	 * Start PWM2 function associated with Timer 2.
	 */
	pwmStart(&PWMD2, &pwmcfg2) ;
	palSetPadMode(SERVO4_PORT, SERVO4_PIN, PAL_MODE_ALTERNATE(1)) ;
	chThdSleepMilliseconds(10) ;

	/*
	 * Start PWM3 function associated with Timer 3.
	 */
//	pwmStart(&PWMD3, &pwmcfg3) ;
//	palSetPadMode(SERVO1_PORT, SERVO1_PIN, PAL_MODE_ALTERNATE(2)) ;
//	palSetPadMode(SERVO5_PORT, SERVO5_PIN, PAL_MODE_ALTERNATE(2)) ;
//	palSetPadMode(SERVO6_PORT, SERVO6_PIN, PAL_MODE_ALTERNATE(2)) ;

	/*
	 * Start PWM8 function associated with Timer 8.
	 */
	pwmStart(&PWMD8, &pwmcfg8) ;
	palSetPadMode(SERVO3_PORT, SERVO3_PIN, PAL_MODE_ALTERNATE(3)) ;
	chThdSleepMilliseconds(10) ;

#ifdef PWM_DEBUG
	/*PA10 -> CH3. For Debugging Only. Servo5 PWM = 1.8 ms */
	pwmEnableChannelI(&PWMD1, SERVO2, PWM_FRACTION_TO_WIDTH(&PWMD1, PWM_PERIOD, 1800)) ;

	/*PB11 -> CH4. For Debugging only. Servo3 PWM = 1.4 ms */
	pwmEnableChannelI(&PWMD2, SERVO4, PWM_FRACTION_TO_WIDTH(&PWMD2, PWM_PERIOD, 1400)) ;

	/*PC8  -> CH4. For Debugging only. Servo4 PWM = 1.6 ms */
	pwmEnableChannelI(&PWMD8, SERVO3, PWM_FRACTION_TO_WIDTH(&PWMD8, PWM_PERIOD, 1600)) ;

	//pwmEnableChannelI(&PWMD3, SERVO1, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, 2000)) ; /*PB5 -> CH2. For Debugging Only. Servo6 PWM = 2.0 ms  */
	//pwmEnableChannelI(&PWMD3, SERVO5, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, 1000)) ; /*PB1 -> CH3. For Debugging Only. Servo1 PWM = 1.0 ms  */
	//pwmEnableChannelI(&PWMD3, SERVO6, PWM_FRACTION_TO_WIDTH(&PWMD3, PWM_PERIOD, 1200)) ; /*PB0 -> CH4. For Debugging Only. Servo2 PWM = 1.2 ms  */
#endif

}
