/*
 * Copyright (C) 2005-2006  Pekka Enberg
 */

#include <bc-test-utils.h>
#include <jit/compilation-unit.h>
#include <jit/expression.h>
#include <jit/jit-compiler.h>
#include <libharness.h>
#include <vm/stack.h>
#include <vm/system.h>
#include <vm/types.h>
#include <vm/vm.h>

#include "vm-utils.h"

static void convert_ir_const_single(struct compilation_unit *cu, void *value)
{
	u8 cp_infos[] = { (unsigned long) value };
	u1 cp_types[] = { CONSTANT_Resolved };

	convert_ir_const(cu, (void *)cp_infos, 8, cp_types);
}

static void assert_convert_getstatic(enum vm_type expected_vm_type,
				     char *field_type)
{
	struct fieldblock fb;
	struct expression *expr;
	unsigned char code[] = { OPC_GETSTATIC, 0x00, 0x00 };
	struct methodblock method = {
		.jit_code = code,
		.code_size = ARRAY_SIZE(code),
	};
	struct compilation_unit *cu;

	fb.type = field_type;

	cu = alloc_simple_compilation_unit(&method);

	convert_ir_const_single(cu, &fb);
	expr = stack_pop(cu->expr_stack);
	assert_field_expr(expected_vm_type, &fb, &expr->node);
	assert_true(stack_is_empty(cu->expr_stack));

	expr_put(expr);
	free_compilation_unit(cu);
}

void test_convert_getstatic(void)
{
	assert_convert_getstatic(J_BYTE, "B");
	assert_convert_getstatic(J_CHAR, "C");
	assert_convert_getstatic(J_DOUBLE, "D");
	assert_convert_getstatic(J_FLOAT, "F");
	assert_convert_getstatic(J_INT, "I");
	assert_convert_getstatic(J_LONG, "J");
	assert_convert_getstatic(J_REFERENCE, "Ljava/lang/Object;");
	assert_convert_getstatic(J_SHORT, "S");
	assert_convert_getstatic(J_BOOLEAN, "Z");
}

static void assert_convert_putstatic(enum vm_type expected_vm_type,
				     char *field_type)
{
	struct fieldblock fb;
	struct statement *stmt;
	unsigned char code[] = { OPC_PUTSTATIC, 0x00, 0x00 };
	struct methodblock method = {
		.jit_code = code,
		.code_size = ARRAY_SIZE(code),
	};
	struct compilation_unit *cu;
	struct expression *value;

	fb.type = field_type;
	value = value_expr(expected_vm_type, 0xdeadbeef);
	cu = alloc_simple_compilation_unit(&method);
	stack_push(cu->expr_stack, value);
	convert_ir_const_single(cu, &fb);
	stmt = stmt_entry(bb_entry(cu->bb_list.next)->stmt_list.next);

	assert_store_stmt(stmt);
	assert_field_expr(expected_vm_type, &fb, stmt->store_dest);
	assert_ptr_equals(value, to_expr(stmt->store_src));
	assert_true(stack_is_empty(cu->expr_stack));

	free_compilation_unit(cu);
}

void test_convert_putstatic(void)
{
	assert_convert_putstatic(J_BYTE, "B");
	assert_convert_putstatic(J_CHAR, "C");
	assert_convert_putstatic(J_DOUBLE, "D");
	assert_convert_putstatic(J_FLOAT, "F");
	assert_convert_putstatic(J_INT, "I");
	assert_convert_putstatic(J_LONG, "J");
	assert_convert_putstatic(J_REFERENCE, "Ljava/lang/Object;");
	assert_convert_putstatic(J_SHORT, "S");
	assert_convert_putstatic(J_BOOLEAN, "Z");
}

