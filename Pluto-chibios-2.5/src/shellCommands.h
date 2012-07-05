/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the shell commands and shell
 *structures required to start the PLUTO shell.
 */
/* 18-06-2012 - Riya Ray - The functions fpuTest and cmd_fputest have been added */

#include "plutoconf.h"

#ifndef SHELLCOMMANDS_H_
#define SHELLCOMMANDS_H_

#if PLUTO_USE_SHELL
#define SHELL_WA_SIZE   THD_WA_SIZE(2048)

extern const ShellCommand commands[] ;

extern const ShellConfig shell_cfg1 ;

#if CORTEX_USE_FPU && PLUTO_USE_FPUTEST
float fpuTest(BaseSequentialStream *bss, char *operand1, char oper, char *operand2);

void cmd_fputest(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*CORTEX_USE_FPU && PLUTO_USE_FPUTEST */

void cmd_mem(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_threads(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_clear(BaseSequentialStream *bss, int argc, char *argv[]);

#if PLUTO_USE_FATFS
void cmd_mount(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_unmount(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_tree(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*PLUTO_USE_FATFS */

#if PLUTO_USE_IMU
void cmd_imu(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*PLUTO_USE_ACCELEROMETER */

#if PLUTO_USE_MAGNETOMETER
void cmd_magnetometer(BaseSequentialStream *bss, int argc, char *argv[]);
#endif /*PLUTO_USE_MAGNETOMETER */

#if PLUTO_USE_BAROMETER
void cmd_barometer(BaseSequentialStream *bss, int argc, char *argv[]);
#endif /*PLUTO_USE_BAROMETER */

#if HAL_USE_PWM
void cmd_pwm(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*HAL_USE_PWM */

#endif	/*PLUTO_USE_SHELL */

#endif	/*SHELLCOMMANDS_H_ */
