#include "sapphire.h"

typedef enum call_type {
  CALL_PUBLIC,
  CALL_FCALL,
  CALL_VCALL,
  CALL_TYPE_MAX
} call_type;

// TODO change this
inline function_ptr rb_search_method_entry(VALUE recv, ID mid, VALUE *defined_class_ptr);

VALUE
vm_call0(VALUE recv, ID id, int argc, const VALUE *argv, function_ptr me, VALUE defined_class)
{
  // just call the function with give args from here
  return me(recv, (VALUE *)argv);
}

inline VALUE
rb_call0(VALUE recv, ID mid, int argc, const VALUE *argv, call_type scope)
{
  VALUE defined_class;
  function_ptr me = rb_search_method_entry(recv, mid, &defined_class);
  // TODO do some stuff
  //
  //
  return vm_call0(recv, mid, argc, argv, me, defined_class);
}

inline VALUE
rb_call(VALUE recv, ID mid, int argc, const VALUE *argv, call_type scope)
{
  // rb_thread_t *th = GET_THREAD();
  return rb_call0(recv, mid, argc, argv, scope);
}

inline function_ptr
rb_search_method_entry(VALUE recv, ID mid, VALUE *defined_class_ptr)
{
  VALUE klass = CLASS_OF(recv);

  if (!klass) {
    // TODO replace this
    printf("\nno class found!!\n");
    throw std::string("error rb_search_method_entry - recv has no class");
  }
  return rb_method_entry(klass, mid, defined_class_ptr);
}

VALUE
rb_funcall(VALUE recv, ID mid, int n, ...)
{
  VALUE *argv;
  va_list ar;

  if (n > 0) {
    long i;

    va_start(ar, n);

    argv = new VALUE[n];

    for (i = 0; i < n; i++) {
      argv[i] = va_arg(ar, VALUE);
    }
    va_end(ar);
  } else {
    argv = 0;
  }
  return rb_call(recv, mid, n, argv, CALL_FCALL);
}
