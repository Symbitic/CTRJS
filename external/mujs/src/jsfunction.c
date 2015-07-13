#include "jsi.h"
#include "jsparse.h"
#include "jscompile.h"
#include "jsvalue.h"
#include "jsbuiltin.h"

#include <ctr/sys.h>

static void jsB_Function(js_State *J)
{
	unsigned int i, top = js_gettop(J);
	js_Buffer *sb = NULL;
	const char *body;
	js_Ast *parse;
	js_Function *fun;

	/* p1, p2, ..., pn */
	if (top > 2) {
		for (i = 1; i < top - 1; ++i) {
			if (i > 1)
				js_putc(J, &sb, ',');
			js_puts(J, &sb, js_tostring(J, i));
		}
		js_putc(J, &sb, ')');
	}

	/* body */
	body = js_isdefined(J, top - 1) ? js_tostring(J, top - 1) : "";

	if (js_try(J)) {
		js_free(J, sb);
		jsP_freeparse(J);
		js_throw(J);
	}

	parse = jsP_parsefunction(J, "[string]", sb ? sb->s : NULL, body);
	fun = jsC_compilefunction(J, parse);

	js_endtry(J);
	js_free(J, sb);
	jsP_freeparse(J);

	js_newfunction(J, fun, J->GE);
}

static void jsB_Function_prototype(js_State *J)
{
	js_pushundefined(J);
}

static void Fp_toString(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	char *s;
	unsigned int i, n;

	if (!js_iscallable(J, 0))
		js_typeerror(J, "not a function");

	if (self->type == JS_CFUNCTION || self->type == JS_CSCRIPT) {
		js_Function *F = self->u.f.function;
		n = strlen("function () { ... }");
		n += strlen(F->name);
		for (i = 0; i < F->numparams; ++i)
			n += strlen(F->vartab[i]) + 1;
		s = js_malloc(J, n);
		strcpy(s, "function ");
		strcat(s, F->name);
		strcat(s, "(");
		for (i = 0; i < F->numparams; ++i) {
			if (i > 0) strcat(s, ",");
			strcat(s, F->vartab[i]);
		}
		strcat(s, ") { ... }");
		if (js_try(J)) {
			js_free(J, s);
			js_throw(J);
		}
		js_pushstring(J, s);
		js_free(J, s);
		js_endtry(J);
	} else {
		js_pushliteral(J, "function () { ... }");
	}
}

static void Fp_apply(js_State *J)
{
	int i, n;

	if (!js_iscallable(J, 0))
		js_typeerror(J, "not a function");

	js_copy(J, 0);
	js_copy(J, 1);

	n = js_getlength(J, 2);
	for (i = 0; i < n; ++i)
		js_getindex(J, 2, i);

	js_call(J, n);
}

static void Fp_call(js_State *J)
{
	unsigned int i, top = js_gettop(J);

	if (!js_iscallable(J, 0))
		js_typeerror(J, "not a function");

	for (i = 0; i < top; ++i)
		js_copy(J, i);

	js_call(J, top - 2);
}

static void callbound(js_State *J)
{
	unsigned int top = js_gettop(J);
	unsigned int i, fun, args, n;

	fun = js_gettop(J);
	js_currentfunction(J);
	js_getproperty(J, fun, "__TargetFunction__");
	js_getproperty(J, fun, "__BoundThis__");

	args = js_gettop(J);
	js_getproperty(J, fun, "__BoundArguments__");
	n = js_getlength(J, args);
	for (i = 0; i < n; ++i)
		js_getindex(J, args, i);
	js_remove(J, args);

	for (i = 1; i < top; ++i)
		js_copy(J, i);

	js_call(J, n + top - 1);
}

static void constructbound(js_State *J)
{
	unsigned int top = js_gettop(J);
	unsigned int i, fun, args, n;

	fun = js_gettop(J);
	js_currentfunction(J);
	js_getproperty(J, fun, "__TargetFunction__");

	args = js_gettop(J);
	js_getproperty(J, fun, "__BoundArguments__");
	n = js_getlength(J, args);
	for (i = 0; i < n; ++i)
		js_getindex(J, args, i);
	js_remove(J, args);

	for (i = 1; i < top; ++i)
		js_copy(J, i);

	js_construct(J, n + top - 1);
}

static void Fp_bind(js_State *J)
{
	unsigned int i, top = js_gettop(J);
	unsigned int n;

	if (!js_iscallable(J, 0))
		js_typeerror(J, "not a function");

	n = js_getlength(J, 0);
	if (n > top - 2)
		n -= top - 2;
	else
		n = 0;

	js_newcconstructor(J, callbound, constructbound, "[bind]", n);

	/* Reuse target function's prototype for HasInstance check. */
	js_getproperty(J, 0, "prototype");
	js_defproperty(J, -2, "prototype", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	/* target function */
	js_copy(J, 0);
	js_defproperty(J, -2, "__TargetFunction__", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	/* bound this */
	js_copy(J, 1);
	js_defproperty(J, -2, "__BoundThis__", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	/* bound arguments */
	js_newarray(J);
	for (i = 2; i < top; ++i) {
		js_copy(J, i);
		js_setindex(J, -2, i - 2);
	}
	js_defproperty(J, -2, "__BoundArguments__", JS_READONLY | JS_DONTENUM | JS_DONTCONF);
}

void jsB_initfunction(js_State *J)
{
	sys_debug_printf("jsB_initfunction: 1\n");
	J->Function_prototype->u.c.function = jsB_Function_prototype;
	sys_debug_printf("jsB_initfunction: 2\n");
	J->Function_prototype->u.c.constructor = NULL;

	sys_debug_printf("jsB_initfunction: 3\n");
	js_pushobject(J, J->Function_prototype);
	{
		sys_debug_printf("jsB_initfunction: 4\n");
		jsB_propf(J, "toString", Fp_toString, 2);
		sys_debug_printf("jsB_initfunction: 5\n");
		//jsB_propf(J, "apply", Fp_apply, 2);
		sys_debug_printf("jsB_initfunction: 6\n");
		//jsB_propf(J, "call", Fp_call, 1);
		sys_debug_printf("jsB_initfunction: 7\n");
		//jsB_propf(J, "bind", Fp_bind, 1);
	}
	sys_debug_printf("jsB_initfunction: 8\n");
	js_newcconstructor(J, jsB_Function, jsB_Function, "Function", 1);
	sys_debug_printf("jsB_initfunction: 9\n");
	js_defglobal(J, "Function", JS_DONTENUM);
	sys_debug_printf("jsB_initfunction: 10\n");
}