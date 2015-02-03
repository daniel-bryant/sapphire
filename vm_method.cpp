#include "sapphire.h"

void
rb_add_method_cfunc(VALUE klass, ID mid, function_ptr func, int argc, rb_method_flag_t noex)
{
  // TODO fill in this method
  RCLASS(klass)->m_tbl_wrapper->tbl[mid] = func;
}

function_ptr
lookup_method_table(VALUE klass, ID id)
{
  if (RCLASS(klass)->m_tbl_wrapper->tbl.count(id)) {
    return RCLASS(klass)->m_tbl_wrapper->tbl.at(id);
  } else {
    return 0;
  }
}

function_ptr
search_method(VALUE klass, ID id, VALUE *defined_class_ptr)
{
  function_ptr me;

  for (me = 0; klass; klass = RCLASS_SUPER(klass)) {
    if ((me = lookup_method_table(klass, id)) != 0) break;
  }

  if (defined_class_ptr) *defined_class_ptr = klass;
  return me;
}

function_ptr
rb_method_entry_get_without_cache(VALUE klass, ID id, VALUE *defined_class_ptr)
{
  VALUE defined_class;
  function_ptr me = search_method(klass, id, &defined_class);

  if (defined_class_ptr) *defined_class_ptr = defined_class;
  return me;
}

function_ptr
rb_method_entry(VALUE klass, ID id, VALUE *defined_class_ptr)
{
  return rb_method_entry_get_without_cache(klass, id, defined_class_ptr);
}
