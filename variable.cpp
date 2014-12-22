#include "sapphire.h"

SpGlobals sp_Globals;

void
Init_var_tables()
{
  //sp_Globals = SpGlobals ();
}

SpGlobals::SpGlobals () {}

void SpGlobals::set_constant(const char *name, SClass *klass)
{
  std::string strname = std::string (name);
  sp_constants[strname] = klass;
}

SClass * SpGlobals::get_constant(const char *name)
{
  std::string strname = std::string (name);
  SClass *klass = sp_constants[strname];
  return klass;
}
