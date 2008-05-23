/*
 * Linear scan register allocation
 * Copyright © 2007-2008  Pekka Enberg
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
 *
 * For more details on the linear scan register allocation algorithm used here,
 * please refer to the following paper:
 *
 *   Wimmer, C. and Mössenböck, H. 2005. Optimized interval splitting
 *   in a linear scan register allocator. In Proceedings of the 1st
 *   ACM/USENIX international Conference on Virtual Execution
 *   Environments (Chicago, IL, USA, June 11 - 12, 2005). VEE '05. ACM
 *   Press, New York, NY, 132-141.
 */

#include <jit/compiler.h>
#include <jit/vars.h>
#include <vm/bitset.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static void allocate_blocked_reg(void)
{
	assert(!"oops: blocked register");
}

/* Find the register that is used the latest.  */
static enum machine_reg pick_register(unsigned long *free_until_pos)
{
	unsigned long max_pos = 0;
	int i, ret = 0;

	for (i = 0; i < NR_REGISTERS; i++) {
		unsigned long pos = free_until_pos[i];

		if (pos > max_pos) {
			max_pos = pos;
			ret = i;
		}
	}
	return ret;
}

static void try_to_allocate_free_reg(struct live_interval *current,
				     struct list_head *active,
				     struct list_head *inactive)
{
	unsigned long free_until_pos[NR_REGISTERS];
	struct live_interval *it;
	enum machine_reg reg;
	int i;

	for (i = 0; i < NR_REGISTERS; i++)
		free_until_pos[i] = LONG_MAX;

	list_for_each_entry(it, active, interval_node) {
		free_until_pos[it->reg] = 0;
	}

	list_for_each_entry(it, inactive, interval_node) {
		if (ranges_intersect(&it->range, &current->range)) {
			assert("!oops: intersects with current");
		}
	}

	reg = pick_register(free_until_pos);
	if (free_until_pos[reg] == 0) {
		/*
		 * No register available without spilling.
		 */
		return;
	}

	if (current->range.end < free_until_pos[reg]) {
		/*
		 * Register available for the full interval.
		 */
		current->reg = reg;
	} else {
		/*
		 * Register available for the first part of the interval.
		 */
		assert(!"need to split");
	}
}

/* Inserts to a list of intervals sorted by increasing start position.  */
static void
insert_to_list(struct live_interval *interval, struct list_head *interval_list)
{
	struct live_interval *this;

	/*
	 * If we find an existing interval, that starts _after_ the
	 * new interval, add ours before that.
	 */
	list_for_each_entry(this, interval_list, interval_node) {
		if (interval->range.start < this->range.start) {
			list_add_tail(&interval->interval_node, &this->interval_node);
			return;
		}
	}

	/*
	 * Otherwise the new interval goes to the end of the list.
	 */
	list_add_tail(&interval->interval_node, interval_list);
}

int allocate_registers(struct compilation_unit *cu)
{
	struct list_head unhandled = LIST_HEAD_INIT(unhandled);
	struct list_head inactive = LIST_HEAD_INIT(inactive);
	struct list_head active = LIST_HEAD_INIT(active);
	struct live_interval *tmp, *current;
	struct bitset *registers;
	struct var_info *var;

	registers = alloc_bitset(NR_REGISTERS);
	if (!registers)
		return -ENOMEM;

	bitset_set_all(registers);

	/*
	 * Fixed intervals need to be inserted before non-fixed intervals to
	 * make sure the former always block a fixed register from being
	 * allocated.
	 */
	for_each_variable(var, cu->var_infos) {
		if (var->interval->reg != REG_UNASSIGNED)
			insert_to_list(var->interval, &unhandled);
	}

	for_each_variable(var, cu->var_infos) {
		if (var->interval->reg == REG_UNASSIGNED)
			insert_to_list(var->interval, &unhandled);
	}

	list_for_each_entry_safe(current, tmp, &unhandled, interval_node) {
		struct live_interval *it, *prev;
		unsigned long position;

		list_del(&current->interval_node);
		position = current->range.start;

		list_for_each_entry_safe(it, prev, &active, interval_node) {
			if (it->range.end <= position) {
				/*
				 * Remove handled interval from active list.
				 */
				list_del(&it->interval_node);
				continue;
			}
			if (!in_range(&it->range, position))
				list_move(&it->interval_node, &inactive);
		}

		list_for_each_entry_safe(it, prev, &inactive, interval_node) {
			if (it->range.end <= position) {
				/*
				 * Remove handled interval from inactive list.
				 */
				list_del(&it->interval_node);
				continue;
			}
			if (in_range(&it->range, position))
				list_move(&it->interval_node, &active);
		}

		/*
		 * Don't allocate registers for fixed intervals.
		 */
		if (current->reg == REG_UNASSIGNED) {
			try_to_allocate_free_reg(current, &active, &inactive);

			if (current->reg == REG_UNASSIGNED)
				allocate_blocked_reg();
		}
		if (current->reg != REG_UNASSIGNED)
			list_add(&current->interval_node, &active);
	}
	free(registers);

	return 0;
}
