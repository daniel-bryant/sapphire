#include "sapphire.h"

VALUE rb_cArray;

#define ARY_DEFAULT_SIZE 16
#define ARY_MAX_SIZE (LONG_MAX / (int)sizeof(VALUE))

VALUE
ary_alloc(VALUE klass)
{
  RArray *ary = new RArray ();
  RBASIC_SET_CLASS(ary, klass);
  RBASIC(ary)->flags |= (T_ARRAY & T_MASK);
  return (VALUE)ary;
}

VALUE
ary_new(VALUE klass, long capa)
{
  VALUE ary;
  VALUE *ptr;

  if (capa < 0) {
    // rb_raise(rb_eArgError, "negative array size (or size too big)");
  }
  if (capa > ARY_MAX_SIZE) {
    // rb_raise(rb_eArgError, "array size too big");
  }
  //if (RUBY_DTRACE_ARRAY_CREATE_ENABLED()) {
  //  RUBY_DTRACE_ARRAY_CREATE(capa, rb_sourcefile(), rb_sourceline());
  //}
  ary = ary_alloc(klass);
  // TODO if (capa > RARRAY_EMBED_LEN_MAX)
  return ary;
}

VALUE
rb_ary_new_capa(long capa)
{
  return ary_new(rb_cArray, capa);
}

void
Init_Array()
{
  rb_cArray  = rb_define_class("Array", rb_cObject);
}
