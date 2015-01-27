#include "sapphire.h"
#include <math.h>

VALUE rb_mMath;

VALUE
math_sin(VALUE obj, VALUE args [])
{
  VALUE x = args[0];

  double xdouble = double((int)x);
  VALUE float_obj = sp_float_new(sin(xdouble));
  return float_obj;
}

VALUE
math_cos(VALUE obj, VALUE args [])
{
  VALUE x = args[0];

  return (VALUE)2;
}

VALUE
math_tan(VALUE obj, VALUE args [])
{
  VALUE x = args[0];

  return (VALUE)3;
}

void
Init_Math()
{
  rb_mMath = rb_define_module("Math");

  rb_define_const(rb_mMath, "THIRTYSEVEN", (VALUE)37);

  rb_define_module_function(rb_mMath, "sin", math_sin, 1);
  rb_define_module_function(rb_mMath, "cos", math_cos, 1);
  rb_define_module_function(rb_mMath, "tan", math_tan, 1);
}