static void assert_convert_array_load(enum vm_type expected_type,
				      unsigned char opc,
				      unsigned long arrayref,
				      unsigned long index)
{
	unsigned char code[] = { opc };
	struct expression *arrayref_expr, *index_expr, *temporary_expr;
	struct statement *stmt;
	struct methodblock method = {
		.jit_code = code,
		.code_size = ARRAY_SIZE(code),
	};
	struct compilation_unit *cu;
	unsigned long expected_temporary;

	cu = alloc_simple_compilation_unit(&method);

	arrayref_expr = value_expr(J_REFERENCE, arrayref);
	index_expr = value_expr(J_INT, index);

	stack_push(cu->expr_stack, arrayref_expr);
	stack_push(cu->expr_stack, index_expr);

	convert_to_ir(cu);
	stmt = stmt_entry(bb_entry(cu->bb_list.next)->stmt_list.next);

	struct statement *nullcheck = stmt;
	struct statement *arraycheck = stmt_entry(nullcheck->stmt_list_node.next);
	struct statement *store_stmt = stmt_entry(arraycheck->stmt_list_node.next);

	assert_null_check_stmt(arrayref_expr, nullcheck);
	assert_arraycheck_stmt(expected_type, arrayref_expr, index_expr, arraycheck);

	assert_store_stmt(store_stmt);
	assert_array_deref_expr(expected_type, arrayref_expr, index_expr,
				store_stmt->store_src);

	temporary_expr = stack_pop(cu->expr_stack);

	expected_temporary = to_expr(store_stmt->store_dest)->temporary;
	assert_temporary_expr(expected_temporary, &temporary_expr->node);
	expr_put(temporary_expr);
	assert_true(stack_is_empty(cu->expr_stack));

	free_compilation_unit(cu);
}

void test_convert_iaload(void)
{
	assert_convert_array_load(J_INT, OPC_IALOAD, 0, 1);
	assert_convert_array_load(J_INT, OPC_IALOAD, 1, 2);
}

void test_convert_laload(void)
{
	assert_convert_array_load(J_LONG, OPC_LALOAD, 0, 1);
	assert_convert_array_load(J_LONG, OPC_LALOAD, 1, 2);
}

void test_convert_faload(void)
{
	assert_convert_array_load(J_FLOAT, OPC_FALOAD, 0, 1);
	assert_convert_array_load(J_FLOAT, OPC_FALOAD, 1, 2);
}

void test_convert_daload(void)
{
	assert_convert_array_load(J_DOUBLE, OPC_DALOAD, 0, 1);
	assert_convert_array_load(J_DOUBLE, OPC_DALOAD, 1, 2);
}

void test_convert_aaload(void)
{
	assert_convert_array_load(J_REFERENCE, OPC_AALOAD, 0, 1);
	assert_convert_array_load(J_REFERENCE, OPC_AALOAD, 1, 2);
}

void test_convert_baload(void)
{
	assert_convert_array_load(J_INT, OPC_BALOAD, 0, 1);
	assert_convert_array_load(J_INT, OPC_BALOAD, 1, 2);
}

void test_convert_caload(void)
{
	assert_convert_array_load(J_CHAR, OPC_CALOAD, 0, 1);
	assert_convert_array_load(J_CHAR, OPC_CALOAD, 1, 2);
}

void test_convert_saload(void)
{
	assert_convert_array_load(J_SHORT, OPC_SALOAD, 0, 1);
	assert_convert_array_load(J_SHORT, OPC_SALOAD, 1, 2);
}

