#include "sapphire.h"

VALUE rb_cString;
VALUE rb_cSymbol;

VALUE
str_alloc(VALUE klass)
{
  // TODO use NEWOBJ_OF macro in TYPE_alloc's like this one
  RString *str = new RString ();
  RBASIC_SET_CLASS(str, klass);
  RBASIC(str)->flags |= (T_STRING & T_MASK);
  return (VALUE)str;
}

VALUE
str_new0(VALUE klass, const char *ptr, long len, int enc)
{
  VALUE str;

  if (len < 0) {
    // rb_raise
  }

  str = str_alloc(klass);

  RSTRING(str)->ptr = (char *)ptr;
  RSTRING(str)->len = len;
  return str;
}

VALUE
setup_fake_str(struct RString *fake_str, const char *name, long len, int enc)
{
  fake_str->basic.flags = T_STRING; // TODO more flags to add

  // ENCODING_SET_INLINED((VALUE)fake_str, encidx);
  
  RBASIC_SET_CLASS_RAW((VALUE)fake_str, rb_cString);
  fake_str->len = len;
  fake_str->ptr = (char *)name;
  // fake_str->as.heap.aux.capa = len; TODO add capa
  return (VALUE)fake_str;
}

VALUE
rb_setup_fake_str(struct RString *fake_str, const char *name, long len, int enc)
{
  return setup_fake_str(fake_str, name, len, enc); // TODO ruby does something with enc here
}

VALUE
rb_enc_str_new(const char *ptr, long len, int enc)
{
  VALUE str;

  str = str_new0(rb_cString, ptr, len, enc);

  return str;
}

void
Init_String()
{
  rb_cString  = sp_define_class("String", rb_cObject);


  rb_cSymbol = sp_define_class("Symbol", rb_cObject);
}
