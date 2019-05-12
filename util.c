#include "util.h"

void reverse(char str[], int length)
{
  int start = 0;
  int end = length - 1;
  while(start < end)
  {
    char t;
    t = *(str + start);
    *(str + start) = *(str + end);
    *(str + end) = t;
    start++;
    end--;
  }
}

unsigned char IntToStr(uint32_t Value, char *buf)
{
  unsigned char i = 0;
  if(!Value)
  {
    buf[0] = '0';
    buf[1] = 0;
    return 1;
  }
  while(Value)
  {
    buf[i] = '0' + Value % 10;
    i++;
    Value /= 10;
  }
  reverse(buf, i);
  buf[i] = 0;
  return i;
}

//union
//{
//  float f;
//  struct
//  {
//    unsigned int    mantissa_lo : 16;
//    unsigned int    mantissa_hi : 7;
//    unsigned int     exponent : 8;
//    unsigned int     sign : 1;
//  };
//} helper;
unsigned char FloatToStr(float f, char *Buf)
{
  long mantissa, int_part, frac_part;
  short exp2;
  typedef union
  {
    long L;
    float F;
  } TLF;
  TLF x;
  char *p;
  if(f == 0.0f)
  {
    Buf[0] = '0';
    Buf[1] = '.';
    Buf[2] = '0';
    Buf[3] = 0;
    return 0;
  }
  x.F = f;
  exp2 = (unsigned char)(x.L >> 23) - 127;
  mantissa = (x.L & 0xFFFFFF) | 0x800000;
  frac_part = 0;
  int_part = 0;
  if(exp2 >= 31)
    return _FTOA_TOO_LARGE;
  else if(exp2 < -23)
    return _FTOA_TOO_SMALL;
  else if(exp2 >= 23)
    int_part = mantissa << (exp2 - 23);
  else if(exp2 >= 0)
  {
    int_part = mantissa >> (23 - exp2);
    frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
  }
  else /* if (exp2 < 0) */
    frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);
  p = Buf;
  if(x.L < 0)
    *p++ = '-';
  if(int_part == 0)
    *p++ = '0';
  else
  {
    //    ltoa(p, int_part, 10);
    IntToStr((uint32_t)int_part, p);
    while(*p)
      p++;
  }
  *p++ = '.';
  if(frac_part == 0)
    *p++ = '0';
  else
  {
    unsigned char m, max;
    max = sizeof(Buf) - (unsigned char)(p - Buf) - 1;
    if(max > 7) max = 7;
    for(m = 0; m < max; m++)    // print BCD
    {
      frac_part = (frac_part << 3) + (frac_part << 1);      // frac_part *= 10;
      *p++ = (frac_part >> 24) + '0';
      frac_part &= 0xFFFFFF;
    }
    for(p--; p[0] == '0' && p[-1] != '.'; --p);     // delete ending zeroes
    p++;
  }
  *p = 0;
  return 0;
}
