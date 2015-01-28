#include "sapphire.h"

VALUE rb_cHash;

VALUE
hash_alloc(VALUE klass)
{
  RHash *hash = new RHash ();
  RBASIC_SET_CLASS(hash, klass);
  RBASIC(hash)->flags |= (T_HASH & T_MASK);

  // RHASH_SET_IFNONE((VALUE)hash, Qnil);

  return (VALUE)hash;
}

VALUE
rb_hash_new()
{
  return hash_alloc(rb_cHash);
}
