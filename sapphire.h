#ifndef SAPPHIRE_H
#define SAPPHIRE_H

#include <string>
/*#include <unordered_map>*/
#include <map>

typedef uintptr_t VALUE;

/*
 * Fixnums are stored as immediate values. They are formed by shifted the integer left 1 bit
 * and setting the least significant bit to 1. Only Fixnum VALUEs will have a LSB of 1.
 */
inline VALUE int_to_value(int num) {
  VALUE val = (VALUE)num; // cast to VALUE
  val = val << 1; // shift left 1 bit
  val = val | 1; //set LSB to 1
  return val;
}

/*
 * Convert a VALUE to int
 */
inline int value_to_int(VALUE val) {
  return (int)(val >> 1); // shift right 1 bit and cast to int
}

/*
 * Check if VALUE is an int
 */
inline bool value_is_fixnum(VALUE val) {
  return (val & 1); // do a bitwise check of the LSB, only Fixnums have an LSB of 1
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

typedef VALUE (*function_ptr)(VALUE, VALUE);
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
    void sp_define_method(const char *name, function_ptr func);
    VALUE sp_call_method(VALUE receiver, const char *method_name, VALUE arg);
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

inline SClass * get_class_from_value(VALUE val)
{
  if (value_is_fixnum(val)){
    return &sp_cFixnum;
  } else {
    return NULL;
  }
}

inline VALUE sp_funcall(VALUE receiver, const char *method_id, int argc, VALUE arg)
{
  /* get class of receiver */
  SClass *s_class = get_class_from_value(receiver);

  /* call instance method with SClass::sp_method_call */
  VALUE retval = s_class->sp_call_method(receiver, method_id, arg);

  return retval;
}

#endif
