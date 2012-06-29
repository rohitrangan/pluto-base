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

#if CORTEX_USE_FPU
/*Returns the heading of the board in degrees.
 *000 -> North.
 *090 -> East.
 *180 -> South.
 *270 -> West.
 */
float getHeading(void) ;
#endif	/*CORTEX_USE_FPU */

#endif /*MAGNETOMETERDATA_H_ */
