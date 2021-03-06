#include "sapphire.h"

VALUE rb_cBasicObject;
VALUE rb_cObject;
VALUE rb_cModule;
VALUE rb_cClass;

void
rb_class_subclass_add(VALUE super, VALUE klass)
{
  if (super && super != Qundef) {
    if (!(RCLASS_EXT(super)->subclasses)) {
      RCLASS_EXT(super)->subclasses = new std::vector<VALUE> ();
    }
    RCLASS_EXT(super)->subclasses->push_back(klass);
    RCLASS_EXT(klass)->parent_subclasses = &RCLASS_EXT(super)->subclasses;
  }
}

void
rb_class_remove_from_super_subclasses(VALUE klass)
{
  if (RCLASS_EXT(klass)->parent_subclasses) {
    std::vector<VALUE> **parent_subclasses = RCLASS_EXT(klass)->parent_subclasses;
    std::vector<VALUE>::iterator it;
    it = std::find((*parent_subclasses)->begin(), (*parent_subclasses)->end(), klass);
    (*parent_subclasses)->erase(it);
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
  RCLASS_IV_TBL(obj) = 0;
  RCLASS_CONST_TBL(obj) = 0;
  RCLASS_M_TBL_WRAPPER(obj) = 0;
  RCLASS_SET_SUPER((VALUE)obj, 0);
  //RCLASS_ORIGIN(obj) = (VALUE)obj;
  //RCLASS_IV_INDEX_TBL(obj) = 0;

  RCLASS_EXT(obj)->subclasses = NULL;
  RCLASS_EXT(obj)->parent_subclasses = NULL;
  RCLASS_EXT(obj)->module_subclasses = NULL;
  //RCLASS_SERIAL(obj) = rb_next_class_serial();

  //RCLASS_REFINED_CLASS(obj) = Qnil;
  RCLASS_EXT(obj)->allocator = 0;
  return (VALUE)obj;
}

VALUE
rb_class_boot(VALUE super)
{
  VALUE klass = class_alloc(T_CLASS, rb_cClass);
  RCLASS_SET_SUPER(klass, super);
  RCLASS_M_TBL_INIT(klass);

  // OBJ_INFECT(klass, super);
  return klass;
}

void
rb_singleton_class_attached(VALUE klass, VALUE obj)
{
  if (FL_TEST(klass, FL_SINGLETON)) {
    if (!RCLASS_IV_TBL(klass)) {
      RCLASS_IV_TBL(klass) = new id_value_map (); // st_init_numtable();
    }
    rb_st_insert_id_and_value(klass, RCLASS_IV_TBL(klass), rb_intern("id__attached__"), obj);
  }
}

#define METACLASS_OF(k) RBASIC(k)->klass
#define SET_METACLASS_OF(k, cls) RBASIC_SET_CLASS(k, cls)

VALUE
make_singleton_class(VALUE obj)
{
  VALUE orig_class = RBASIC(obj)->klass;
  VALUE klass = rb_class_boot(orig_class);

  FL_SET(klass, FL_SINGLETON);
  RBASIC_SET_CLASS(obj, klass);
  rb_singleton_class_attached(klass, obj);

  SET_METACLASS_OF(klass, METACLASS_OF(rb_class_real(orig_class)));
  return klass;
}

VALUE
boot_defclass(const char *name, VALUE super)
{
  VALUE obj = rb_class_boot(super);
  ID id = rb_intern(name);

  rb_name_class(obj, id);
  rb_const_set((rb_cObject ? rb_cObject : obj), id, obj);
  return obj;
}

void
Init_class_hierarchy()
{
  rb_cBasicObject = boot_defclass("BasicObject", 0);
  rb_cObject = boot_defclass("Object", rb_cBasicObject);
  rb_cModule = boot_defclass("Module", rb_cObject);
  rb_cClass = boot_defclass("Class", rb_cModule);

  rb_const_set(rb_cObject, rb_intern("BasicObject"), rb_cBasicObject);
  RBASIC_SET_CLASS(rb_cClass, rb_cClass);
  RBASIC_SET_CLASS(rb_cModule, rb_cClass);
  RBASIC_SET_CLASS(rb_cObject, rb_cClass);
  RBASIC_SET_CLASS(rb_cBasicObject, rb_cClass);
}

VALUE
rb_class_new(VALUE super)
{
  // Check_Type(super, T_CLASS);
  // rb_check_inheritable(super);
  return rb_class_boot(super);
}

VALUE
make_metaclass(VALUE klass)
{
  VALUE super;
  VALUE metaclass = rb_class_boot(Qundef);

  FL_SET(metaclass, FL_SINGLETON);
  rb_singleton_class_attached(metaclass, klass);

  // TODO fill this in
  //
  ////////////////////

  return metaclass;
}

VALUE
rb_make_metaclass(VALUE obj)
{
  if (BUILTIN_TYPE(obj) == T_CLASS) {
    return make_metaclass(obj);
  } else {
    return make_singleton_class(obj);
  }
}

VALUE
rb_define_class_id(ID id, VALUE super)
{
  if (!super) super = rb_cObject;
  VALUE klass = rb_class_new(super);
  // rb_make_metaclass(klass, RBASIC(super)->klass);

  return klass;
}

VALUE
rb_define_class(const char *name, VALUE super)
{
  VALUE klass;
  VALUE id;

  id = rb_intern(name);
  klass = rb_define_class_id(id, super);
  rb_name_class(klass, id);
  rb_const_set(rb_cObject, id, klass);

  return klass;
}

VALUE
rb_module_new()
{
  VALUE mdl = class_alloc(0, rb_cModule);
  RCLASS_M_TBL_INIT(mdl);
  return mdl;
}

VALUE
rb_define_module_id(ID id)
{
  VALUE module = rb_module_new();
  rb_name_class(module, id);
  return module;
}

VALUE
rb_define_module(const char *name)
{
  VALUE module;
  ID id;

  id = rb_intern(name);
  if (rb_const_defined(rb_cObject, id)) {
    module = rb_const_get(rb_cObject, id);
    // if module is a Module
    return module;
    // else raise error
  }
  module = rb_define_module_id(id);
  rb_const_set(rb_cObject, id, module);

  return module;
}

// Methods
void
rb_define_method(VALUE klass, const char *name, function_ptr func, int argc)
{
  rb_add_method_cfunc(klass, rb_intern(name), func, argc, NOEX_PUBLIC);
}

void
rb_define_private_method(VALUE klass, const char *name, function_ptr func, int argc)
{
  rb_add_method_cfunc(klass, rb_intern(name), func, argc, NOEX_PRIVATE);
}

VALUE
special_singleton_class_of(VALUE obj)
{
  if (obj == Qnil) return rb_cNilClass;
  if (obj == Qfalse) return rb_cFalseClass;
  if (obj == Qtrue) return rb_cTrueClass;
  return Qnil;
}

VALUE
singleton_class_of(VALUE obj)
{
  VALUE klass;

  if (FIXNUM_P(obj) || FLONUM_P(obj) || SYMBOL_P(obj)) {
    rb_raise("can't define singleton");
  }
  if (SPECIAL_CONST_P(obj)) {
    klass = special_singleton_class_of(obj);
    if (NIL_P(klass))
      rb_bug("unknown immediate");
    return klass;
  } else {
    int type = BUILTIN_TYPE(obj); //enum ruby_value_type type = BUILTIN_TYPE(obj);
    if (type == T_FLOAT || type == T_BIGNUM) {
      rb_raise("can't define singleton");
    }
  }

  klass = RBASIC(obj)->klass;
  if (!(FL_TEST(klass, FL_SINGLETON) && rb_ivar_get(klass, rb_intern("id__attached__")) == obj)) {
    klass = rb_make_metaclass(obj);
  }

  /*if (OBJ_TAINTED(obj)) {
    OBJ_TAINT(klass);
  }
  else {
    FL_UNSET(klass, FL_TAINT);
  }
  if (OBJ_FROZEN(obj)) OBJ_FREEZE_RAW(klass);*/

  return klass;
}

void
rb_define_singleton_method(VALUE obj, const char *name, function_ptr func, int argc)
{
  rb_define_method(singleton_class_of(obj), name, func, argc);
}

void
rb_define_module_function(VALUE module, const char *name, function_ptr func, int argc)
{
  rb_define_private_method(module, name, func, argc);
  rb_define_singleton_method(module, name, func, argc);
}
