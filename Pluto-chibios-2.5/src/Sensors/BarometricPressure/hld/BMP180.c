/*
 * BMP180.c
 *
 *  Created on: Apr 28, 2012
 *      Author: sapan
 */

#include "ch.h"
#include "hal.h"
#include "BMP180.h"
#include "chprintf.h"

uint8_t measurement_mode = 0x00, bmp_txbuf[30],bmp_rxbuf[30], oss = 0;
int16_t delay_bmp180 = 0, delay_bmp180_temperature = 0;

// states to use in calculation function
enum{
	TEMP = 1,
	PRESSURE = 2,
};

// Calibration coefficients
static int16_t  ac1=0, ac2=0, ac3=0, b1=0, b2=0, mb=0, mc=0, md=0;
static uint16_t ac4=0, ac5=0, ac6=0;

static uint32_t up = 0, ut = 0;
static uint32_t pval = 0;
static int32_t  tval = 0;

/* This function takes operating_mode as input and sets value of measurement_mode and
 * delay required between sending measurement command and reading data register.
 * 0 = ultra_low power mode.		delay = 6 ms (4.5 ms as per datasheet)
 * 1 = standard mode.				delay = 9 ms (7.5 ms as per datasheet)
 * 2 = high resoltuion mode.		delay = 15 ms (13.5 ms as per datasheet)
 * 3 = ultra high resolution mode.	delay = 27 ms (25.5 ms as per datasheet)
 *
 */

void initialize_bmp180(uint8_t operating_mode){

	delay_bmp180_temperature = 6; //4.5 ms as per datasheet
	oss = operating_mode;
	if(operating_mode == 0){
		measurement_mode = BOSCH_OSS0;
		delay_bmp180 = 6;
	}

	if(operating_mode == 1){
		measurement_mode = BOSCH_OSS1;
		delay_bmp180 = 9;
	}

	if(operating_mode == 2){
		measurement_mode = BOSCH_OSS2;
		delay_bmp180 = 15;
	}

	if(operating_mode == 3){
		measurement_mode = BOSCH_OSS3;
		delay_bmp180 = 27;
	}

	chprintf((BaseSequentialStream *)&SD1, "Inside initialize. measurement delay and oss %x, %d, %x \r\n", measurement_mode,delay_bmp180, oss);
}

void read_bmp180_coefficient(void){
  /* wait until sensor inits */
	chThdSleepMilliseconds(20); //(datasheet 10 ms)

  chprintf((BaseSequentialStream *)&SD1, "inside init_bmp \r\n");

  bmp_txbuf[0] = 0xAA;
   i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 22);

   ac1 = (bmp_rxbuf[0] << 8) + bmp_rxbuf[1];
   ac2 = (bmp_rxbuf[2] << 8) + bmp_rxbuf[3];
   ac3 = (bmp_rxbuf[4] << 8) + bmp_rxbuf[5];
   ac4 = (bmp_rxbuf[6] << 8) + bmp_rxbuf[7];
   ac5 = (bmp_rxbuf[8] << 8) + bmp_rxbuf[9];
   ac6 = (bmp_rxbuf[10] << 8) + bmp_rxbuf[11];
   b1 = (bmp_rxbuf[12] << 8) + bmp_rxbuf[13];
   b2 = (bmp_rxbuf[14] << 8) + bmp_rxbuf[15];
   mb = (bmp_rxbuf[16] << 8) + bmp_rxbuf[17];
   mc = (bmp_rxbuf[18] << 8) + bmp_rxbuf[19];
   md = (bmp_rxbuf[20] << 8) + bmp_rxbuf[21];

 	chprintf((BaseSequentialStream *)&SD1, "Value of ac1 %d \r\n", ac1);
	chprintf((BaseSequentialStream *)&SD1, "Value of ac2 %d \r\n", ac2);
	chprintf((BaseSequentialStream *)&SD1, "Value of ac3 %d \r\n", ac3);
	chprintf((BaseSequentialStream *)&SD1, "Value of ac4 %u \r\n", ac4);
	chprintf((BaseSequentialStream *)&SD1, "Value of ac5 %u \r\n", ac5);
	chprintf((BaseSequentialStream *)&SD1, "Value of ac6 %u \r\n", ac6);
	chprintf((BaseSequentialStream *)&SD1, "Value of b1 %d \r\n", b1);
	chprintf((BaseSequentialStream *)&SD1, "Value of b2 %d \r\n", b2);
	chprintf((BaseSequentialStream *)&SD1, "Value of mb %d \r\n", mb);
	chprintf((BaseSequentialStream *)&SD1, "Value of mc %d \r\n", mc);
	chprintf((BaseSequentialStream *)&SD1, "Value of md %d \r\n", md);


 }

