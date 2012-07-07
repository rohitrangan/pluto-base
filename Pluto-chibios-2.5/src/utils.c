/*Created By - Rohit Rangan
 *Created On - 06-07-2012
 *
 *Contains important conversion functions.
 */
#include<math.h>

#include "ch.h"
#include "hal.h"

inline float convertCounterToMilliseconds(uint32_t start, uint32_t end) {
	return (((float)(end - start) / (float)STM32_SYSCLK) * 1000.0f) ;
}

inline float my_atan2f(float y, float x) {
	if((x > 0) && (y >= 0))
		return atanf(y / x) ;
	else if(x < 0)
		return (M_PI + atanf(y / x)) ;
	else if((x > 0) && (y < 0))
		return (M_TWOPI + atanf(y / x)) ;
	else if((x == 0) && (y < 0))
		return M_PI_2 ;
	else if((x == 0) && (y > 0))
		return (3 * M_PI_2) ;
	else
		return -32768.0 ;
}
