# README

## Name

gc-trigger

## Description

gc-trigger is a library for extension libraries that use many memories in external libraries. For example, an extension library that manipulates images is a target library.

Ruby's GC doesn't care about memory usage of external libraries. It means that Ruby's GC isn't run when extension library uses huge memories in external libraries. For example, Ruby's GC isn't run when you open many images by the extension library.

## Install

```
% gem install gc-trigger
```

## Usage

You can use C API and Ruby API. Normally, C API is suitable.

### C API usage

Add the following code to your `mkmf.rb`:

```ruby
gc_trigger_spec = Gem::Specification.find_by_name("gc-trigger")
include_dir = File.join(gc_trigger_spec.gem_dir, "ext", "gc_trigger")
$CFLAGS << " -I#{include_dir}"
```

Include `rb-gc-trigger.h` in your C code:

```c
#include <rb-gc-trigger.h>
```

Call `rb_gc_trigger_update_memory_usage()` with memory usage difference when memory usage is changed:

```c
static VALUE
xxx_initialize(VALUE self)
{
  /* Allocate memory for external library. */
  DATA_PTR(self) = xxx_new();
  /* Notify added memory usage in byte. */
  rb_gc_trigger_update_memory_usage(1024);
  return Qnil;
}

static void
free_func(void *xxx)
{
  xxx_free(xxx);
  /* Notify freed memosy usage in byte. */
  rb_gc_trigger_update_memory_usage(-1024);
}
```

### Ruby API usage

Require `gc-trigger`:

```ruby
require "gc-trigger"
```

Call `GCTrigger.update_memory_usage` with memory usage difference when memory usage is changed:

```ruby
def xxx_finalizer(size)
  lambda do |id|
    # Notify freed memory usage.
    GCTrigger.update_memory_usage(-size)
  end
end

def create_xxx
  xxx = XXX.new
  size = 1024
  # Register finalizer that notify freed memory usage.
  ObjectSpace.define_finalizer(xxx, xxx_finalizer(size))
  # Notify added memory usage.
  GCTrigger.update_memory_usage(size)
  xxx
end
```

## Author

Kouhei Sutou \<kou@clear-code.com\>

## License

LGPLv3 or later. See doc/text/lgpl-3.0.txt for details.

(Kouhei Sutou has a right to change the license including contributed
patches.)
