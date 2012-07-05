/*
 * heading.c
 *
 *  Created on: 29-Jun-2012
 *      Author: riya
 */
#include <math.h>
#include "IMUData.h"
#include "chprintf.h"
#include "ch.h"
#include "MagnetometerData.h"

int16_t acc[3];
float angle[3], cos_pitch, sin_pitch, cos_roll, sin_roll, Heading, X_h, Y_h;
float mag[3] ;

static WORKING_AREA(waheading, 512);
static msg_t heading(void *arg){

	chRegSetThreadName("Heading");

	while(TRUE){
		eulerAngles(angle); /* Gets the roll and pitch angles in degrees */
		cos_pitch = cosf(angle[0]*M_PI/180);
<<<<<<< HEAD
		sin_pitch = sinf(angle[0]*M_PI/180);
		cos_roll = cosf(angle[1]*M_PI/180);
		sin_roll =sinf(angle[1]*M_PI/180);
=======
		sin_pitch = sinf(angle[1]*M_PI/180);
		cos_roll  = cosf(angle[0]*M_PI/180);
		sin_roll  = sinf(angle[1]*M_PI/180);
>>>>>>> d3b3c52ed1cb2ab5a24c4c032e90f1957c39f058

		magGetScaledData(mag) ;
		chThdSleepMilliseconds(20) ;

		X_h = mag[0] * cos_pitch + mag[2] * sin_pitch;
		Y_h = mag[0] * sin_roll * sin_pitch + mag[1] * cos_roll - mag[2] * sin_roll * cos_pitch ;

		Heading = atan2f((-1) *Y_h, X_h);
		Heading *= 180;
		Heading = Heading / M_PI;
		chprintf((BaseSequentialStream *)arg,"Heading : %f \r\n", Heading); /* Heading is obtained in degrees */

		chThdSleepMilliseconds(1000) ;
	}
	return (0);
}

void startHeadingThread(BaseSequentialStream *bss){
	chThdCreateStatic(waheading, sizeof(waheading), NORMALPRIO, heading, bss);
}
