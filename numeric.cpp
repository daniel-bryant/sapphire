#include "sapphire.h"

SClass sp_cFixnum;

VALUE
fix_plus(VALUE self, VALUE num2)
{
  int result = value_to_int(self) + value_to_int(num2); // convert to int and add
  return int_to_value(result);
}

VALUE
fix_minus(VALUE self, VALUE num2)
{
  int result = value_to_int(self) - value_to_int(num2);
  return int_to_value(result);
}

VALUE
fix_times(VALUE self, VALUE num2)
{
  int result = value_to_int(self) * value_to_int(num2);
  return int_to_value(result);
}

VALUE
fix_divide(VALUE self, VALUE num2)
{
  int result = value_to_int(self) / value_to_int(num2);
  return int_to_value(result);
}

void
Init_Numeric()
{
  sp_cFixnum = SClass ("Fixnum", &sp_cClass);
  sp_cFixnum.sp_define_method("+", fix_plus);
  sp_cFixnum.sp_define_method("-", fix_minus);
  sp_cFixnum.sp_define_method("*", fix_times);
  sp_cFixnum.sp_define_method("/", fix_divide);
}
