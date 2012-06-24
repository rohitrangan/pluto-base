/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the shell commands and shell
 *structures required to start the PLUTO shell.
 */
/* 18-06-2012 - Riya Ray - The functions fpuTest and cmd_fputest have been added */

#include <string.h>
#include "plutoconf.h"

#include "ch.h"
#include "ff.h"
#include "fsInit.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "shellCommands.h"
#include "BarometerData.h"
#include "MagnetometerData.h"
#include "IMUData.h"

#if PLUTO_USE_SHELL
const ShellCommand commands[] = {
	{"mem", cmd_mem},
	{"threads", cmd_threads},
#if CORTEX_USE_FPU
	{"fputest", cmd_fputest},
#endif   /*CORTEX_USE_FPU */
#if PLUTO_USE_FATFS
	{"mount", cmd_mount},
	{"unmount", cmd_unmount},
	{"tree", cmd_tree},
#endif	/*PLUTO_USE_FATFS */
	{"clear", cmd_clear},
#if PLUTO_USE_IMU
	{"imu", cmd_imu},
#endif	/*PLUTO_USE_IMU */
#if PLUTO_USE_MAGNETOMETER
	{"magnetometer", cmd_magnetometer},
#endif	/*PLUTO_USE_MAGNETOMETER */
#if PLUTO_USE_BAROMETER
	{"barometer", cmd_barometer},
#endif	/*PLUTO_USE_BAROMETER */
	{NULL, NULL}
};

const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

#if CORTEX_USE_FPU
void cmd_fputest(BaseSequentialStream *bss, int argc, char *argv[]) {
	float ans = 0.0;
	if((argc > 3) || (argc == 0)) {
		chprintf(bss, "Usage: fputest [number1] [operator] [number2]\r\nOperators:\r\n") ;
		chprintf(bss, "  +\tAddition.\r\n") ;
		chprintf(bss, "  -\tSubtraction.\r\n") ;
		chprintf(bss, "  *\tMultiplication.\r\n") ;
		chprintf(bss, "  /\tDivision.\r\n") ;
		chprintf(bss, "  s\tSine of the number(in degrees).\r\n") ;
		chprintf(bss, "  c\tCosine of the number(in degrees).\r\n") ;
		chprintf(bss, "  t\tTangent of the number(in degrees).\r\n") ;
		chprintf(bss, "  p\tPower of [number1] to [number2].\r\n") ;
		chprintf(bss, "  r\tSquare root of the number.\r\n") ;
		chprintf(bss, "  l\tLogarithm of [number2] to the base of [number1].\r\n") ;
		chprintf(bss, "  Note that the operations s, c, t, r only require [number1].\r\n") ;
		return;
	}
	if((!strcasecmp(argv[0], "--help")) || (!strcasecmp(argv[0], "-h"))) {
		chprintf(bss, "Usage: fputest [number1] [operator] [number2]\r\nOperators:\r\n") ;
		chprintf(bss, "  +\tAddition.\r\n") ;
		chprintf(bss, "  -\tSubtraction.\r\n") ;
		chprintf(bss, "  *\tMultiplication.\r\n") ;
		chprintf(bss, "  /\tDivision.\r\n") ;
		chprintf(bss, "  s\tSine of the number(in degrees).\r\n") ;
		chprintf(bss, "  c\tCosine of the number(in degrees).\r\n") ;
		chprintf(bss, "  t\tTangent of the number(in degrees).\r\n") ;
		chprintf(bss, "  p\tPower of [number1] to [number2].\r\n") ;
		chprintf(bss, "  r\tSquare root of the number.\r\n") ;
		chprintf(bss, "  l\tLogarithm of [number2] to the base of [number1].\r\n") ;
		chprintf(bss, "  Note that the operations s, c, t, r only require [number1].\r\n") ;
	}
	else {
		ans = fpuTest(bss, argv[0], argv[1][0], argv[2]);
		chprintf(bss, "The answer is %f\r\n", ans);
	}
}
#endif	/*CORTEX_USE_FPU  */

void cmd_mem(BaseSequentialStream *bss, int argc, char *argv[]) {
	size_t n, size;

	(void)argv;
	if(argc > 0) {
		chprintf(bss, "Usage: mem\r\n");
		return;
	}
	n = chHeapStatus(NULL, &size);
	chprintf(bss, "Core free memory : %u bytes\r\n", chCoreStatus());
	chprintf(bss, "Heap fragments   : %u\r\n", n);
	chprintf(bss, "Heap free total  : %u bytes\r\n", size);
}

