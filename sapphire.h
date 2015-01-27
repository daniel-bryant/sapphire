#ifndef SAPPHIRE_H
#define SAPPHIRE_H

#include <string>
#include <vector>
/*#include <unordered_map>*/
#include <map>

typedef uintptr_t VALUE;
typedef uintptr_t ID;
#define SIGNED_VALUE intptr_t

enum ruby_special_consts {
    RUBY_Qfalse = 0,		/* ...0000 0000 */
    RUBY_Qtrue  = 2,		/* ...0000 0010 */
    RUBY_Qnil   = 4,		/* ...0000 0100 */
    RUBY_Qundef = 6,		/* ...0000 0110 */

    RUBY_IMMEDIATE_MASK = 0x03,
    RUBY_FIXNUM_FLAG    = 0x01,	/* ...xxxx xxx1 */
    RUBY_FLONUM_MASK    = 0x00,	/* any values ANDed with FLONUM_MASK cannot be FLONUM_FLAG */
    RUBY_FLONUM_FLAG    = 0x02,
    RUBY_SYMBOL_FLAG    = 0x0e,	/* ...0000 1110 */

    RUBY_SPECIAL_SHIFT  = 8
};

#define Qfalse ((VALUE)RUBY_Qfalse)
#define Qtrue  ((VALUE)RUBY_Qtrue)
#define Qnil   ((VALUE)RUBY_Qnil)
#define Qundef ((VALUE)RUBY_Qundef) /* undefined value for placeholder */
#define IMMEDIATE_MASK RUBY_IMMEDIATE_MASK
#define FIXNUM_FLAG RUBY_FIXNUM_FLAG
#define SYMBOL_FLAG RUBY_SYMBOL_FLAG

#define RTEST(v) !(((VALUE)(v) & ~Qnil) == 0)
#define NIL_P(v) !((VALUE)(v) != Qnil)

#define FIXNUM_P(f) (((int)(SIGNED_VALUE)(f))&FIXNUM_FLAG)
#define IMMEDIATE_P(x) ((VALUE)(x) & IMMEDIATE_MASK)

#define STATIC_SYM_P(x) (((VALUE)(x)&~((~(VALUE)0)<<RUBY_SPECIAL_SHIFT))==SYMBOL_FLAG)

enum ruby_value_type {
    RUBY_T_NONE   = 0x00,

    RUBY_T_OBJECT = 0x01,
    RUBY_T_CLASS  = 0x02,
    RUBY_T_MODULE = 0x03,
    RUBY_T_FLOAT  = 0x04,
    RUBY_T_STRING = 0x05,
    RUBY_T_REGEXP = 0x06,
    RUBY_T_ARRAY  = 0x07,
    RUBY_T_HASH   = 0x08,
    RUBY_T_STRUCT = 0x09,
    RUBY_T_BIGNUM = 0x0a,
    RUBY_T_FILE   = 0x0b,
    RUBY_T_DATA   = 0x0c,
    RUBY_T_MATCH  = 0x0d,
    RUBY_T_COMPLEX  = 0x0e,
    RUBY_T_RATIONAL = 0x0f,

    RUBY_T_NIL    = 0x11,
    RUBY_T_TRUE   = 0x12,
    RUBY_T_FALSE  = 0x13,
    RUBY_T_SYMBOL = 0x14,
    RUBY_T_FIXNUM = 0x15,

    RUBY_T_UNDEF  = 0x1b,
    RUBY_T_NODE   = 0x1c,
    RUBY_T_ICLASS = 0x1d,
    RUBY_T_ZOMBIE = 0x1e,

    RUBY_T_MASK   = 0x1f
};

#define T_NONE   RUBY_T_NONE
#define T_NIL    RUBY_T_NIL
#define T_OBJECT RUBY_T_OBJECT
#define T_CLASS  RUBY_T_CLASS
#define T_ICLASS RUBY_T_ICLASS
#define T_MODULE RUBY_T_MODULE
#define T_FLOAT  RUBY_T_FLOAT
#define T_STRING RUBY_T_STRING
#define T_REGEXP RUBY_T_REGEXP
#define T_ARRAY  RUBY_T_ARRAY
#define T_HASH   RUBY_T_HASH
#define T_STRUCT RUBY_T_STRUCT
#define T_BIGNUM RUBY_T_BIGNUM
#define T_FILE   RUBY_T_FILE
#define T_FIXNUM RUBY_T_FIXNUM
#define T_TRUE   RUBY_T_TRUE
#define T_FALSE  RUBY_T_FALSE
#define T_DATA   RUBY_T_DATA
#define T_MATCH  RUBY_T_MATCH
#define T_SYMBOL RUBY_T_SYMBOL
#define T_RATIONAL RUBY_T_RATIONAL
#define T_COMPLEX RUBY_T_COMPLEX
#define T_UNDEF  RUBY_T_UNDEF
#define T_NODE   RUBY_T_NODE
#define T_ZOMBIE RUBY_T_ZOMBIE
#define T_MASK   RUBY_T_MASK

