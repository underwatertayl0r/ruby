#include "ruby.h"
#include "internal/string.h"

static VALUE
stack_alloca_overflow(VALUE self)
{
    size_t i = 0;

    while (1) {
        // Allocate and touch memory to force actual memory usage:
        char *buf = (char *)malloc(1024);
        if (!buf) {
            break;
        }
        volatile char *stack = buf;
        stack[0] = (char)i;
        stack[1023] = (char)i;
        i++;
        free(buf);
    }

    return Qnil;
}

static VALUE
asan_p(VALUE klass)
{
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_stack(VALUE klass)
{
    rb_define_singleton_method(rb_cThread, "alloca_overflow", stack_alloca_overflow, 0);
    rb_define_singleton_method(rb_cThread, "asan?", asan_p, 0);
}
