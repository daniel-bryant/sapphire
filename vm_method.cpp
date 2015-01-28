#include "sapphire.h"

void
rb_add_method_cfunc(VALUE klass, ID mid, function_ptr func, int argc, rb_method_flag_t noex)
{
  // TODO fill in this method
  RCLASS(klass)->m_tbl_wrapper->tbl[mid] = func;
}
