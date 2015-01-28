#include "sapphire.h"

VALUE rb_cFloat;
VALUE rb_cFixnum;

VALUE
fix_plus(VALUE self, VALUE args [])
{
  VALUE num2 = args[0];

  int result = value_to_int(self) + value_to_int(num2); // convert to int and add
  return int_to_value(result);
}

VALUE
fix_minus(VALUE self, VALUE args [])
{
  VALUE num2 = args[0];

  int result = value_to_int(self) - value_to_int(num2);
  return int_to_value(result);
}

VALUE
fix_times(VALUE self, VALUE args [])
{
  VALUE num2 = args[0];

  int result = value_to_int(self) * value_to_int(num2);
  return int_to_value(result);
}

VALUE
fix_divide(VALUE self, VALUE args [])
{
  VALUE num2 = args[0];

  int result = value_to_int(self) / value_to_int(num2);
  return int_to_value(result);
}

void
Init_Numeric()
{
  rb_cFloat = rb_define_class("Float", rb_cObject);

  rb_cFixnum = rb_define_class("Fixnum", rb_cObject);
  rb_define_method(rb_cFixnum, "+", fix_plus, 1);
  rb_define_method(rb_cFixnum, "-", fix_minus, 1);
  rb_define_method(rb_cFixnum, "*", fix_times, 1);
  rb_define_method(rb_cFixnum, "/", fix_divide, 1);
}

VALUE
rb_float_new(double d)
{
  // do some range checking here like is done in ruby's rb_float_new_inline
  // here we are replacing ruby's #define NEWOBJ_OF(obj,type,klass,flags) type *(obj) = (type*)rb_newobj_of(klass, flags)
  RFloat *flt = new RFloat ();
  RBASIC_SET_CLASS(flt, rb_cFloat);
  RBASIC(flt)->flags |= (T_FLOAT & T_MASK);
  flt->float_value = d;
  return (VALUE)flt;
}
