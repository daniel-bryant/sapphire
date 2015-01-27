#include "sapphire.h"

std::vector<RString *> global_symbols;

ID
intern_str(VALUE str)
{
  ID index = global_symbols.size();
  global_symbols.push_back(RSTRING(str));
  // TODO handle overflow of symbol IDs
  return index;
}

VALUE
lookup_str_sym(VALUE str)
{
  char *ptr = RSTRING(str)->ptr;
  std::vector<RString *>::iterator it;
  for (it = global_symbols.begin(); it != global_symbols.end(); it++) {
    if ( !strcmp((*it)->ptr, ptr) ) { break; }
  }

  if (it != global_symbols.end()) {
    long index = it - global_symbols.begin();
    VALUE sym = (VALUE)(((long)(index)) << 8 | SYMBOL_FLAG);
    return sym;
  } else {
    return (VALUE)0;
  }
}

ID
rb_intern3(const char *name, long len, int enc)
{
  VALUE sym;
  RString fake_str = {{0, 0}, 0, 0};
  VALUE str = rb_setup_fake_str(&fake_str, name, len, enc);
  // OBJ_FREEZE(str);

  sym = lookup_str_sym(str);
  if (sym) {
    return (ID)(sym >> 8);
  } else {
    str = rb_enc_str_new(name, len, enc); /* make true string */
    return intern_str(str);
  }
}

ID
rb_intern2(const char *name, long len)
{
  return rb_intern3(name, len, 0);
}

ID
rb_intern(const char *name)
{
  return rb_intern2(name, strlen(name));
}
