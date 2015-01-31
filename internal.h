#ifndef RUBY_INTERNAL_H
#define RUBY_INTERNAL_H

#define RUBY_BIT_ROTL(v, n) (((v) << (n)) | ((v) >> ((sizeof(v) * 8) - n)))
#define RUBY_BIT_ROTR(v, n) (((v) >> (n)) | ((v) << ((sizeof(v) * 8) - n)))

inline double
rb_float_value_inline(VALUE v)
{
  if (FLONUM_P(v)) {
    if (v != (VALUE)0x8000000000000002) { /* LIKELY */
      union {
        double d;
        VALUE v;
      } t;

      VALUE b63 = (v >> 63);
      /* e: xx1... -> 011... */
      /*    xx0... -> 100... */
      /*      ^b63           */
      t.v = RUBY_BIT_ROTR((2 - b63) | (v & ~0x03), 3);
      return t.d;
    } else {
      return 0.0;
    }
  }
  return ((struct RFloat *)v)->float_value;
}

inline VALUE
rb_float_new_inline(double d)
{
  union {
    double d;
    VALUE v;
  } t;
  int bits;

  t.d = d;
  bits = (int)((VALUE)(t.v >> 60) & 0x7);
  /* bits contains 3 bits of b62..b60. */
  /* bits - 3 = */
  /*   b011 -> b000 */
  /*   b100 -> b001 */

  if (t.v != 0x3000000000000000 /* 1.72723e-77 */ && !((bits-3) & ~0x01)) {
    return (RUBY_BIT_ROTL(t.v, 3) & ~(VALUE)0x01) | 0x02;
  } else if (t.v == (VALUE)0) {
    /* +0.0 */
    return 0x8000000000000002;
  }
  /* out of range */
  return rb_float_new_in_heap(d);
}

#define rb_float_value(v) rb_float_value_inline(v)
#define rb_float_new(d)   rb_float_new_inline(d)

#endif
