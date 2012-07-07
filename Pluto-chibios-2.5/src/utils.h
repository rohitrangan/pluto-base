/*Created By - Rohit Rangan
 *Created On - 06-07-2012
 *
 *Contains important conversion functions.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "ch.h"

/*This function converts the halCounter ticks to milliseconds. Call
 *halGetCounterValue() before starting any operation. Assign the value
 *returned to start, the first parameter of this function. After the
 *operation has concluded, the end parameter will be halGetCounterValue().
 */
inline float convertCounterToMilliseconds(uint32_t start, uint32_t end) ;

/*My atan2f function.
 */
inline float my_atan2f(float y, float x) ;

#endif	/*UTILS_H_ */
