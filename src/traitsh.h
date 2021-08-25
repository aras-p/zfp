/* half-precision floating-point traits */

#define Scalar uint16      /* floating-point type */
#define Int int16          /* corresponding signed integer type */
#define UInt uint16        /* corresponding unsigned integer type */
#define EBITS 5            /* number of exponent bits */
#define PBITS 4            /* number of bits needed to encode precision */
#define NBMASK 0xaaaau     /* negabinary mask */
#define TCMASK 0x7fffu     /* two's complement mask */
#define SCALAR_MIN 6.10351562e-05f /* smallest positive normal number */

static inline uint16 frexp_fp16(uint16 arg, int *exp)
{
  *exp = 0;
  unsigned int abs = arg & 0x7FFF;
  if(abs >= 0x7C00 || !abs)
    return (abs>0x7C00) ? (arg |  0x200) : arg;
  for(; abs<0x400; abs<<=1,--*exp) ;
  *exp += (abs>>10) - 14;
  return (arg&0x8000)|0x3800|(abs&0x3FF);
}
static inline uint16 ldexp_fp16(uint16 arg, long exp)
{
  unsigned int abs = arg & 0x7FFF, sign = arg & 0x8000;
  if(abs >= 0x7C00 || !abs)
    return (abs>0x7C00) ? (arg |  0x200) : arg;
  for(; abs<0x400; abs<<=1,--exp) ;
  exp += abs >> 10;
  if(exp > 30)
    return (sign|0x7C00);
  else if(exp < -10)
    return sign;
  else if(exp > 0)
    return sign|(exp<<10)|(abs&0x3FF);
  unsigned int m = (abs&0x3FF) | 0x400;
  return sign|(m>>(1-exp));
}


#define FABS(x)     ((x)&0x7FFF)
#define FREXP(x, e) frexp_fp16(x, e)
#define LDEXP(x, e) ldexp_fp16(x, e)