#define BUILTIN_TYPE(x) (int)(RBASIC(x)->flags & T_MASK)

#define FL_SINGLETON (((VALUE)1)<<12)
#define FL_SET(x,f) (RBASIC(x)->flags |= f)

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

struct RBasic
{
  VALUE flags;
  const VALUE klass;
};

struct RObject {
  struct RBasic basic;
  long numiv;
  //strvalmap ivmap;
};

typedef VALUE (*function_ptr)(VALUE, VALUE []);
typedef std::map<VALUE, function_ptr> function_map;
typedef std::map<ID, VALUE> id_value_map;

typedef unsigned long long int rb_serial_t;
// from include/ruby/intern.h
typedef VALUE (*rb_alloc_func_t)(VALUE);

struct rb_classext_struct {
  id_value_map iv_index_tbl;
  id_value_map iv_tbl; // TODO make this a pointer maybe
  id_value_map const_tbl;
  std::vector<VALUE> subclasses;
  std::vector<VALUE> *parent_subclasses;
  /**
   * In the case that this is an `ICLASS`, `module_subclasses` points to the link
   * in the module's `subclasses` list that indicates that the klass has been
   * included. Hopefully that makes sense.
   */
  std::vector<VALUE> *module_subclasses;
  rb_serial_t class_serial;
  VALUE origin;
  VALUE refined_class;
  rb_alloc_func_t allocator;
};

struct method_table_wrapper {
  std::map<VALUE, function_ptr> tbl;
  size_t serial;
};

// this struct should be moved to internal.h
typedef struct rb_classext_struct rb_classext_t;

struct RClass {
  struct RBasic basic;
  VALUE super;
  rb_classext_t *ptr;
  struct method_table_wrapper *m_tbl_wrapper;
};

struct RString {
  struct RBasic basic;
  long len;
  char *ptr;
};

struct RFloat {
  struct RBasic basic;
  double float_value;
};

#define R_CAST(st)   (struct st*)
#define RBASIC(obj)  (R_CAST(RBasic)(obj))
#define ROBJECT(obj) (R_CAST(RObject)(obj))
#define RCLASS(obj)  (R_CAST(RClass)(obj))
#define RMODULE(obj) RCLASS(obj)
#define RSTRING(obj) (R_CAST(RString)(obj))

// this also goes in internal.h
struct RBasicRaw {
  VALUE flags;
  VALUE klass;
};
#define RBASIC_SET_CLASS_RAW(obj, cls) (((struct RBasicRaw *)obj)->klass = (cls))
#define RBASIC_SET_CLASS(obj, cls) (((struct RBasicRaw *)obj)->klass = (cls))




extern id_value_map rb_global_tbl;
extern ID autoload, classpath, tmp_classpath, classid;

extern std::vector<RString *> global_symbols;

extern VALUE rb_mKernel;
extern VALUE rb_mMath;

extern VALUE rb_cBasicObject;
extern VALUE rb_cObject;
extern VALUE rb_cModule;
extern VALUE rb_cClass;
extern VALUE rb_cFalseClass;
extern VALUE rb_cFloat;
extern VALUE rb_cFixnum;
extern VALUE rb_cNilClass;
extern VALUE rb_cString;
extern VALUE rb_cSymbol;
extern VALUE rb_cTrueClass;

/* class.cpp */
void Init_class_hierarchy();
void rb_class_subclass_add(VALUE super, VALUE klass);
void rb_class_remove_from_super_subclasses(VALUE klass);
VALUE rb_define_class(const char *name, VALUE super);
VALUE rb_define_module(const char *name);
void rb_define_method(VALUE klass, const char *name, function_ptr func, int argc);
void rb_define_module_function(VALUE module, const char *name, function_ptr func, int argc);

