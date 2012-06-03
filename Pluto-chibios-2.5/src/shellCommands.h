/*Created By - Rohit Rangan
 *Created On - 02-06-2012
 *
 *This file contains all the shell commands and shell
 *structures required to start the PLUTO shell.
 */

#include "plutoconf.h"

#ifndef SHELLCOMMANDS_H_
#define SHELLCOMMANDS_H_

#if PLUTO_USE_SHELL
#define SHELL_WA_SIZE   THD_WA_SIZE(2048)

extern const ShellCommand commands[] ;

extern const ShellConfig shell_cfg1 ;

void cmd_mem(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_threads(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_clear(BaseSequentialStream *bss, int argc, char *argv[]);

#if PLUTO_USE_FATFS
void cmd_mount(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_unmount(BaseSequentialStream *bss, int argc, char *argv[]);

void cmd_tree(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*PLUTO_USE_FATFS */

#if PLUTO_USE_ACCELEROMETER
void cmd_accelerometer(BaseSequentialStream *bss, int argc, char *argv[]);
#endif	/*PLUTO_USE_ACCELEROMETER */

#endif	/*PLUTO_USE_SHELL */

#endif	/*SHELLCOMMANDS_H_ */
