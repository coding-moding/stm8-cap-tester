#include "../stm8-sdcc-examples/stm8.h"
#include "main.h"
#include "serial.h"
#include "util.h"

void SerialInit()
{
  // Setup UART1 (TX=D5)
  UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
  // UART1_CR2 |= UART_CR2_REN; // Receiver enable
  UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
  // 9600 baud: UART_DIV = 16000000/9600 ~ 1667 = 0x0683
  UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 0x0683 coded funky way (see ref manual)
}

int SerialWrite(const char *str)
{
  const char *i;
  for(i = str; *i; i++)
  {
    while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
    UART1_DR = (unsigned char)*i;
  }
  return i - str; // Bytes sent
}

void SerialWriteInt(const uint32_t Value)
{
  IntToStr(Value, Num);
  SerialWrite(Num);
}

void SerialWriteFloat(const float Value)
{
  FloatToStr(Value, Num);
  SerialWrite(Num);
}
