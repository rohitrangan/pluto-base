/*Created By - Rohit Rangan
 *Created On - 22-06-2012
 *
 *This file contains the functions to get data from
 *the magnetometer.
 */

#ifndef MAGNETOMETERDATA_H_
#define MAGNETOMETERDATA_H_

#include "hmc5883.h"
#define MAG_RANGE hmc_range

/*The raw values are stored in val. To convert to nano Tesla,
 *multiply val[i] by MAG_RANGE.
 */
void readMagnetometerData(int16_t val[3]) ;

/*This function calculates the magnetometer data in uT and
 *is stored in val.
 */
void magGetScaledData(float val[3]) ;

/*Returns the heading of the board in degrees.
 *000 -> North.
 *090 -> East.
 *180 -> South.
 *270 -> West.
 */
float getHeading(void) ;

#endif /*MAGNETOMETERDATA_H_ */
