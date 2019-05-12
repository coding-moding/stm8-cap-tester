#include <stdint.h>
typedef uint8_t Bool;
#define True 1
#define False 0

//static void reverse(char str[], int length);
unsigned char IntToStr(uint32_t Value, char *buf);
#define _FTOA_TOO_LARGE 1
#define _FTOA_TOO_SMALL 2
unsigned char FloatToStr(float f, char *Buf);
