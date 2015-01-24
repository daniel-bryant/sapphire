#include "sapphire.h"

VALUE sp_cBasicObject;
VALUE sp_cObject;
VALUE sp_cModule;
VALUE sp_cClass;

void
rb_class_subclass_add(VALUE super, VALUE klass)
{
  if (super && super != Qundef) {
    RCLASS_EXT(super)->subclasses.push_back(klass);
    RCLASS_EXT(klass)->parent_subclasses = &RCLASS_EXT(super)->subclasses;
  }
}

void
rb_class_remove_from_super_subclasses(VALUE klass)
{
  if (RCLASS_EXT(klass)->parent_subclasses) {
    std::vector<VALUE> *parent_subclasses = RCLASS_EXT(klass)->parent_subclasses;
    std::vector<VALUE>::iterator it;
    it = std::find(parent_subclasses->begin(), parent_subclasses->end(), klass);
    parent_subclasses->erase(it);
  }

  RCLASS_EXT(klass)->parent_subclasses = NULL;
}

/*
 * class_alloc
 */
VALUE
class_alloc(VALUE flags, VALUE klass)
{
  RClass *obj = new RClass ();
  RBASIC_SET_CLASS(obj, klass);
  RBASIC(obj)->flags |= (flags & T_MASK);
  RCLASS(obj)->ptr = new rb_classext_t ();
  // FILL THIS IN
  //
  //

  return (VALUE)obj;
}

VALUE
sp_class_boot(VALUE super)
{
  VALUE klass = class_alloc(T_CLASS, sp_cClass);
  RCLASS_SET_SUPER(klass, super);
  RCLASS_M_TBL_INIT(klass);

  // OBJ_INFECT(klass, super);
  return klass;
}

void
rb_singleton_class_attached(VALUE klass, VALUE obj)
{
  //if (FL_TEST(klass, FL_SINGLETON)) {
    //if (!RCLASS_IV_TBL(klass)) {
      //RCLASS_IV_TBL(klass) = st_init_numtable();
    //}
    sp_st_insert_id_and_value(klass, &(RCLASS_EXT(klass)->iv_tbl), rb_intern("id__attached__"), obj);
  //}
}

#define METACLASS_OF(k) RBASIC(k)->klass
#define SET_METACLASS_OF(k, cls) RBASIC_SET_CLASS(k, cls)

VALUE
make_singleton_class(VALUE obj)
{
  VALUE orig_class = RBASIC(obj)->klass;
  VALUE klass = sp_class_boot(orig_class);

  FL_SET(klass, FL_SINGLETON);
  RBASIC_SET_CLASS(obj, klass);
  rb_singleton_class_attached(klass, obj);

  SET_METACLASS_OF(klass, METACLASS_OF(rb_class_real(orig_class)));
  return klass;
}

VALUE
boot_defclass(const char *name, VALUE super)
{
  VALUE obj = sp_class_boot(super);
  ID id = rb_intern(name);

  sp_name_class(obj, id);
  sp_const_set((sp_cObject ? sp_cObject : obj), id, obj);
  return obj;
}

void
Init_class_hierarchy()
{
  sp_cBasicObject = boot_defclass("BasicObject", 0);
  sp_cObject = boot_defclass("Object", sp_cBasicObject);
  sp_cModule = boot_defclass("Module", sp_cObject);
  sp_cClass = boot_defclass("Class", sp_cModule);

  sp_const_set(sp_cObject, rb_intern("BasicObject"), sp_cBasicObject);
  // RBASIC_SET_CLASS(rb_cClass, rb_cClass);
  // RBASIC_SET_CLASS(rb_cModule, rb_cClass);
  // RBASIC_SET_CLASS(rb_cObject, rb_cClass);
  // RBASIC_SET_CLASS(rb_cBasicObject, rb_cClass);
}

VALUE
sp_class_new(VALUE super)
{
  // Check_Type(super, T_CLASS);
  // rb_check_inheritable(super);
  return sp_class_boot(super);
}

VALUE
make_metaclass(VALUE klass)
{
  VALUE super;
  VALUE metaclass = sp_class_boot(Qundef);

  FL_SET(metaclass, FL_SINGLETON);
  rb_singleton_class_attached(metaclass, klass);

  // TODO fill this in
  //
  ////////////////////

  return metaclass;
}

VALUE
sp_make_metaclass(VALUE obj)
{
  if (BUILTIN_TYPE(obj) == T_CLASS) {
    return make_metaclass(obj);
  } else {
    return make_singleton_class(obj);
  }
}

VALUE
sp_define_class_id(ID id, VALUE super)
{
  if (!super) super = sp_cObject;
  VALUE klass = sp_class_new(super);
  // rb_make_metaclass(klass, RBASIC(super)->klass);

  return klass;
}

VALUE
sp_define_class(const char *name, VALUE super)
{
  VALUE klass;
  VALUE id;

  id = rb_intern(name);
  klass = sp_define_class_id(id, super);
  sp_name_class(klass, id);
  sp_const_set(sp_cObject, id, klass);

  return klass;
}

VALUE
sp_module_new()
{
  VALUE mdl = class_alloc(0, sp_cModule);
  RCLASS_M_TBL_INIT(mdl);
  return mdl;
}

VALUE
sp_define_module_id(ID id)
{
  VALUE module = sp_module_new();
  sp_name_class(module, id);
  return module;
}

VALUE
sp_define_module(const char *name)
{
  VALUE module;
  ID id;

  id = rb_intern(name);
  if (sp_const_defined(sp_cObject, id)) {
    module = sp_const_get(sp_cObject, id);
    // if module is a Module
    return module;
    // else raise error
  }
  module = sp_define_module_id(id);
  sp_const_set(sp_cObject, id, module);

  return module;
}

// Methods
void
sp_define_method(VALUE klass, const char *name, function_ptr func, int argc)
{
  sp_add_method_cfunc(klass, rb_intern(name), func, argc, NOEX_PUBLIC);
}

void
sp_define_private_method(VALUE klass, const char *name, function_ptr func, int argc)
{
  sp_add_method_cfunc(klass, rb_intern(name), func, argc, NOEX_PRIVATE);
}


VALUE
singleton_class_of(VALUE obj)
{
  VALUE klass;

  klass = RBASIC(obj)->klass;
  if (true) {
    klass = sp_make_metaclass(obj);
  }

  return klass;
}

void
sp_define_singleton_method(VALUE obj, const char *name, function_ptr func, int argc)
{
  sp_define_method(singleton_class_of(obj), name, func, argc);
}

void
sp_define_module_function(VALUE module, const char *name, function_ptr func, int argc)
{
  sp_define_private_method(module, name, func, argc);
  sp_define_singleton_method(module, name, func, argc);
}
