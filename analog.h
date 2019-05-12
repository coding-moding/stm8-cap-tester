#include <stdint.h>

void AnalogInit(uint8_t Channel);
uint16_t AnalogRead(); //just adding analog input to pin C4

typedef struct
{
  void (*Init)(uint8_t Channel);
  uint16_t (*Read)(void);
} TAnalog;

static TAnalog Analog = {.Init = AnalogInit, .Read = AnalogRead};
