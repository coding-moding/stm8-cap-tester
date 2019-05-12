#pragma once
#include "stm8.h"


/* Simple busy loop delay */
static void delay(unsigned long count)
{
  while(count--)
    nop();
}
