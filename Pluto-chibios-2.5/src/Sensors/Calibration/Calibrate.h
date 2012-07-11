/*Created By - Rohit Rangan
 *Created On - 08-07-2012
 *
 *All the function to calibrate the different sensors
 *on pluto.
 */

#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#if PLUTO_CALIBRATE_IMU

/*Number of readings to be taken.
 */
#define SAMPLES		100
/*Calibration is done using linear regression. coeff stores
 *the coefficients of the linear regression, first for the
 *accelerometer, then the gyrometer.
 */
void calibrateIMU(float coeff[12]) ;
#endif	/*PLUTO_CALIBRATE_IMU */

#endif	/*CALIBRATE_H_ */
