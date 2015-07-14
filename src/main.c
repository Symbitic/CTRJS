/*
 *******************************************************************************
 * This file is part of CTRJS.
 * 
 * Copyright © 2015 Symbitic.
 * 
 * CTRJS is free software: you can redistribute it and/or modify it 
 * under the terms of version 3 of the GNU General Public License, as published
 * by the Free Software Foundation.
 * 
 * CTRJS is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * CTRJS. If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ctr/sys.h"
#include "ctr/fs.h"
#include "ctr/gsp.h"

#include <sys/iosupport.h>

#include "v7.h"

ssize_t js_write(struct _reent* r, int fd, const char* ptr, size_t len) {
	sys_debug_printf("JS> %s", ptr);
	return len;
}

int main(int argc, char* argv[]) {
	static const devoptab_t dotab_js = { "js", 0, NULL, NULL, js_write, NULL, NULL, NULL };

	devoptab_list[STD_OUT] = &dotab_js;
	devoptab_list[STD_ERR] = &dotab_js;

	setvbuf(stdout, NULL , _IOLBF, 0);
	setvbuf(stderr, NULL , _IOLBF, 0);

	printf("Testing\n");


	struct v7* v7 = NULL;
	v7_val_t exec_result;
	int ret;

	v7 = v7_create();
	if(v7 == NULL) {
		sys_debug_printf("v7_create failed: %s\n", v7_get_parser_error(v7));
		while(1) sys_thread_sleep(UINT64_MAX);
	}

	ret = v7_exec(v7, &exec_result, "print(5)");
	if(ret != 0) {
		sys_debug_printf("v7_exec error: %d (%s)\n", ret, v7_get_parser_error(v7));
		while(1) sys_thread_sleep(UINT64_MAX);
	}

	sys_debug_printf("v7_exec returned %d", exec_result);

	while(1) sys_thread_sleep(UINT64_MAX);

	v7_destroy(v7);

	return 0;
}
#endif
