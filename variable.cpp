#include "sapphire.h"

id_value_map rb_global_tbl;
ID autoload, classpath, tmp_classpath, classid;

void
Init_var_tables()
{
  // rb_global_tbl = st_init_numtable();
  autoload = rb_intern_const("__autoload__");
  /* __classpath__: fully qualified class path */
  classpath = rb_intern_const("__classpath__");
  /* __tmp_classpath__: temporary class path which contains anonymous names */
  tmp_classpath = rb_intern_const("__tmp_classpath__");
  /* __classid__: name given to class/module under an anonymous namespace */
  classid = rb_intern_const("__classid__");
}

VALUE
rb_const_get_0(VALUE klass, ID id)
{
  // TODO fill this in
  return RCLASS_CONST_TBL(klass)[id];
}

VALUE
rb_const_get(VALUE klass, ID id)
{
  return rb_const_get_0(klass, id);
}

void
rb_const_set(VALUE klass, ID id, VALUE val)
{
  // TODO obviously fill this method in, but this is thei MOST basic implementation for now.
  // This is equivalent to 'st_insert(RCLASS_CONST_TBL(klass), (st_data_t)id, (st_data_t)ce);'
  // in Ruby's source.
  RCLASS_CONST_TBL(klass)[id] = val;
}

void
rb_define_const(VALUE klass, const char *name, VALUE val)
{
  ID id = rb_intern(name);
  // if (!rb_is_const_id(id)) {
  //   rb_warn("rb_define_const: invalid name `%s' for constant", name);
  // }
  rb_const_set(klass, id, val);
}

bool
rb_const_defined(VALUE klass, VALUE id)
{
  id_value_map::iterator it = RCLASS_EXT(klass)->const_tbl.find(id);
  return it != RCLASS_EXT(klass)->const_tbl.end();
}

void
rb_name_class(VALUE klass, ID id)
{
  //TODO fully implement this when we have ivars
  RCLASS_EXT(klass)->iv_tbl[classid] = id;
}

int rb_st_insert_id_and_value(VALUE obj, id_value_map *tbl, ID key, VALUE value)
{
  (*tbl)[key] = value;
  return 0;
}
