/*
 * dcm_update.c
 *
 *  Created on: 25-Jun-2012
 *      Author: Riya
 */

#include <math.h>

#include "ch.h"
#include "hal.h"
#include "dcm.h"
#include "utils.h"
#include "IMUData.h"
#include "chprintf.h"
#include "plutoconf.h"
#include "MagnetometerData.h"

// TODO - Print the gyro, accelerometer and magnetometer values separately

#if PLUTO_USE_DCM
float mag_final[3], gyro[3], acc_final[3];

float calc_gyro_rate(int16_t raw, float sens){
  float tmp = (float)raw;
  tmp /= sens;
  return tmp;
}

static WORKING_AREA(waUpdate, 512);
static msg_t Update(void *arg) {

	chRegSetThreadName("DCMUpdate");
	float dcmMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0 ,1}} ;
	uint32_t start, end ;
	uint8_t first = 1 ;
	while(TRUE) {
		float interval, pitch, roll, yaw; /* Time between 2 measurements */
		if(first) {
			start = halGetCounterValue() ;
			first = 0 ;
		}
		readIMUData(ACCEL_DATA, acc_final) ; /* Reads values from the accelerometer */
		chThdSleepMilliseconds(10) ;

		readIMUData(GYRO_DATA, gyro) ; /* Reads values from the gyroscope */
		chThdSleepMilliseconds(10) ;

		magGetScaledData(mag_final) ; /* Reads values from the magnetometer */
		chThdSleepMilliseconds(10) ;
		end = halGetCounterValue() ;
		interval = convertCounterToMilliseconds(start, end) / 1000.0f ;
		start = end ;

		dcmUpdate(dcmMatrix, acc_final[0], acc_final[1], acc_final[2], gyro[0], gyro[1], gyro[2], mag_final[0], mag_final[1], mag_final[2], interval);

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

		chprintf((BaseSequentialStream *)arg, "Interval:- %f ms ", (interval * 1000.0f)) ;
		chprintf((BaseSequentialStream *)arg, "Pitch : %f\t\t", pitch);
		chprintf((BaseSequentialStream *)arg, "Roll  : %f\t\t", roll );
		chprintf((BaseSequentialStream *)arg, "Yaw   : %f\r\n", yaw  );
	}
	return(0);
}

void startDCMThread(BaseSequentialStream *bss){
	chThdCreateStatic(waUpdate, sizeof(waUpdate), HIGHPRIO, Update, bss);
}
#endif	/*PLUTO_USE_DCM */