void cmd_threads(BaseSequentialStream *bss, int argc, char *argv[]) {
	static const char *states[] = {THD_STATE_NAMES};
	Thread *tp;

	(void)argv;
	if(argc > 0) {
		chprintf(bss, "Usage: threads\r\n");
		return;
	}
	chprintf(bss, "    addr    stack prio refs     state time\r\n");
	tp = chRegFirstThread();
	do {
		chprintf(bss, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
				(uint32_t)tp, (uint32_t)tp->p_ctx.r13,
				(uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
				states[tp->p_state], (uint32_t)tp->p_time);
		tp = chRegNextThread(tp);
	}while(tp != NULL);
}

#if PLUTO_USE_FATFS
void cmd_mount(BaseSequentialStream *bss, int argc, char *argv[]) {
	(void)argv;
	if(argc > 0) {
		chprintf(bss, "Usage: mount\r\n");
		return;
	}
	mmcConnect(&MMCD1);
	FRESULT err;
	err = f_mount(0, &MMC_FS);
	if(err != FR_OK) {
		chprintf(bss, "Disk not mounted. Error - %d\r\n", err);
		mmcDisconnect(&MMCD1);
	}
	else {
		chprintf(bss, "Disk mounted.\r\n");
		fs_ready = TRUE;
	}
}

void cmd_unmount(BaseSequentialStream *bss, int argc, char *argv[]) {
	(void)argv;
	if(argc > 0) {
		chprintf(bss, "Usage: unmount\r\n");
		return;
	}
	mmcDisconnect(&MMCD1);
	fs_ready = FALSE;
	chprintf(bss, "Disk unmounted.\r\n");
}

void cmd_tree(BaseSequentialStream *bss, int argc, char *argv[]) {
	FRESULT err;
	uint32_t clusters;
	FATFS *fsp;

	(void)argv;
	if(argc > 0) {
		chprintf(bss, "Usage: tree\r\n");
		return;
	}
	if(!fs_ready) {
		chprintf(bss, "File System not mounted\r\n");
		return;
	}
	err = f_getfree("/", &clusters, &fsp);
	if(err != FR_OK) {
		chprintf(bss, "FS: f_getfree() failed\r\n");
		return;
	}
	chprintf(bss,
			"FS: %lu free clusters, %lu sectors per cluster, %lu bytes free\r\n",
			clusters, (uint32_t)MMC_FS.csize,
			clusters * (uint32_t)MMC_FS.csize * (uint32_t)MMC_SECTOR_SIZE);
	fbuff[0] = 0;
	err = scan_files(bss, (char *)fbuff);
}
#endif	/*PLUTO_USE_FATFS */

void cmd_clear(BaseSequentialStream *bss, int argc, char *argv[]) {

	(void)argv ;
	if(argc > 0) {
		chprintf(bss, "Usage: clear\r\n") ;
		return ;
	}
	chprintf(bss, "\f") ;

}

#if PLUTO_USE_IMU
void cmd_imu(BaseSequentialStream *bss, int argc, char *argv[]) {

	uint8_t i ;
	int16_t data[3] ;

	if(argc != 1) {
		chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
		chprintf(bss, " --accel\t| -a\tAccelerometer's value.\r\n") ;
		chprintf(bss, " --gyro \t| -g\tGyrometer's value.\r\n") ;
		chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
		return ;
	}
	if((!strcasecmp("--temp", argv[0])) || (!strcasecmp("-t", argv[0]))) {
		for(i = 0 ; i < 50 ; i++) {
			readIMUData(3, data) ;
			chprintf(bss, "Temperature Sensor Value:- %d\r\n", data[0]) ;
			chThdSleepMilliseconds(100) ;
		}
	}
	else if((!strcasecmp("--accel", argv[0])) || (!strcasecmp("-a", argv[0]))) {
		for(i = 0 ; i < 50 ; i++) {
			readIMUData(1, data) ;
			chprintf(bss, "Accelerometer Value:- %d  %d  %d\r\n", data[0], data[1], data[2]) ;
			chThdSleepMilliseconds(100) ;
		}
	}
	else if((!strcasecmp("--gyro", argv[0])) || (!strcasecmp("-g", argv[0]))) {
		for(i = 0 ; i < 50 ; i++) {
			readIMUData(2, data) ;
			chprintf(bss, "Gyrometer Value:- %d  %d  %d\r\n", data[0], data[1], data[2]) ;
			chThdSleepMilliseconds(100) ;
		}
	}
#if CORTEX_USE_FPU
	else if((!strcasecmp("--angle", argv[0])) || (!strcasecmp("-n", argv[0]))) {
		float angles[3] ;
		for(i = 0 ; i < 50 ; i++) {
			eulerAngles(angles) ;
			chprintf(bss, "Angles Value:- %f", angles[0]) ;
			chprintf(bss, " %f", angles[1]) ;
			chprintf(bss, " %f\r\n", angles[2]) ;
			chThdSleepMilliseconds(100) ;
		}
	}
#endif	/*CORTEX_USE_FPU */
	else if((!strcasecmp("--help", argv[0])) || (!strcasecmp("-h", argv[0]))) {
		chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
		chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
		chprintf(bss, " --accel\t| -a\tAccelerometer's value.\r\n") ;
		chprintf(bss, " --gyro \t| -g\tGyrometer's value.\r\n") ;
		chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
	}
	else {
		chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
		chprintf(bss, " --accel\t| -a\tAccelerometer's value.\r\n") ;
		chprintf(bss, " --gyro \t| -g\tGyrometer's value.\r\n") ;
		chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
	}

}
#endif	/*PLUTO_USE_IMU */

#if PLUTO_USE_MAGNETOMETER
void cmd_magnetometer(BaseSequentialStream *bss, int argc, char *argv[]) {

	int16_t data[3] ;
	uint8_t i ;

	if(argc != 1) {
		chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
		chprintf(bss, " --tesla\t| -t\tDisplay value in nanoTesla.\r\n") ;
		return ;
	}
	if((!strcasecmp(argv[0], "--raw")) || (!strcasecmp(argv[0], "-r"))) {
		for(i = 0 ; i < 50 ; i++) {
			readMagnetometerData(data) ;
			chprintf(bss, "Magnetometer Value:- %d %d %d\r\n", data[0], data[1], data[2]) ;
			chThdSleepMilliseconds(100) ;
		}
	}
	else if((!strcasecmp(argv[0], "--tesla")) || (!strcasecmp(argv[0], "-t"))) {
		for(i = 0 ; i < 50 ; i++) {
			readMagnetometerData(data) ;
			chprintf(bss, "Magnetometer Value:- %D %D %D\r\n", (data[0] * RANGE / 10), (data[1] * RANGE / 10), (data[2] * RANGE / 10)) ;
			chThdSleepMilliseconds(100) ;
		}
	}
	else if((!strcasecmp(argv[0], "--help")) || (!strcasecmp(argv[0], "-h"))) {
		chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
		chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
		chprintf(bss, " --tesla\t| -t\tDisplay value in nanoTesla.\r\n") ;
		return ;
	}
	else {
		chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
		chprintf(bss, " --tesla\t| -t\tDisplay value in nanoTesla.\r\n") ;
		return ;
	}

}
#endif	/*PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_BAROMETER
void cmd_barometer(BaseSequentialStream *bss, int argc, char *argv[]) {

	uint8_t i ;

	if(argc != 1) {
		chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
		chprintf(bss, " --temp \t| -t\tDisplay Temperature in 0.1 Degrees Celsius.\r\n") ;
		return ;
	}
	if((!strcasecmp("--temp", argv[0])) || (!strcasecmp("-t", argv[0]))) {
		for(i = 0 ; i < 50 ; i++) {
			chprintf(bss, "Temperature Value:- %D\r\n", readBarometerData(1)) ;
		}
	}
	else if((!strcasecmp("--press", argv[0])) || (!strcasecmp("-p", argv[0]))) {
		for(i = 0 ; i < 50 ; i++) {
			chprintf(bss, "Pressure Value:- %D\r\n", readBarometerData(2)) ;
		}
	}
	else if((!strcasecmp("--help", argv[0])) || (!strcasecmp("-h", argv[0]))) {
		chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
		chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
		chprintf(bss, " --temp \t| -t\tDisplay Temperature in 0.1 Degrees Celsius.\r\n") ;
	}
	else {
		chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
		chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
		chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
		chprintf(bss, " --temp \t| -t\tDisplay Temperature in 0.1 Degrees Celsius.\r\n") ;
	}

}
#endif	/*PLUTO_USE_BAROMETER */

#endif	/*PLUTO_USE_SHELL */