/* math.cpp */
void Init_Math();

/* numeric.cpp */
void Init_Numeric();
VALUE sp_float_new(double d);

/* object.cpp */
void Init_Object();
VALUE rb_class_real(VALUE cl);

/* string.cpp */
void Init_String();
VALUE rb_enc_str_new(const char *name, long len, int enc);

/* symbol.cpp */
ID rb_intern(const char *name);
ID rb_intern2(const char *name, long len);
#define rb_intern_const(str) rb_intern2((str), (long)strlen(str))

/* variable.cpp */
void Init_var_tables();
VALUE rb_const_get(VALUE klass, ID id);
void rb_define_const(VALUE klass, const char *name, VALUE val);
void sp_const_set(VALUE klass, VALUE id, VALUE val);
VALUE sp_const_get(VALUE klass, VALUE id);
bool sp_const_defined(VALUE klass, VALUE id);
void sp_name_class(VALUE klass, ID id);
int sp_st_insert_id_and_value(VALUE obj, id_value_map *tbl, ID key, VALUE value);

/* vm_method.cpp */
/* possibly move the stuff below to method.h */
typedef enum {
  NOEX_PUBLIC    = 0x00,
  NOEX_NOSUPER   = 0x01,
  NOEX_PRIVATE   = 0x02
} rb_method_flag_t;
void sp_add_method_cfunc(VALUE klass, ID mid, function_ptr func, int argc, rb_method_flag_t noex);

inline void alpha_ruby_init()
{
  Init_var_tables();

  Init_Object();

  Init_Math();
  Init_Numeric();
  Init_String();
}

inline VALUE sp_funcall(VALUE receiver, const char *name, int argc, VALUE arg)
{
  /* get class of receiver */

  // VALUE retval = ((SClass *) receiver)->sp_call_method(receiver, name, arg);
  VALUE retval = 0;

  return retval;
}

/* in the ruby source, this is in internal.h */
#define RCLASS_EXT(c) (RCLASS(c)->ptr)
#define RCLASS_M_TBL_WRAPPER(c) (RCLASS(c)->m_tbl_wrapper)
#define RCLASS_CONST_TBL(c) (RCLASS_EXT(c)->const_tbl)
inline void
RCLASS_M_TBL_INIT(VALUE c)
{
  struct method_table_wrapper *wrapper;
  wrapper = new method_table_wrapper ();
  //wrapper->tbl = st_init_numtable();
  wrapper->serial = 0;
  RCLASS_M_TBL_WRAPPER(c) = wrapper;
}

inline VALUE
RCLASS_SET_SUPER(VALUE klass, VALUE super)
{
  if (super) {
    rb_class_remove_from_super_subclasses(klass);
    rb_class_subclass_add(super, klass);
  }
  RCLASS(klass)->super = super; // RB_OBJ_WRITE(klass, &RCLASS(klass)->super, super);
  return super;
}

VALUE rb_setup_fake_str(struct RString *fake_str, const char *name, long len, int enc);

inline VALUE
rb_class_of(VALUE obj)
{
  if (IMMEDIATE_P(obj)) {
    if (FIXNUM_P(obj)) return rb_cFixnum;
    //if (FLONUM_P(obj)) return rb_cFloat;
    if (obj == Qtrue)  return rb_cTrueClass;
    if (STATIC_SYM_P(obj)) return rb_cSymbol;
  } else if (!RTEST(obj)) {
    if (obj == Qnil)   return rb_cNilClass;
    if (obj == Qfalse) return rb_cFalseClass;
  }
  return RBASIC(obj)->klass;
}

inline int
rb_type(VALUE obj)
{
  if (IMMEDIATE_P(obj)) {
    if (FIXNUM_P(obj)) return T_FIXNUM;
    //if (FLONUM_P(obj)) return T_FLOAT; // TODO we don't have floats defined yet
    if (obj == Qtrue)  return T_TRUE;
    if (STATIC_SYM_P(obj)) return T_SYMBOL;
    if (obj == Qundef) return T_UNDEF;
  } else if (!RTEST(obj)) {
    if (obj == Qnil)   return T_NIL;
    if (obj == Qfalse) return T_FALSE;
  }
  return BUILTIN_TYPE(obj);
}

#endif
