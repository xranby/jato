/*
 * Copyright (c) 2009  Tomasz Grabiec
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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "arch/call.h"

#include "vm/call.h"
#include "vm/class.h"
#include "vm/jni.h"
#include "vm/method.h"
#include "vm/object.h"
#include "vm/stack-trace.h"

static unsigned long
vm_call_method_a(struct vm_method *method, unsigned long *args)
{
	unsigned long result;
	void *target;

	if (vm_method_is_vm_native(method)) {
		target = vm_method_native_ptr(method);
		vm_native_call(target, args, method->args_count, result);
		return result;
	}

	target = vm_method_call_ptr(method);

	if (vm_method_is_jni(method)) {
		if (vm_enter_jni(__builtin_frame_address(0), 0, method))
			return 0;

		native_call(target, args, method->args_count, result);
		vm_leave_jni();
		return result;
	}

	native_call(target, args, method->args_count, result);
	return result;
}


static unsigned long
vm_call_jni_method_v(struct vm_method *method, va_list args)
{
	unsigned long args_array[method->args_count];
	int i;

	i = 0;
	args_array[i++] = (unsigned long)vm_jni_get_jni_env();

	if (vm_method_is_static(method))
		args_array[i++] = (unsigned long)method->class->object;

	while (i < method->args_count)
		args_array[i++] = va_arg(args, long);

	return vm_call_method_a(method, args_array);
}

unsigned long vm_call_method_v(struct vm_method *method, va_list args)
{
	unsigned long args_array[method->args_count];

	if (vm_method_is_jni(method))
		return vm_call_jni_method_v(method, args);

	for (int i = 0; i < method->args_count; i++)
		args_array[i] = va_arg(args, unsigned long);

	return vm_call_method_a(method, args_array);
}

unsigned long vm_call_method(struct vm_method *method, ...)
{
	unsigned long result;
	va_list args;

	va_start(args, method);
	result = vm_call_method_v(method, args);
	va_end(args);

	return result;
}