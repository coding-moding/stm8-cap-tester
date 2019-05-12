#pragma once
#include <stdint.h>

static char Num[11];

void SerialInit();
int SerialWrite(const char *str);
void SerialWriteInt(const uint32_t Value);
void SerialWriteFloat(const float Value);

typedef struct
{
  void (*Init)();
  int (*Write)(const char *str);
  void (*WriteInt)(const uint32_t Value);
  void (*WriteFloat)(const float Value);
} TSerial;

static TSerial Serial =
{
  .Init = SerialInit,
  .Write = SerialWrite,
  .WriteInt = SerialWriteInt,
  .WriteFloat = SerialWriteFloat
};
