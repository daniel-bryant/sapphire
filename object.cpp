#include "sapphire.h"

VALUE
rb_class_real(VALUE cl)
{
  // TODO uncomment this and make it work. returning cl should work for now
  //while (cl && ((RBASIC(cl)->flags & FL_SINGLETON) || BUILTIN_TYPE(cl) == T_ICLASS)) {
  //  cl = RCLASS_SUPER(cl);
  //}
  return cl;
}
