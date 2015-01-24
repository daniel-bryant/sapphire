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

void
sp_const_set(VALUE klass, VALUE id, VALUE val)
{
  RCLASS_EXT(klass)->const_tbl[id] = val;
}

VALUE
sp_const_get(VALUE klass, VALUE id)
{
  return RCLASS_EXT(klass)->const_tbl[id];
}

bool
sp_const_defined(VALUE klass, VALUE id)
{
  id_value_map::iterator it = RCLASS_EXT(klass)->const_tbl.find(id);
  return it != RCLASS_EXT(klass)->const_tbl.end();
}

void
sp_name_class(VALUE klass, ID id)
{
  //TODO fully implement this when we have ivars
  RCLASS_EXT(klass)->iv_tbl[classid] = id;
}

int sp_st_insert_id_and_value(VALUE obj, id_value_map *tbl, ID key, VALUE value)
{
  (*tbl)[key] = value;
  return 0;
}
