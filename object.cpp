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

void
Init_Object()
{
  Init_class_hierarchy();

  rb_mKernel = rb_define_module("Kernel");

  rb_cNilClass = rb_define_class("NilClass", rb_cObject);

  rb_cTrueClass = rb_define_class("TrueClass", rb_cObject);

  rb_cFalseClass = rb_define_class("FalseClass", rb_cObject);
}
