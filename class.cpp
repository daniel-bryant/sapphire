#include "sapphire.h"

SClass sp_cBasicObject;
SClass sp_cObject;
SClass sp_cModule;
SClass sp_cClass;

SClass
boot_defclass(const char *name, SClass *super)
{
  SClass obj = SClass (name, super);
  sp_Globals.set_constant(name, &obj);
  //rb_const_set((sp_cObject ? sp_cObject : obj), name, obj); //make name id instead
  return obj;
}

void
Init_class_hierarchy()
{
  sp_cBasicObject = boot_defclass("BasicObject", 0);
  sp_cObject = boot_defclass("Object", &sp_cBasicObject);
  sp_cModule = boot_defclass("Module", &sp_cObject);
  sp_cClass = boot_defclass("Class", &sp_cModule);

  sp_cClass.set_class(&sp_cClass);
  sp_cModule.set_class(&sp_cClass);
  sp_cObject.set_class(&sp_cClass);
  sp_cBasicObject.set_class(&sp_cClass);
}

SClass::SClass ()
{
  name = NULL;
  super = NULL;
}

SClass::SClass (const char *name_in, SClass *super_in)
{
  name = name_in;
  super = super_in;
}

void SClass::set_class(SClass *class_in)
{
  klass = class_in;
}

void SClass::define_instance_method(const char *method_name, function_ptr func)
{
  std::string fname = std::string (method_name);
  method_table[fname] = func;
}
