/*
 * dcm_update.c
 *
 *  Created on: 25-Jun-2012
 *      Author: Riya
 */

#include <math.h>

#include "ch.h"
#include "dcm.h"
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

	float dcmMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0 ,1}} ;
	chRegSetThreadName("DCMUpdate");
	while(TRUE){
		float interval = 110.0f, pitch, roll, yaw; /* Time between 2 measurements */

		readIMUData(ACCEL_DATA, acc_final) ; /* Reads values from the accelerometer */
		chThdSleepMilliseconds(10) ;

		readIMUData(GYRO_DATA, gyro) ; /* Reads values from the gyroscope */
		chThdSleepMilliseconds(10) ;

		magGetScaledData(mag_final) ; /* Reads values from the magnetometer */
		chThdSleepMilliseconds(20) ;

		dcmUpdate(dcmMatrix, acc_final[0], acc_final[1], acc_final[2], gyro[0], gyro[1], gyro[2], mag_final[0], mag_final[1], mag_final[2], interval);

		if(dcmMatrix[2][2] >= 0) {
			pitch = -asinf(dcmMatrix[2][0]) ;
			roll  = -asinf(dcmMatrix[2][1]) ;
		}
		else {
			pitch = M_PI - (-asinf(dcmMatrix[2][0])) ;
			roll  = M_PI - (-asinf(dcmMatrix[2][1])) ;
		}

		yaw = atan2f(dcmMatrix[1][0], (-1 * dcmMatrix[0][0])) ;
		pitch = (180 * pitch) / M_PI ;
		roll  = (180 * roll ) / M_PI ;
		yaw   = (180 * yaw  ) / M_PI ;

		chprintf((BaseSequentialStream *)arg,"Pitch : %f\t\t", pitch);
		chprintf((BaseSequentialStream *)arg,"Roll  : %f\t\t", roll );
		chprintf((BaseSequentialStream *)arg,"Yaw   : %f\r\n", yaw  );

		chThdSleepMilliseconds(100) ;
	}
	return(0);
}

void startDCMThread(BaseSequentialStream *bss){
	chThdCreateStatic(waUpdate, sizeof(waUpdate), NORMALPRIO, Update, bss);
}
#endif	/*PLUTO_USE_DCM */
