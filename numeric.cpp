#include "sapphire.h"

SClass sp_cFixnum;

void
Init_Numeric()
{
  sp_cFixnum = SClass ("Fixnum", &sp_cClass);
}
