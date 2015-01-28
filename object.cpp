#include "sapphire.h"

VALUE rb_mKernel;
VALUE rb_cFalseClass;
VALUE rb_cNilClass;
VALUE rb_cTrueClass;

VALUE
rb_class_real(VALUE cl)
{
  // TODO uncomment this and make it work. returning cl should work for now
  //while (cl && ((RBASIC(cl)->flags & FL_SINGLETON) || BUILTIN_TYPE(cl) == T_ICLASS)) {
  //  cl = RCLASS_SUPER(cl);
  //}
  return cl;
}

VALUE
nil_to_i(VALUE obj, VALUE args [])
{
  return INT2FIX(0);
}

VALUE
nil_to_f(VALUE obj, VALUE args [])
{
  return DBL2NUM(0.0);
}

VALUE
nil_to_s(VALUE obj, VALUE args [])
{
  return rb_usascii_str_new(0, 0);
}

VALUE
nil_to_a(VALUE obj, VALUE args [])
{
  return rb_ary_new2(0);
}

VALUE
nil_to_h(VALUE obj, VALUE args [])
{
  return rb_hash_new();
}

VALUE
nil_inspect(VALUE obj, VALUE args [])
{
  return rb_usascii_str_new2("nil");
}

VALUE
false_and(VALUE obj, VALUE args [])
{
  VALUE obj2 = args[0];
  return Qfalse;
}

VALUE
false_or(VALUE obj, VALUE args [])
{
  VALUE obj2 = args[0];
  return RTEST(obj2)?Qtrue:Qfalse;
}

VALUE
false_xor(VALUE obj, VALUE args [])
{
  VALUE obj2 = args[0];
  return RTEST(obj2)?Qtrue:Qfalse;
}

VALUE
rb_true(VALUE obj, VALUE args [])
{
  return Qtrue;
}

VALUE
rb_false(VALUE obj, VALUE args [])
{
  return Qfalse;
}

void
Init_Object()
{
  Init_class_hierarchy();

  rb_mKernel = rb_define_module("Kernel");

  rb_cNilClass = rb_define_class("NilClass", rb_cObject);
  rb_define_method(rb_cNilClass, "to_i", nil_to_i, 0);
  rb_define_method(rb_cNilClass, "to_f", nil_to_f, 0);
  rb_define_method(rb_cNilClass, "to_s", nil_to_s, 0);
  rb_define_method(rb_cNilClass, "to_a", nil_to_a, 0);
  rb_define_method(rb_cNilClass, "to_h", nil_to_h, 0);
  rb_define_method(rb_cNilClass, "inspect", nil_inspect, 0);
  rb_define_method(rb_cNilClass, "&", false_and, 1);
  rb_define_method(rb_cNilClass, "|", false_or, 1);
  rb_define_method(rb_cNilClass, "^", false_xor, 1);

  rb_define_method(rb_cNilClass, "nil?", rb_true, 0);
  //rb_undef_alloc_func(rb_cNilClass);
  //rb_undef_method(CLASS_OF(rb_cNilClass), "new");
  /*
   * An alias of +nil+
   */
  //rb_define_global_const("NIL", Qnil);

  rb_cTrueClass = rb_define_class("TrueClass", rb_cObject);

  rb_cFalseClass = rb_define_class("FalseClass", rb_cObject);
}
