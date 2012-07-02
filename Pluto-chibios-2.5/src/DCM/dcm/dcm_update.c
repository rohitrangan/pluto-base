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

int16_t acc_raw[3], acc_final[3];
int16_t gyro[3];
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

		readIMUData(ACCEL_DATA, acc_raw) ; /* Reads values from the accelerometer */
		chThdSleepMilliseconds(10) ;

		acc_final[0] = 1000 * (acc_raw[0]) / ACCEL_XSENS;
		acc_final[1] = 1000 * (acc_raw[1]) / ACCEL_YSENS;
		acc_final[2] = 1000 * (acc_raw[2]) / ACCEL_ZSENS;

		readIMUData(GYRO_DATA, gyro) ; /* Reads values from the gyroscope */
		chThdSleepMilliseconds(10) ;

		readMagnetometerData(mag_raw) ; /* Reads values from the magnetometer */
		chThdSleepMilliseconds(20) ;

		mag_final[0] = mag_raw[0] * MAG_RANGE ;
		mag_final[1] = mag_raw[1] * MAG_RANGE ;
		mag_final[2] = mag_raw[2] * MAG_RANGE ;

		dcmUpdate(gyro_rates, ((float)acc_final[0]) / 1000, ((float)acc_final[1]) / 1000, ((float)acc_final[2]) / 1000, calc_gyro_rate(gyro[0], GYRO_XSENS), calc_gyro_rate(gyro[1], GYRO_YSENS), calc_gyro_rate(gyro[2], GYRO_ZSENS), mag_final[0], mag_final[1], mag_final[2], interval);

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
