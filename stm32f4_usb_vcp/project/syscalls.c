/*
 * syscalls.c
 *
 *  Created on: 03.12.2009, modified 2.6.2012
 *      Author: Martin Thomas, 3BSD license
 */

#define SBRK_VERBOSE 1
#define EXIT_VERBOSE 1

/* Set the following non-null to enable a workaround for GNU tools for ARM
 * (launchpad) where libgcc is configured to throw exceptions in ldiv on
 * division by 0. This disables the exception and saves around 4kBytes of
 * flash-memory. Option may be removed/disabled for future releases
 * of the tool-chain and is also not needed for CodeBench lite where the
 * compiler is configured differently. */
#define FORCE_DIV_EXCEPTION_OFF 1

#include <reent.h>
#include <errno.h>
#include <stdlib.h> /* abort */
#include <sys/types.h>
#include <sys/stat.h>

#include "stm32f4xx.h" /* for __get_PSP() and __get_MSP() */

#if SBRK_VERBOSE || EXIT_VERBOSE
#include "xprintf.h"
#endif

extern int _io_putchar(int); /* see main.c */

#undef errno
extern int errno;

#if FORCE_DIV_EXCEPTION_OFF
/* idea from http://devkitpro.org/viewtopic.php?f=2&t=2944 & Linux arm/unwind.c */
void __aeabi_unwind_cpp_pr0() { ; }
void __aeabi_unwind_cpp_pr1() { ; }
void __aeabi_unwind_cpp_pr2() { ; }
#endif

/**
 * @brief  User abort
 *   overwrites libc's abort() and saves some program-space
 * @param  None
 * @retval None
 */
void abort(void)
{
	while (1) { ; }
}

int _kill(int pid, int sig)
{
	(void)pid;
	(void)sig; /* avoid warnings */
	errno = EINVAL;
	return -1;
}

void _exit(int status)
{
#if EXIT_VERBOSE
	xprintf("_exit called with parameter %d\n", status);
#endif
	while(1) {;}
}

int _getpid(void)
{
	return 1;
}


extern char _end; /* Defined by the linker. */
static char *heap_end;

char* get_heap_end(void)
{
	return (char*) heap_end;
}

char* get_stack_top(void)
{
	return (char*) __get_MSP();
	//return (char*) __get_PSP();
}

caddr_t _sbrk(int incr)
{
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &_end;
	}
	prev_heap_end = heap_end;

#if SBRK_VERBOSE
	xprintf("_sbrk called with incr %d heap-end before at 0x%08lX, stack_top at 0x%08lX",
			incr, (uint32_t)heap_end, (uint32_t)get_stack_top());
#endif

	if (heap_end + incr > get_stack_top()) {
#if SBRK_VERBOSE
		xprintf(" *** Heap and stack collision ***\n");
#endif
		abort();
	}

	heap_end += incr;

#if SBRK_VERBOSE
	xprintf(", heap-end after at 0x%08lX\n", (uint32_t)heap_end);
#endif

	return (caddr_t) prev_heap_end;
}

int _close(int file)
{
	(void)file; /* avoid warning */
	return -1;
}

int _fstat(int file, struct stat *st)
{
	(void)file; /* avoid warning */
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	(void)file; /* avoid warning */
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	(void)file; /* avoid warning */
	(void)ptr; /* avoid warning */
	(void)dir; /* avoid warning */
	return 0;
}

int _read(int file, char *ptr, int len)
{
	(void)file; /* avoid warning */
	(void)ptr; /* avoid warning */
	(void)len; /* avoid warning */
	return 0;
}

int _write(int file, char *ptr, int len)
{
	int todo;

	for (todo = 0; todo < len; todo++) {
		if (_isatty(file) && *ptr == '\n') {
			_io_putchar('\r');
		}
		_io_putchar(*ptr++);
	}
	return len;
}
