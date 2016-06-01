/*
 * Copyright (C) 2016  Kouhei Sutou <kou@clear-code.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ruby.h>
#include <ruby/thread_native.h>

#define RB_GC_TRIGGER_DLL
#include "rb-gc-trigger.h"

static rb_nativethread_lock_t memory_usage_lock;
static int64_t memory_usage_current = 0;
static int64_t memory_usage_threshold = 4096;

void
rb_gc_trigger_update_memory_usage(int64_t diff)
{
  int need_gc;

  memory_usage_current += diff;

  if (diff <= 0) {
    return;
  }

  if (rb_during_gc()) {
    return;
  }

  rb_nativethread_lock_lock(&memory_usage_lock);
  need_gc = (memory_usage_current > memory_usage_threshold);
  rb_nativethread_lock_unlock(&memory_usage_lock);
  if (!need_gc) {
    return;
  }

  rb_gc();
  rb_nativethread_lock_lock(&memory_usage_lock);
  if (memory_usage_current > memory_usage_threshold) {
    memory_usage_threshold *= 2;
  }
  rb_nativethread_lock_unlock(&memory_usage_lock);
}

static VALUE
update_memory_usage(VALUE module, VALUE rb_diff)
{
  int64_t diff;

  diff = NUM2LL(rb_diff);
  rb_gc_trigger_update_memory_usage(diff);

  return Qnil;
}

void
Init_gc_trigger(void)
{
  VALUE gc_trigger;

  rb_nativethread_lock_initialize(&memory_usage_lock);

  gc_trigger = rb_define_module("GCTrigger");

  rb_define_singleton_method(gc_trigger,
                             "update_memory_usage",
                             update_memory_usage,
                             1);
}
