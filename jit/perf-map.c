/*
 * Copyright (c) 2009  Pekka Enberg
 * 
 * This file is released under the GPL version 2 with the following
 * clarification and special exception:
 *
 *     Linking this library statically or dynamically with other modules is
 *     making a combined work based on this library. Thus, the terms and
 *     conditions of the GNU General Public License cover the whole
 *     combination.
 *
 *     As a special exception, the copyright holders of this library give you
 *     permission to link this library with independent modules to produce an
 *     executable, regardless of the license terms of these independent
 *     modules, and to copy and distribute the resulting executable under terms
 *     of your choice, provided that you also meet, for each linked independent
 *     module, the terms and conditions of the license of that module. An
 *     independent module is a module which is not derived from or based on
 *     this library. If you modify this library, you may extend this exception
 *     to your version of the library, but you are not obligated to do so. If
 *     you do not wish to do so, delete this exception statement from your
 *     version.
 *
 * Please refer to the file LICENSE for details.
 */

#include <jit/perf-map.h>
#include <vm/die.h>

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

static pthread_mutex_t perf_mutex = PTHREAD_MUTEX_INITIALIZER; 
static FILE *perf_file;

void perf_map_open(void)
{
	char filename[32];
	pid_t pid;

	pid = getpid();
	sprintf(filename, "/tmp/perf-%d.map", pid);

	perf_file = fopen(filename, "w");
	if (!perf_file)
		die("fopen");
}

void perf_map_append(const char *symbol, unsigned long addr, unsigned long size)
{
	if (perf_file == NULL)
		return;

	pthread_mutex_lock(&perf_mutex);
	fprintf(perf_file, "%lx %lx %s\n", addr, size, symbol);
	pthread_mutex_unlock(&perf_mutex);
}