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

#include "ctr/sys.h"
#include "ctr/fs.h"
#include "ctr/gsp.h"

#include "mujs.h"

static void js_print(js_State* J) {
	char buffer[255];
	uint32_t i;
	uint32_t top = 0;
	uint32_t size = 0;

	top = js_gettop(J);

	for(i=1; i<top; i++) {
		const char* s = js_tostring(J, i);
		if(i > 1) {
			strncpy(buffer + size, " ", 1);
			size++;
		}
		const uint32_t len = strlen(s);
		strncpy(buffer + size, s, len);
		size += len;
	}

	sys_debug_printf("JS: %s\n", buffer);

	js_pushundefined(J);
}

int main(int argc, char *argv[]) {
	js_State* J = NULL;

	sys_debug_printf("Creating state");

	J = js_newstate(NULL, NULL, JS_STRICT);
	if(J == NULL) {
		sys_debug_printf("Error creating JavaScript state object.");
		exit(EXIT_FAILURE);
	}

	sys_debug_printf("Adding \'print\' function");

	js_newcfunction(J, js_print, "print", 1);
	js_setglobal(J, "print");

	sys_debug_printf("Executing \'print\' function");

	js_dostring(J, "print('hello world');", 0);

	sys_debug_printf("Completed -- entering sleep\n");

	while(1) {
		sys_thread_sleep(UINT64_MAX);
	}

	js_freestate(J);

	return 0;
}
