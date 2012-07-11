/*
 * dcm_update.h
 *
 *  Created on: 25-Jun-2012
 *      Author: riya
 */

#ifndef DCM_UPDATE_H_
#define DCM_UPDATE_H_

#if PLUTO_USE_DCM && PLUTO_USE_MAGNETOMETER && PLUTO_USE_IMU
float calc_gyro_rate(int16_t raw, float sens);
void startDCMThread(BaseSequentialStream *bss);
#endif	/*PLUTO_USE_DCM && PLUTO_USE_MAGNETOMETER && PLUTO_USE_IMU */

#endif /*DCM_UPDATE_H_ */