static void assert_convert_array_store(enum vm_type expected_type,
				       unsigned char opc,
				       unsigned long arrayref,
				       unsigned long index, unsigned long value)
{
	unsigned char code[] = { opc };
	struct expression *arrayref_expr, *index_expr, *expr;
	struct statement *stmt;
	struct compilation_unit *cu;
	struct methodblock method = {
		.jit_code = code,
		.code_size = ARRAY_SIZE(code),
	};

	cu = alloc_simple_compilation_unit(&method);

	arrayref_expr = value_expr(J_REFERENCE, arrayref);
	index_expr = value_expr(J_INT, index);
	expr = temporary_expr(expected_type, value);

	stack_push(cu->expr_stack, arrayref_expr);
	stack_push(cu->expr_stack, index_expr);
	stack_push(cu->expr_stack, expr);

	convert_to_ir(cu);
	stmt = stmt_entry(bb_entry(cu->bb_list.next)->stmt_list.next);

	struct statement *nullcheck = stmt;
	struct statement *arraycheck = stmt_entry(nullcheck->stmt_list_node.next);
	struct statement *store_stmt = stmt_entry(arraycheck->stmt_list_node.next);

	assert_null_check_stmt(arrayref_expr, nullcheck);
	assert_arraycheck_stmt(expected_type, arrayref_expr, index_expr,
			       arraycheck);

	assert_store_stmt(store_stmt);
	assert_array_deref_expr(expected_type, arrayref_expr, index_expr,
				store_stmt->store_dest);
	assert_temporary_expr(value, store_stmt->store_src);

	assert_true(stack_is_empty(cu->expr_stack));

	free_compilation_unit(cu);
}

void test_convert_iastore(void)
{
	assert_convert_array_store(J_INT, OPC_IASTORE, 0, 1, 2);
	assert_convert_array_store(J_INT, OPC_IASTORE, 2, 3, 4);
}

void test_convert_lastore(void)
{
	assert_convert_array_store(J_LONG, OPC_LASTORE, 0, 1, 2);
	assert_convert_array_store(J_LONG, OPC_LASTORE, 2, 3, 4);
}

void test_convert_fastore(void)
{
	assert_convert_array_store(J_FLOAT, OPC_FASTORE, 0, 1, 2);
	assert_convert_array_store(J_FLOAT, OPC_FASTORE, 2, 3, 4);
}

void test_convert_dastore(void)
{
	assert_convert_array_store(J_DOUBLE, OPC_DASTORE, 0, 1, 2);
	assert_convert_array_store(J_DOUBLE, OPC_DASTORE, 2, 3, 4);
}

void test_convert_aastore(void)
{
	assert_convert_array_store(J_REFERENCE, OPC_AASTORE, 0, 1, 2);
	assert_convert_array_store(J_REFERENCE, OPC_AASTORE, 2, 3, 4);
}

void test_convert_bastore(void)
{
	assert_convert_array_store(J_INT, OPC_BASTORE, 0, 1, 2);
	assert_convert_array_store(J_INT, OPC_BASTORE, 2, 3, 4);
}

void test_convert_castore(void)
{
	assert_convert_array_store(J_CHAR, OPC_CASTORE, 0, 1, 2);
	assert_convert_array_store(J_CHAR, OPC_CASTORE, 2, 3, 4);
}

void test_convert_sastore(void)
{
	assert_convert_array_store(J_SHORT, OPC_SASTORE, 0, 1, 2);
	assert_convert_array_store(J_SHORT, OPC_SASTORE, 2, 3, 4);
}

static void assert_convert_new(unsigned long expected_type_idx,
			       unsigned char idx_1, unsigned char idx_2)
{
	struct object *instance_class = new_class();
	unsigned char code[] = { OPC_NEW, 0x0, 0x0 };
	struct compilation_unit *cu;
	struct expression *new_expr;
	struct methodblock method = {
		.jit_code = code,
		.code_size = ARRAY_SIZE(code),
	};

	cu = alloc_simple_compilation_unit(&method);
	convert_ir_const_single(cu, instance_class);

	new_expr = stack_pop(cu->expr_stack);
	assert_int_equals(EXPR_NEW, expr_type(new_expr));
	assert_int_equals(J_REFERENCE, new_expr->vm_type);
	assert_ptr_equals(instance_class, new_expr->class);

	free_expression(new_expr);
	free_compilation_unit(cu);

	free(instance_class);
}

void test_convert_new(void)
{
	assert_convert_new(0xcafe, 0xca, 0xfe);
}
