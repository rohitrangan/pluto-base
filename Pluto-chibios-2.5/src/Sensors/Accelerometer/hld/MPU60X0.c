/*
 * MPU60X0.c
 *
 *  Created on: May 1, 2012
 *      Author: sapan
 */

#include "mcuconf.h"
#include "chconf.h"
#include "ch.h"
#include "hal.h"
#include "MPU60X0.h"
#include "chprintf.h"
#include "plutoInit.h"

#define MPU_DEBUG
#define OUTPUT 		SD1

uint8_t smplrt_div= 0, mpu_config = 0, gyro_config = 0, accel_config = 0, fifo_enable = 0x00;
uint8_t int_pin_config = 0x00, int_pin_enable = 0x00, signal_path_reset = 0x00, user_control = 0x00;
uint8_t power_mgmt1 = 0x00, power_mgmt2  = 0x00, aux_vddio = 0x00;

#ifdef MPU_DEBUG
uint8_t debug_var = 0x00;
#endif

/*
 * This function sets up the MPU and the I2C driver.
 * MPU registers can be read thereafter.
 * Added by rohitrangan 31-05-2012
 */
void set_mpu_i2c(void) {

	uint8_t txbuf[10], rxbuf[10] ;
	msg_t status = RDY_OK ;

	txbuf[0] = PWR_MGMT_1 ;
	txbuf[1] = 0x01 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(100) ;

	txbuf[0] = GYRO_CONFIG ;
	txbuf[1] = 0x00 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(100) ;

	txbuf[0] = ACCEL_CONFIG ;
	txbuf[1] = 0x00 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(100) ;

	txbuf[0] = SMPRT_DIV ;
	txbuf[1] = 0x09 ;
	i2cAcquireBus(&I2C_MPU) ;
	status = i2cMasterTransmit(&I2C_MPU, MPU_ADDR, txbuf, 2, rxbuf, 0) ;
	i2cReleaseBus(&I2C_MPU) ;

	if(status != RDY_OK)
		return ;

	chThdSleepMilliseconds(100) ;

}

/*
 * This function defines value for SMPRT_DIV register.
 * Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
 */

uint8_t set_mpu_sample_rate(uint8_t samplerate_divisor){
	smplrt_div = samplerate_divisor;
#ifdef MPU_DEBUG
	debug_var = smplrt_div;
	chprintf((BaseSequentialStream *)&OUTPUT,"Sample rate value: %x\r\n", debug_var);
#endif
	return smplrt_div;
}

/*
 * This function sets value for CONFIG register. This register controls FSYNC and bandwidth of gyro and
 * accelerometer.
 * Typical function call: set_mpu_config_register(EXT_SYNC_SET0, DLPF_CFG0);
 */
uint8_t set_mpu_config_regsiter(uint8_t ext_sync_set, uint8_t dlpf_cfg){
	mpu_config = 0x00;
	mpu_config = ext_sync_set | dlpf_cfg;
#ifdef MPU_DEBUG
	debug_var = mpu_config;
	chprintf((BaseSequentialStream *)&OUTPUT,"CONFIG Register value: %x\r\n", debug_var);
#endif
	return mpu_config;
}

/*
 * This function defines value for GYRO_CONFIG register. This register controls
 * self test and  range of gyroscopes.
 * Typical function call: set_mpu_gyro(XG_ST_EN, YG_ST_EN, ZG_ST_EN, FS_SEL250)
 */
uint8_t set_mpu_gyro(uint8_t xgyro_st, uint8_t ygyro_st, uint8_t zgyro_st, uint8_t gyro_range){
	gyro_config = 0x00;
	gyro_config = xgyro_st | ygyro_st | zgyro_st | gyro_range;
#ifdef MPU_DEBUG
	debug_var = gyro_config;
	chprintf((BaseSequentialStream *)&OUTPUT,"Gyro Config value: %x\r\n", debug_var);
#endif
	return gyro_config;
}

