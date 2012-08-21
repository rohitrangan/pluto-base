/*
 * dcm.h
 *
 *  Created on: 25-Jun-2012
 *      Author: riya
 */

#ifndef DCM_H_
#define DCM_H_

#include "plutoconf.h"
#if PLUTO_USE_DCM
void dcmInit(void);
void dcmUpdate(float dcmEst[3][3], float xacc,  float yacc,  float zacc,\
				float xgyro, float ygyro, float zgyro,\
				float xmag,  float ymag,  float zmag,\
				float imu_interval);

typedef struct {
  float interval; // milliseconds
  uint32_t timestamp;
  float raw_gyro_data[3]; // deg per second ?? TODO - confirm
  float raw_accel_data[3]; // meter per s sq. or G // TODO - confirm
  float raw_mag_data[3]; // TODO - units ?
  float attitude[3]; // roll. pitch, yaw // TODO
}current_attitude_struct;

#endif 	/*PLUTO_USE_DCM */

#endif 	/*DCM_H_ */
