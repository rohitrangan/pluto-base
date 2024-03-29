#include "pluto.h"

//extern InputValues(float input[4]);

static float att_err_int[3] = {0.0f, 0.0f, 0.0f};  // integral of att_err till now .. unit: deg-sec
static float rate_err_int[3] = {0.0f, 0.0f, 0.0f}; // integral of rate_err till now .. unit: deg

// outer loop gains
float kpo[3] = {2.0f, 2.0f, 0.0f};
float kio[3] = {0.0f, 0.0f, 0.0f};

// inner loop gains
float kpi[3] = {0.002f, 0.002f, 0.0035f};
float kii[3] = {0.0f, 0.0f, 0.0f};

// saturators
static float max_att_err_int[3] = {30.0f,30.0f,30.0f};  	// max integral of attitude error
static float max_rate_err_int[3] = {20.0f, 20.0f, 20.0f}; 	// max integral of rate error
static float max_rate_des[3] = {20.0f, 20.0f, 20.0f}; 		// max rate desired
static float max_corr[3] = {200.0f, 200.0f, 200.0f};  		// max correction to motor RPMs

// desired attitude (roll, pitch, yaw) in degrees
float att_des[3] = {0.0f, 0.0f, 180.0f};

extern current_attitude_struct current_attitude;

uint32_t previous_time = 0;

void control(BaseSequentialStream* bss){
float rcinp[4];
InputValues(rcinp);
int base_val = 1500; // (uint32_t)rcinp[2];        // base RPM from Throttle .. value between 1000 (min) and 2000 (max)
float rate_curr[3] = {current_attitude.raw_gyro_data[0], current_attitude.raw_gyro_data[1], current_attitude.raw_gyro_data[2]}; // current value of rates (roll, pitch, yaw) .. units: deg/sec
previous_time = current_attitude.timestamp;

float att_err[3] = {0.0f, 0.0f, 0.0f};
int i=0;
for(i = 0; i <= 2; i++){
  att_err[i] = att_des[i] - current_attitude.attitude[i];
  att_err_int[i] = att_err_int[i] + att_err[i] * current_attitude.interval;
  if (att_err_int[i] > max_att_err_int[i]){
	att_err_int[i] = max_att_err_int[i];
  }
}

float rate_des[3];
for(i = 0; i <= 2; i++){
  rate_des[i] = att_err[i] * kpo[i] + att_err_int[i] * kio[i];
  if(rate_des[i] > max_rate_des[i]){
	rate_des[i] = max_rate_des[i];
  }
}

float rate_err[3];
for(i = 0; i <= 2; i++){
  rate_err[i] = rate_des[i] - rate_curr[i];
  rate_err_int[i] = rate_err_int[i] + rate_err[i] * current_attitude.interval;
  if(rate_err_int[i] > max_rate_err_int[i])
	rate_err_int[i] = max_rate_err_int[i];
}

float corr[3];
for(i = 0; i <= 2; i++){
  corr[i] = rate_err[i] * kpi[i] + rate_err_int[i] * kii[i];
  if(corr[i] > max_corr[i])
	corr[i] = max_corr[i];
}

uint32_t motor[4];
motor[0] = base_val + (uint32_t)corr[1] + (uint32_t)corr[2];
motor[1] = base_val + (uint32_t)corr[0] - (uint32_t)corr[2];
motor[2] = base_val - (uint32_t)corr[1] + (uint32_t)corr[2];
motor[3] = base_val - (uint32_t)corr[0] - (uint32_t)corr[2];
chprintf(bss, "Motor values = %d  %d  %d  %d\r\n", motor[0], motor[1], motor[2], motor[3]);
//chSysLockFromIsr() ;

startServo(SERVO2, motor[0]) ;
startServo(SERVO4, motor[1]) ;
startServo(SERVO5, motor[2]) ;
startServo(SERVO6, motor[3]) ;

//chSysUnlockFromIsr() ;

//while(previous_time == current_attitude.timestamp){
  chThdSleepMilliseconds(20);
//}
chprintf(bss,"Whew got after the while loop\r\n");
}