/*
 * This function defines value for ACCEL_CONFIG register. This register controls
 * self test, accelerometer range and DHPF for accelerometer.
 * Typical function call: set_mpu_accel(XA_ST_EN/DIS, YA_ST_EN/DIS, ZA_ST_EN/DIS, AFS_SEL0, ACCEL_HPF0)
 */
uint8_t set_mpu_accel(uint8_t xaccel_st, uint8_t yaccel_st, uint8_t zaccel_st, uint8_t accel_range, uint8_t dhpf_accel){
	accel_config = 0x00;
	accel_config = xaccel_st | yaccel_st | zaccel_st | accel_range | dhpf_accel;
#ifdef MPU_DEBUG
	debug_var = accel_config;
	chprintf((BaseSequentialStream *)&OUTPUT,"Accelerometer Config value: %x\r\n", debug_var);
#endif
	return accel_config;
}

/*
 * This function defines value for FIFO_EN register. This register controls which sensor output to be
 * written in fifo regiser.
 * Typical funciton call: set_mpu_fifo_register(TEMP_FIFO_EN/DIS, XG_FIFO_EN/DIS, YG_FIFO_EN/DIS, ACCLE_FIFO_EN/DIS, SLVx_FIFO_EN/DIS...)
 */
uint8_t set_mpu_fifo_register(uint8_t temperature_fifo, uint8_t xg_fifo, uint8_t yg_fifo, uint8_t zg_fifo, uint8_t accel_fifo, uint8_t slv2_fifo, uint8_t slv1_fifo,uint8_t slv0_fifo){
	fifo_enable = 0x00;
	fifo_enable = temperature_fifo | xg_fifo | yg_fifo | zg_fifo | accel_fifo | slv2_fifo | slv1_fifo | slv0_fifo;
#ifdef MPU_DEBUG
	debug_var = fifo_enable;
	chprintf((BaseSequentialStream *)&OUTPUT,"FIFO Enable value: %x\r\n", debug_var);
#endif
	return fifo_enable;
}

/*
 * This function defines value for INT_PIN_CFG register. This controls behavior of Interrupt PIN
 */
uint8_t set_mpu_interrupt_behavior(uint8_t int_level, uint8_t int_pin_mode, uint8_t latch_int, uint8_t int_status_bits, uint8_t fsync_level, uint8_t fsync_enable, uint8_t i2c_bypass, uint8_t clock){
	int_pin_config = 0x00;
	int_pin_config = int_level | int_pin_mode | latch_int | int_status_bits | fsync_level | fsync_enable | i2c_bypass |clock;
#ifdef MPU_DEBUG
	debug_var = int_pin_config;
	chprintf((BaseSequentialStream *)&OUTPUT,"Interrupt behavior value: %x\r\n", debug_var);
#endif
	return int_pin_config;
}

/*
 * This function defines value for INT_ENABLE register. This register controls source of interrupt.
 * Typical function call: set_mpu_interrupt_sources(FF_EN/DIS, MOT_EN/DIS,.......,DATA_RDY_EN/DIS)
 */
uint8_t set_mpu_interrupt_source(uint8_t free_fall, uint8_t motion_threshold, uint8_t zero_motion, uint8_t fifo_overflow, uint8_t i2c_mst, uint8_t data_ready){
	int_pin_enable = 0x00;
	int_pin_enable = free_fall | motion_threshold | zero_motion | fifo_overflow | i2c_mst | data_ready;
#ifdef MPU_DEBUG
	debug_var = int_pin_enable;
	chprintf((BaseSequentialStream *)&OUTPUT,"Interrupt Pin Source value value: %x\r\n", debug_var);
#endif
	return int_pin_enable;
}

/*
 * This function defines value for SIGNAL_PATH_RESET register. This register can reset gyro, accelerometer and
 * temperature sensors' digital and analog signal path.
 * Typical function call: reset_mpu_signal_path(GYRO_RESET_EN/DIS, ACCEL_RESET_EN/DIS, TEMP_RESET_EN/DIS)
 */
