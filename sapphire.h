#ifndef SAPPHIRE_H
#define SAPPHIRE_H

#include <string>
/*#include <unordered_map>*/
#include <map>

typedef uintptr_t VALUE;

inline VALUE str_to_value(const char * str) {
  int i = atoi(str);
  return (VALUE)i;
}

inline int value_to_int(VALUE val) {
  return (int)val;
}

/*
class SBasic
{
  VALUE flags;
  const VALUE klass;
};

class SObject {
  long numiv;
  //strvalmap ivmap;
};*/

typedef int (*function_ptr)(int, int);
typedef std::map<std::string, function_ptr> function_map;
/* SClass - internal structure for all classes in Sapphire */
class SClass {
  SClass *klass;
  SClass *super;
  const char *name;
  function_map method_table;

  int iv_table;
  int const_table;
  int subclasses;
  int parent_subclasses;
  int module_subclasses;

  public:
    SClass ();
    SClass (const char* name_in, SClass *super_in);
    void set_class(SClass *class_in);
    void define_instance_method(const char *name, function_ptr func);
};


typedef std::map<std::string, SClass *> sclass_map;
/* SpGlobals - storage mechanism for global constants e.g. BasicObject, Class, Math, etc. */
class SpGlobals
{
  sclass_map sp_constants;

  public:
    SpGlobals ();
    void set_constant(const char *name, SClass *klass);
    SClass * get_constant(const char *name);
};
extern SpGlobals sp_Globals;



extern SClass sp_cBasicObject;
extern SClass sp_cObject;
extern SClass sp_cModule;
extern SClass sp_cClass;
extern SClass sp_cFixnum;

void Init_var_tables();
void Init_class_hierarchy();
void Init_Numeric();

#endif
