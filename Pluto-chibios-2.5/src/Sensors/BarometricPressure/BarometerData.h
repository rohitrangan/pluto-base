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

#define BARO_TEMP_DATA		1
#define	BARO_PRESSURE_DATA	2

#define EXPONENT			0.190294957f
#define SEA_LEVEL_PRESSURE	101325.0f

#define DELAY_PRESSURE		delay_bmp180
#define DELAY_TEMP			delay_bmp180_temperature
#define MEASUREMENT_MODE	measurement_mode
#define OSS					oss

/*If mode is 1, the function returns the temperature value
 *in degrees Celsius. If mode is 2, the function returns
 *the pressure value in Pascals. No sleeps need to be given
 *when calling this function.
 */
float readBarometerData(uint8_t mode) ;

/*This function returns the absolute altitude above sea level
 *in meters.
 */
float getAltitude(void) ;

#endif	/*BAROMETERDATA_H_ */