uint8_t reset_mpu_signal_path(uint8_t gyro_reset, uint8_t accel_reset, uint8_t temperature_reset){
	signal_path_reset = 0x00;
	signal_path_reset = gyro_reset | accel_reset | temperature_reset;
#ifdef MPU_DEBUG
	debug_var = signal_path_reset;
	chprintf((BaseSequentialStream *)&OUTPUT,"Signal Path Reset value: %x\r\n", debug_var);
#endif
	return signal_path_reset;
}

/*
 * This function defines value for USER_CTRL register.
 * Typical Function Call: set_mpu_user_control(USER_FIFO_EN/DIS,I2C_MST_EN/DIS,I2C_IF_EN/DIS,FIFO_RESET_EN/DIS,I2C_MST_RESET_EN/DIS,SIG_COND_RESET_EN/DIS);
 */
uint8_t set_mpu_user_control(uint8_t fifo_operation, uint8_t aux_i2c, uint8_t bus_select, uint8_t fifo_reset, uint8_t i2c_reset, uint8_t signal_cond_reset){
	user_control = 0x00;
	user_control = fifo_operation | aux_i2c | bus_select | fifo_reset | i2c_reset | signal_cond_reset;
#ifdef MPU_DEBUG
	debug_var = user_control;
	chprintf((BaseSequentialStream *)&OUTPUT,"User Control value: %x\r\n", debug_var);
#endif
	return user_control;
}

/*
 * This fucntion defines value for PWR_MGMT_1 register. This register controls device reset, sleep mode, cycle
 * between different mode and clock source.
 * Typical Function Call: set_mput_power_mgmt1(DEVICE_RESET_EN/DIS, SLEEP_EN/DIS, CYCLE_EN/DIS, TEMPERATURE_EN/DIS, CLKSEL_XG)
 *
 */
uint8_t set_mpu_power_mgmt1(uint8_t device_reset, uint8_t sleep, uint8_t cycle, uint8_t temperature, uint8_t clock_source){
	power_mgmt1 = 0x00;
	power_mgmt1 = device_reset | sleep | cycle | temperature | clock_source;
#ifdef MPU_DEBUG
	debug_var = power_mgmt1;
	chprintf((BaseSequentialStream *)&OUTPUT,"Power Managemen1 value: %x\r\n", debug_var);
#endif
	return power_mgmt1;
}

void write_mpu_sample_rate(void){
	 mpu_i2c_write(SMPRT_DIV, smplrt_div);
}

void write_mpu_config_register(){
	mpu_i2c_write(CONFIG, mpu_config);
}

void write_mpu_gyro(void){
	mpu_i2c_write(GYRO_CONFIG, gyro_config);
}

void write_mpu_accel(void){
	mpu_i2c_write(ACCEL_CONFIG, accel_config);
}

void write_mpu_power_mgmt1(void){
	mpu_i2c_write(PWR_MGMT_1, power_mgmt1);
}
void write_mpu_user_control(void){
	mpu_i2c_write(USER_CTRL, user_control);
}

void mpu_i2c_write(uint8_t addr, uint8_t value){
	uint8_t mpu_txbuf[10], mpu_rxbuf[10];
	mpu_txbuf[0] = addr;
	mpu_txbuf[1] = value;
#ifdef MPU_DEBUG
	chprintf((BaseSequentialStream *)&OUTPUT,"Address: %x Value: %x\r\n", mpu_txbuf[0],mpu_txbuf[1]);
#endif
	i2cAcquireBus(&I2C_MPU) ;
	i2cMasterTransmit(&I2C_MPU, MPU_ADDR, mpu_txbuf, 2, mpu_rxbuf, 0);
	i2cReleaseBus(&I2C_MPU) ;
	chThdSleepMilliseconds(200);
}


