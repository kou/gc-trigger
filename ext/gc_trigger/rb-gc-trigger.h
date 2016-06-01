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

#pragma once

#include <stdint.h>

#if defined(__cplusplus)
#  define RB_GC_TRIGGER_BEGIN_DECLS extern "C" {
#  define RB_GC_TRIGGER_END_DECLS }
#else
#  define RB_GC_TRIGGER_BEGIN_DECLS
#  define RB_GC_TRIGGER_END_DECLS
#endif

#ifdef _WIN32
#  define RB_GC_TRIGGER_API __declspec(dllexport)
#else
#  define RB_GC_TRIGGER_API
#endif

RB_GC_TRIGGER_BEGIN_DECLS

RB_GC_TRIGGER_API void rb_gc_trigger_update_memory_usage(int64_t diff);
RB_GC_TRIGGER_API void Init_gc_trigger(void);

RB_GC_TRIGGER_END_DECLS
