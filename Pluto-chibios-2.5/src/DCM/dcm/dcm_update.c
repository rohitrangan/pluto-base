/*
 * dcm_update.c
 *
 *  Created on: 25-Jun-2012
 *      Author: Riya
 */

#include <math.h>

#include "pluto.h"

// TODO - Print the gyro, accelerometer and magnetometer values separately

#if PLUTO_USE_DCM && PLUTO_USE_MAGNETOMETER && PLUTO_USE_IMU

static WORKING_AREA(waUpdate, 512);
static msg_t Update(void *arg) {

	chRegSetThreadName("DCMUpdate") ;
	float dcmMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0 ,1}} ;
	uint32_t start, end ;
	uint8_t first = 1 ;
	while(TRUE) {
		float interval, pitch, roll, yaw; /* Time between 2 measurements */
		if(first) {
			start = halGetCounterValue() ;
			first = 0 ;
		}
		readAllIMUData(&IMUD1) ; /* Reads values from the accelerometer and gyroscope */
		chThdSleepMilliseconds(10) ;

		readMagnetometerData(&MD1) ; /* Reads values from the magnetometer */
		chThdSleepMilliseconds(10) ;
		end = halGetCounterValue() ;
		interval = convertCounterToMilliseconds(start, end) / 1000.0f ;
		start = end ;

		dcmUpdate(dcmMatrix, IMUD1.ACCEL_X, IMUD1.ACCEL_Y, IMUD1.ACCEL_Z, IMUD1.GYRO_X, IMUD1.GYRO_Y, IMUD1.GYRO_Z, MD1.MAG_X, MD1.MAG_Y, MD1.MAG_Z, interval) ;

		if(dcmMatrix[2][2] >= 0) {
			pitch = -asinf(dcmMatrix[2][0]) ;
			roll  = -asinf(dcmMatrix[2][1]) ;
		}
		else {
			pitch = M_PI - (-asinf(dcmMatrix[2][0])) ;
			roll  = M_PI - (-asinf(dcmMatrix[2][1])) ;
		}

		yaw = my_atan2f((-1.0f * dcmMatrix[1][0]), (-1.0f * dcmMatrix[0][0])) ;
		pitch = (180 * pitch) / M_PI ;
		roll  = (180 * roll ) / M_PI ;
		yaw   = (180 * yaw  ) / M_PI ;

        //TODO - rrangan - put this in ifdef and make a command of it.
		chprintf((BaseSequentialStream *)arg, "Interval:- %f ms ", (interval * 1000.0f)) ;
		chprintf((BaseSequentialStream *)arg, "Pitch : %f\t\t", pitch) ;
		chprintf((BaseSequentialStream *)arg, "Roll  : %f\t\t", roll ) ;
		chprintf((BaseSequentialStream *)arg, "Yaw   : %f\r\n", yaw  ) ;
	}
	return 0 ;
}

void startDCMThread(BaseSequentialStream *bss) {
	chThdCreateStatic(waUpdate, sizeof(waUpdate), HIGHPRIO, Update, bss) ;
}
#endif	/*PLUTO_USE_DCM && PLUTO_USE_MAGNETOMETER && PLUTO_USE_IMU */
