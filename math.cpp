#include "sapphire.h"
#include <math.h>

VALUE sp_mMath;

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
  sp_mMath = sp_define_module("Math");

  sp_define_module_function(sp_mMath, "sin", math_sin, 1);
  sp_define_module_function(sp_mMath, "cos", math_cos, 1);
  sp_define_module_function(sp_mMath, "tan", math_tan, 1);
}