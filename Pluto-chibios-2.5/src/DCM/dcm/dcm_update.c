/*
 * dcm_update.c
 *
 *  Created on: 25-Jun-2012
 *      Author: Riya
 */

#include <math.h>

#include "ch.h"
#include "dcm.h"
#include "chprintf.h"
#include "IMUData.h"
#include "MagnetometerData.h"

// TODO - Print the gyro, accelerometer and magnetometer values separately

float acc_final[3];
float gyro[3];
int16_t mag_raw[3];
float gyro_rates[3], mag_final[3];

float calc_gyro_rate(int16_t raw, float sens){
  float tmp = (float)raw;
  tmp /= sens;
  return tmp;
}

static WORKING_AREA(waUpdate, 512);
static msg_t Update(void *arg){

	chRegSetThreadName("update");
	while(TRUE){
		float interval = 110.0f; /* Time between 2 measurements */

		readIMUData(ACCEL_DATA, acc_final) ; /* Reads values from the accelerometer */
		chThdSleepMilliseconds(10) ;

		readIMUData(GYRO_DATA, gyro) ; /* Reads values from the gyroscope */
		chThdSleepMilliseconds(10) ;

		readMagnetometerData(mag_raw) ; /* Reads values from the magnetometer */
		chThdSleepMilliseconds(20) ;

		mag_final[0] = mag_raw[0] * MAG_RANGE ;
		mag_final[1] = mag_raw[1] * MAG_RANGE ;
		mag_final[2] = mag_raw[2] * MAG_RANGE ;

		dcmUpdate(gyro_rates, acc_final[0], acc_final[1], acc_final[2], gyro[0], gyro[1], gyro[2], mag_final[0], mag_final[1], mag_final[2], interval);

		chprintf((BaseSequentialStream *)arg,"gyro rate x: %f\t\t", gyro_rates[0]);
		chprintf((BaseSequentialStream *)arg,"gyro rate y: %f\t\t", gyro_rates[1]);
		chprintf((BaseSequentialStream *)arg,"gyro rate z: %f\r\n", gyro_rates[2]);

		chThdSleepMilliseconds(100) ;
	}
	return(0);
}

void startDCMThread(BaseSequentialStream *bss){
	chThdCreateStatic(waUpdate, sizeof(waUpdate), NORMALPRIO, Update, bss);
}
