/*
 * Copyright (C) 2005  Pekka Enberg
 */

#include <operand-stack.h>

#include <CuTest.h>
#include <stdlib.h>

void test_stack_retains_elements(CuTest *ct)
{
	struct operand_stack stack = OPERAND_STACK_INIT;
	stack_push(&stack, 0);
	stack_push(&stack, 1);
	CuAssertIntEquals(ct, 1, stack_pop(&stack));
	CuAssertIntEquals(ct, 0, stack_pop(&stack));
}
