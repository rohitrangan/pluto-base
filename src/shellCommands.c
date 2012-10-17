/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the shell commands and shell
 *structures required to start the PLUTO shell.
 */

#include<stdlib.h>
#include<string.h>

#include "pluto.h"

extern float kpi[3];
extern float kii[3];
extern float kpo[3];
extern float kio[3];
extern current_attitude_struct current_attitude; /* declared in dcm_update.c */

#if PLUTO_USE_ZIGBEE
/* Serial Driver for Output. */
#define OUTPUT					SD5
#else
#define OUTPUT					SD1
#endif /* PLUTO_USE_ZIGBEE*/

#if PLUTO_USE_SHELL
const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
#if CORTEX_USE_FPU && PLUTO_USE_FPUTEST
  {"fputest", cmd_fputest},
#endif   /*CORTEX_USE_FPU && PLUTO_USE_FPUTEST */
#if PLUTO_USE_FATFS
  {"mount", cmd_mount},
  {"unmount", cmd_unmount},
  {"tree", cmd_tree},
#endif  /*PLUTO_USE_FATFS */
  {"clear", cmd_clear},
#if PLUTO_USE_IMU
  {"imu", cmd_imu},
#endif  /*PLUTO_USE_IMU */
#if PLUTO_USE_MAGNETOMETER
  {"magnetometer", cmd_magnetometer},
#endif  /*PLUTO_USE_MAGNETOMETER */
#if PLUTO_USE_BAROMETER
  {"barometer", cmd_barometer},
#endif  /*PLUTO_USE_BAROMETER */
#if HAL_USE_PWM
  {"pwm", cmd_pwm},
#endif	/*HAL_USE_PWM */
#if HAL_USE_ICU
  {"icu", cmd_icu},
#endif	/*HAL_USE_ICU */
#if PLUTO_USE_DCM
  {"attitude", cmd_attitude}, /* print current attitude roll, pitch, yaw number of times */
  {"set_gain_kpi", cmd_set_gain_kpi}, /* proportional gain inner loop roll, pitch, yaw */
  {"set_gain_kii", cmd_set_gain_kii}, /* integral gain inner loop roll, pitch, yaw */
  {"set_gain_kpo", cmd_set_gain_kpo}, /* proportional gain outer loop roll, pitch, yaw */
  {"set_gain_kio", cmd_set_gain_kio}, /* integral gain outer loop roll, pitch, yaw */
#endif /* PLUTO_USE_DCM */
  {NULL, NULL}
};

const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&OUTPUT,
  commands
};

#if CORTEX_USE_FPU && PLUTO_USE_FPUTEST
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
#endif  /*CORTEX_USE_FPU && PLUTO_USE_FPUTEST */

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
    chThdSleepMilliseconds(10);
  }
  else {
    chprintf(bss, "Disk mounted.\r\n");
    fs_ready = TRUE;
    chThdSleepMilliseconds(10);
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
  chThdSleepMilliseconds(10);
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
  chThdSleepMilliseconds(10);
  fbuff[0] = 0;
  err = scan_files(bss, (char *)fbuff);
  chThdSleepMilliseconds(10);
}
#endif  /*PLUTO_USE_FATFS */

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

  uint16_t i ;

  if(argc != 1) {
	  chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
	  chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
	  chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
	  chprintf(bss, " --accel\t| -a\tValue of acceleration in g's.\r\n") ;
	  chprintf(bss, " --gyro \t| -g\tValue of Angular Speed in rad/s.\r\n") ;
	  chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
    return ;
  }
  if((!strcasecmp("--temp", argv[0])) || (!strcasecmp("-t", argv[0]))) {
    for(i = 0 ; i < 50 ; i++) {
      readIMUData(IMU_TEMP_DATA, &IMUD1) ;
      chprintf(bss, "Temperature Sensor Value:- %d\r\n", IMUD1.RAW_TEMP) ;
      chThdSleepMilliseconds(100) ;
    }
  }
  else if((!strcasecmp("--accel", argv[0])) || (!strcasecmp("-a", argv[0]))) {
    for(i = 0 ; i < 50 ; i++) {
      readIMUData(ACCEL_DATA, &IMUD1) ;
      chprintf(bss, "Accelerometer Value:- %f g ", IMUD1.ACCEL_X) ;
      chprintf(bss, " %f g ", IMUD1.ACCEL_Y) ;
      chprintf(bss, " %fg\r\n", IMUD1.ACCEL_Z) ;
      chThdSleepMilliseconds(100) ;
    }
  }
  else if((!strcasecmp("--gyro", argv[0])) || (!strcasecmp("-g", argv[0]))) {
    for(i = 0 ; i < 50 ; i++) {
      readIMUData(GYRO_DATA, &IMUD1) ;
      chprintf(bss, "Gyrometer Value:- %f rad/s", IMUD1.GYRO_X) ;
      chprintf(bss, " %f rad/s ", IMUD1.GYRO_Y) ;
      chprintf(bss, " %f rad/s\r\n", IMUD1.GYRO_Z) ;
      chThdSleepMilliseconds(100) ;
    }
  }
  else if((!strcasecmp("--angle", argv[0])) || (!strcasecmp("-n", argv[0]))) {
    float angles[3] ;
    for(i = 0 ; i < 50 ; i++) {
      eulerAngles(&IMUD1, angles) ;
      chprintf(bss, "Angles Value:- %f", angles[0]) ;
      chprintf(bss, " %f", angles[1]) ;
      chprintf(bss, " %f\r\n", angles[2]) ;
      chThdSleepMilliseconds(100) ;
    }
  }
  else if((!strcasecmp("--help", argv[0])) || (!strcasecmp("-h", argv[0]))) {
    chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
    chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
    chprintf(bss, " --accel\t| -a\tValue of acceleration in g's.\r\n") ;
    chprintf(bss, " --gyro \t| -g\tValue of Angular Speed in rad/s.\r\n") ;
    chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
  }
  else {
	  chprintf(bss, "Usage: imu [options]\r\nOptions:\r\n") ;
	  chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
	  chprintf(bss, " --temp \t| -t\tTemperature Sensor's value.\r\n") ;
	  chprintf(bss, " --accel\t| -a\tValue of acceleration in g's.\r\n") ;
	  chprintf(bss, " --gyro \t| -g\tValue of Angular Speed in rad/s.\r\n") ;
	  chprintf(bss, " --angle\t| -n\tEuler angles value. First pitch, then roll and yaw.\r\n") ;
  }

}
#endif  /*PLUTO_USE_IMU */

