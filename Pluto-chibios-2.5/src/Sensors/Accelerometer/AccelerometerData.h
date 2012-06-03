/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains the functions to get data from
 *the accelerometer.
 */

#ifndef ACCELEROMETERDATA_H_
#define ACCELEROMETERDATA_H_

/*If Mode is 1 then the accelerometer values
 *are read, mode is 2 then gyrometer values
 *are read, if mode is 3 then the temperature
 *value is read. val stores the values of the
 *sensor data.
 */
void readAccelerometerData(uint8_t mode, int16_t val[3]) ;

#endif
