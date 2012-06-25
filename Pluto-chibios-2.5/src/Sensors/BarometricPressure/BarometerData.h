/*Created By - Rohit Rangan
 *Created On - 23-06-2012
 *
 *This file contains the functions to get data from
 *the barometer.
 */

#ifndef BAROMETERDATA_H_
#define BAROMETERDATA_H_

#include "ch.h"
#include "BMP180.h"

#define BARO_TEMP_DATA	1
#define	PRESSURE_DATA	2

#define DELAY_PRESSURE		delay_bmp180
#define DELAY_TEMP			delay_bmp180_temperature
#define MEASUREMENT_MODE	measurement_mode
#define OSS					oss

/*If mode is 1, the function returns the temperature value
 *in 0.1 degrees Celsius. If mode is 2, the function returns
 *the pressure value in Pascals. No sleeps need to be given
 *when calling this function.
 */
int32_t readBarometerData(uint8_t mode) ;

#endif	/*BAROMETERDATA_H_ */
