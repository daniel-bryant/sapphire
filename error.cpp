#include "sapphire.h"

void
rb_bug(const char *fmt)
{
  throw std::string(fmt);
}

void
rb_raise(const char *fmt)
{
  throw std::string(fmt);
}