#if PLUTO_USE_MAGNETOMETER
void cmd_magnetometer(BaseSequentialStream *bss, int argc, char *argv[]) {

  uint16_t i ;

  if(argc != 1) {
    chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
    chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
    chprintf(bss, " --tesla\t| -t\tDisplay value in uT.\r\n") ;
    chprintf(bss, " --head \t| -a\tDisplay heading in degrees.\r\n") ;
    return ;
  }
  if((!strcasecmp(argv[0], "--raw")) || (!strcasecmp(argv[0], "-r"))) {
    for(i = 0 ; i < 50 ; i++) {
      readMagnetometerData(&MD1) ;
      chprintf(bss, "Magnetometer Value:- %d %d %d\r\n", MD1.RAW_MAG_X, MD1.RAW_MAG_Y, MD1.RAW_MAG_Z) ;
      chThdSleepMilliseconds(100) ;
    }
  }
  else if((!strcasecmp(argv[0], "--tesla")) || (!strcasecmp(argv[0], "-t"))) {
    for(i = 0 ; i < 50 ; i++) {
      readMagnetometerData(&MD1) ;
      chprintf(bss, "Magnetometer Value:- %f uT", MD1.MAG_X) ;
      chprintf(bss, " %f uT", MD1.MAG_Y) ;
      chprintf(bss, " %f uT\r\n", MD1.MAG_Z) ;
      chThdSleepMilliseconds(100) ;
    }
  }
#if PLUTO_USE_IMU
  else if((!strcasecmp(argv[0], "--head")) || (!strcasecmp(argv[0], "-a"))) {
	float heading ;
    for(i = 0 ; i < 50 ; i++) {
    	heading = getHeading(&IMUD1, &MD1) ;
    	chprintf(bss, "Heading :- %f\r\n", heading) ;
    	chThdSleepMilliseconds(100) ;
    }
  }
#endif	/*PLUTO_USE_IMU */
  else if((!strcasecmp(argv[0], "--help")) || (!strcasecmp(argv[0], "-h"))) {
    chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
    chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
    chprintf(bss, " --tesla\t| -t\tDisplay value in uT.\r\n") ;
    chprintf(bss, " --head \t| -a\tDisplay heading in degrees.\r\n") ;
    return ;
  }
  else {
    chprintf(bss, "Usage: magnetometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
    chprintf(bss, " --raw  \t| -r\tDisplay raw data.\r\n") ;
    chprintf(bss, " --tesla\t| -t\tDisplay value in uT.\r\n") ;
    chprintf(bss, " --head \t| -a\tDisplay heading in degrees.\r\n") ;
    return ;
  }

}
#endif  /*PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_BAROMETER
void cmd_barometer(BaseSequentialStream *bss, int argc, char *argv[]) {

  uint16_t i ;

  if(argc != 1) {
    chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
    chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
    chprintf(bss, " --temp \t| -t\tDisplay Temperature in Degrees Celsius.\r\n") ;
    chprintf(bss, " --alt  \t| -a\tDisplay Absolute Altitude in meters.\r\n") ;
    return ;
  }
  if((!strcasecmp("--temp", argv[0])) || (!strcasecmp("-t", argv[0]))) {
    for(i = 0 ; i < 50 ; i++) {
      readBarometerData(&BD1, BARO_TEMP_DATA) ;
      chprintf(bss, "Temperature Value:- %f C\r\n", BD1.TEMP) ;
    }
  }
  else if((!strcasecmp("--press", argv[0])) || (!strcasecmp("-p", argv[0]))) {
    for(i = 0 ; i < 50 ; i++) {
      readBarometerData(&BD1, BARO_PRESSURE_DATA) ;
      chprintf(bss, "Pressure Value:- %f", BD1.PRESSURE) ;
      chprintf(bss, " Pa\r\n") ;
    }
  }
  else if((!strcasecmp("--alt", argv[0])) || (!strcasecmp("-a", argv[0]))) {
	for(i = 0 ; i < 50 ; i++) {
      chprintf(bss, "Altitude:- %f m\r\n", getAltitude(&BD1)) ;
    }
  }
  else if((!strcasecmp("--help", argv[0])) || (!strcasecmp("-h", argv[0]))) {
    chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay this help.\r\n") ;
    chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
    chprintf(bss, " --temp \t| -t\tDisplay Temperature in Degrees Celsius.\r\n") ;
    chprintf(bss, " --alt  \t| -a\tDisplay Absolute Altitude in meters.\r\n") ;
  }
  else {
    chprintf(bss, "Usage: barometer [options]\r\nOptions:\r\n") ;
    chprintf(bss, " --help \t| -h\tDisplay help.\r\n") ;
    chprintf(bss, " --press\t| -p\tDisplay Pressure in Pascals.\r\n") ;
    chprintf(bss, " --temp \t| -t\tDisplay Temperature in Degrees Celsius.\r\n") ;
    chprintf(bss, " --alt  \t| -a\tDisplay Absolute Altitude in meters.\r\n") ;
  }

}
#endif  /*PLUTO_USE_BAROMETER */

#if HAL_USE_PWM
void cmd_pwm(BaseSequentialStream *bss, int argc, char *argv[]) {

  if(argc < 1) {
	chprintf(bss, "Usage: pwm [options] [servo_number] [pulse_width]\r\nOptions:\r\n") ;
	chprintf(bss, " --help   \t| -h\tDisplay help.\r\n") ;
	chprintf(bss, " --enable \t| -e\tEnable the given Servo with a pulse width.\r\n") ;
	chprintf(bss, " --disable\t| -d\tDisable the given Servo.\r\n\n") ;
	chprintf(bss, " servo_number range:- 1 - 6\r\n\n") ;
	chprintf(bss, " pulse_width  range:- 1000 - 2000\r\n\n") ;
	return ;
  }
  if((!strcasecmp("--enable", argv[0])) || (!strcasecmp("-e", argv[0]))) {
	if(argc != 3) {
	  chprintf(bss, "Usage: pwm [options] [servo_number] [pulse_width]\r\nOptions:\r\n") ;
	  chprintf(bss, " --help   \t| -h\tDisplay help.\r\n") ;
	  chprintf(bss, " --enable \t| -e\tEnable the given Servo with a pulse width.\r\n") ;
	  chprintf(bss, " --disable\t| -d\tDisable the given Servo.\r\n\n") ;
	  chprintf(bss, " servo_number range:- 1 - 6\r\n\n") ;
	  chprintf(bss, " pulse_width  range:- 1000 - 2000\r\n\n") ;
	  return ;
	}
	int servo_no = atoi(argv[1]) ;
	int width    = atoi(argv[2]) ;
	if((servo_no < 1) || (servo_no > 6)) {
	  chprintf(bss, "Illegal Value for [servo_number]\r\n") ;
	  return ;
	}
	startServo(servo_no, (uint32_t)width) ;
  }
  else if((!strcasecmp("--disable", argv[0])) || (!strcasecmp("-d", argv[0]))) {
	if(argc != 2) {
	  chprintf(bss, "Usage: pwm [options] [servo_number] [pulse_width]\r\nOptions:\r\n") ;
	  chprintf(bss, " --help   \t| -h\tDisplay help.\r\n") ;
	  chprintf(bss, " --enable \t| -e\tEnable the given Servo with a pulse width.\r\n") ;
	  chprintf(bss, " --disable\t| -d\tDisable the given Servo.\r\n\n") ;
	  chprintf(bss, " servo_number range:- 1 - 6\r\n\n") ;
      chprintf(bss, " pulse_width  range:- 1000 - 2000\r\n\n") ;
	  return ;
	}
	int servo_no = atoi(argv[1]) ;
	if((servo_no < 1) || (servo_no > 6)) {
	  chprintf(bss, "Illegal Value for [servo_number]\r\n") ;
	  return ;
	}
	stopServo(servo_no) ;
  }
  else if((!strcasecmp("--help", argv[0])) || (!strcasecmp("-h", argv[0]))) {
	chprintf(bss, "Usage: pwm [options] [servo_number] [pulse_width]\r\nOptions:\r\n") ;
	chprintf(bss, " --help   \t| -h\tDisplay this help.\r\n") ;
	chprintf(bss, " --enable \t| -e\tEnable the given Servo with a pulse width.\r\n") ;
	chprintf(bss, " --disable\t| -d\tDisable the given Servo.\r\n\n") ;
	chprintf(bss, " servo_number range:- 1 - 6\r\n\n") ;
	chprintf(bss, " pulse_width  range:- 1000 - 2000\r\n\n") ;
  }
  else {
	chprintf(bss, "Usage: pwm [options] [servo_number] [pulse_width]\r\nOptions:\r\n") ;
	chprintf(bss, " --help   \t| -h\tDisplay this help.\r\n") ;
	chprintf(bss, " --enable \t| -e\tEnable the given Servo with a pulse width.\r\n") ;
	chprintf(bss, " --disable\t| -d\tDisable the given Servo.\r\n\n") ;
	chprintf(bss, " servo_number range:- 1 - 6\r\n\n") ;
	chprintf(bss, " pulse_width  range:- 1000 - 2000\r\n\n") ;
  }

}
#endif	/*HAL_USE_PWM */

#if HAL_USE_ICU
void cmd_icu(BaseSequentialStream *bss, int argc, char *argv[]) {
  (void)argc ; (void)argv ;
  chprintf(bss, "Last width:- %d, Last period:- %d\r\n", last_width[0], last_period[0]) ;
}
#endif	/*HAL_USE_ICU */

#if PLUTO_USE_DCM
/* print current attitude roll, pitch, yaw number of times */
void cmd_attitude(BaseSequentialStream *bss, int argc, char *argv[]) {
  if(argc < 1) {
    chprintf(bss, "Usage: number of times attitude needs to be printed\r\n If a second argument is given it changes the default sleep between 2 prints from 100 ms\r\n") ;
  }
  int sleep_time = 100; /* default millisecond value */
  if(argc == 2) {
    sleep_time = atoi(argv[1]);
  }
  int i;
  for(i = 0; i < atoi(argv[0]); i++){
	chprintf(bss, "Timestamp  : %d   ", current_attitude.timestamp);
    chprintf(bss, "Roll  : %f   ", current_attitude.attitude[0]);
    chprintf(bss, "Pitch : %f   ", current_attitude.attitude[1]);
    chprintf(bss, "Yaw   : %f\r\n", current_attitude.attitude[2]);

    chThdSleepMilliseconds(sleep_time); /* TODO take this sleep time as the second argument */
  }
}

/* proportional gain inner loop roll, pitch, yaw */
void cmd_set_gain_kpi(BaseSequentialStream *bss, int argc, char *argv[]) {
  if(argc < 3) {
	chprintf(bss, "Usage: kpi roll-gain pitch-gain yaw-gain in float\r\n") ;
  }
  kpi[0] = atof(argv[0]);
  kpi[1] = atof(argv[1]);
  kpi[2] = atof(argv[2]);
}

/* integral gain inner loop roll, pitch, yaw */
void cmd_set_gain_kii(BaseSequentialStream *bss, int argc, char *argv[]) {
  if(argc < 3) {
    chprintf(bss, "Usage: kii roll-gain pitch-gain yaw-gain in float\r\n") ;
  }
  kii[0] = atof(argv[0]);
  kii[1] = atof(argv[1]);
  kii[2] = atof(argv[2]);
}

/* proportional gain outer loop roll, pitch, yaw */
void cmd_set_gain_kpo(BaseSequentialStream *bss, int argc, char *argv[]) {
  if(argc < 3) {
    chprintf(bss, "Usage: kpo roll-gain pitch-gain yaw-gain in float\r\n") ;
  }
  kpo[0] = atof(argv[0]);
  kpo[1] = atof(argv[1]);
  kpo[2] = atof(argv[2]);
}

/* integral gain outer loop roll, pitch, yaw */
void cmd_set_gain_kio(BaseSequentialStream *bss, int argc, char *argv[]) {
  if(argc < 3) {
    chprintf(bss, "Usage: kio roll-gain pitch-gain yaw-gain in float\r\n") ;
  }
  kio[0] = atof(argv[0]);
  kio[1] = atof(argv[1]);
  kio[2] = atof(argv[2]);
}
#endif /* PLUTO_USE_DCM */

#endif  /*PLUTO_USE_SHELL */