void read_bmp180_temperature(){


//	chprintf((BaseSequentialStream *)&SD6,"Inside read_temperature \r\n");

	bmp_txbuf[0] = BOSCH_CTL;
    bmp_txbuf[1] = BOSCH_TEMP;
    i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 2, bmp_rxbuf, 0);

 //  chprintf((BaseSequentialStream *)&SD6,"Temp Measurement Command %x, %x\r\n", bmp_txbuf[0],bmp_txbuf[1]);

    /* wait temperature results (datasheet says 4.5 ms) */
    chThdSleepMilliseconds(5);

    /* read measured value */
    bmp_txbuf[0] = BOSCH_ADC_MSB;
    i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 2);
    bmp180_calc(TEMP);
  //  chprintf((BaseSequentialStream *)&SD6,"Temp Measurement Return %x, %x\r\n", bmp_rxbuf[0], bmp_rxbuf[1]);

  //  chprintf((BaseSequentialStream *)&SD6,"Temp Measurement Return %U\r\n", ut);


}

void read_bmp180_pressure(){

    // command to measure pressure
    bmp_txbuf[0] = BOSCH_CTL;
    bmp_txbuf[1] = measurement_mode;
    i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 2, bmp_rxbuf, 0);

    /* Wait untill pressure measurement is done. Time required for this depends on selected mode. */
    chThdSleepMilliseconds(delay_bmp180);

     //Read Pressure Bytes
    bmp_txbuf[0] = BOSCH_ADC_MSB;
    i2cMasterTransmit(&I2C_BMP,  BMP_ADDR, bmp_txbuf, 1, bmp_rxbuf, 3);
    bmp180_calc(PRESSURE);

//    chprintf((BaseSequentialStream *)&SD6,"Pressure Measurement Return %x, %x %x\r\n", bmp_rxbuf[0], bmp_rxbuf[1], bmp_rxbuf[2]);

    chprintf((BaseSequentialStream *)&SD1, "Temperature in 0.1deg and Pressure in Pascal is %D \t %U \r\n",tval, pval);

}




void bmp180_calc(int state){
  switch(state){
  case TEMP:
  	/* process termal data */
  	ut = (bmp_rxbuf[0] << 8) + bmp_rxbuf[1];
  	break;

  case PRESSURE:
  	/* now we have all 3 bytes. Calculate pressure using black magic from datasheet */
    up = ((bmp_rxbuf[0] << 16) + (bmp_rxbuf[1] << 8) + bmp_rxbuf[2]) >> (8 - oss);

  	int32_t  x1, x2, x3, b3, b5, b6, p;
  	uint32_t  b4, b7;

  	x1 = (ut - ac6) * ac5 >> 15;
  	x2 = ((int32_t) mc << 11) / (x1 + md);
  	b5 = x1 + x2;
  	tval = (b5 + 8) >> 4;

  	b6 = b5 - 4000;
  	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
  	x2 = ac2 * b6 >> 11;
  	x3 = x1 + x2;
  	b3 = ((((int32_t)ac1 * 4 + x3) << oss) + 2) >> 2;

  	x1 = ac3 * b6 >> 13;
  	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  	x3 = ((x1 + x2) + 2) >> 2;
  	b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
  	b7 = ((uint32_t)up - b3) * (50000 >> oss);
  	if(b7 < 0x80000000)
  		p = (b7 * 2) / b4;
  	else
  		p = (b7 / b4) * 2;

  	x1 = (p >> 8) * (p >> 8);
  	x1 = (x1 * 3038) >> 16;
  	x2 = (-7357 * p) >> 16;
  	pval = p + ((x1 + x2 + 3791) >> 4);

  	break;

  default:
  	break;
  }
}

