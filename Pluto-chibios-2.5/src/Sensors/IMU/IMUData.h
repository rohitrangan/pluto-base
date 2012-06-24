/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */

#ifndef IMUDATA_H_
#define IMUDATA_H_

/*If Mode is 1 then the accelerometer values
 *are read, mode is 2 then gyrometer values
 *are read, if mode is 3 then the temperature
 *value is read. val stores the values of the
 *sensor data.
 */
void readIMUData(uint8_t mode, int16_t val[3]) ;

/*This function calculates the pitch, roll and
 *yaw. It calls readAccelerometerData inside.
 *The angles are stored in the order pitch,
 *roll and yaw.
 */
#if CORTEX_USE_FPU
void eulerAngles(float angles[3]) ;
#endif	/*CORTEX_USE_FPU */

#endif	/*IMUDATA_H_ */
