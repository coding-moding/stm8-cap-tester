#include "util.h"
#include "main.h"

typedef enum {In = 0, Out} TPinDirection;
enum {Float = 0, PullUp};
enum {OpenDrain = 0, PushPull};
//void SetPin(unsigned char *PinGroup, unsigned char Pin)
//{
//  *(PinGroup + PA_DDR - PA) |= Pin;
//  PC_DDR |= PIN5;
//  PC_CR1 |= PIN5;
//  PC_ODR |= Pin;
//}
