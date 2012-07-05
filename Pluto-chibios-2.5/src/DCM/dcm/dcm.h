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
#endif 	/*PLUTO_USE_DCM */

#endif 	/*DCM_H_ */
