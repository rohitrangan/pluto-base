/*Created By - Rohit Rangan
 *Created On - 22-06-2012
 *
 *This file contains the functions to get data from
 *the magnetometer.
 */

#ifndef MAGNETOMETERDATA_H_
#define MAGNETOMETERDATA_H_

#include "hmc5883.h"
#define RANGE hmc_range

/*The raw values are stored in val. To convert to nano Tesla,
 *multiply val[i] by HMC_RANGE_CONVERT and divide by 10.
 */
void readMagnetometerData(int16_t val[3]) ;

#endif /*MAGNETOMETERDATA_H_ */
