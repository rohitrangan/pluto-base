/*
 * dcm_update.h
 *
 *  Created on: 25-Jun-2012
 *      Author: riya
 */

#ifndef DCM_UPDATE_H_
#define DCM_UPDATE_H_

float calc_gyro_rate(int16_t raw, float sens);
void start_thread(BaseSequentialStream *bss);

#endif /* DCM_UPDATE_H_ */
