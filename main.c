#include "main.h"
#include "util.h"
#include "analog.h"
#include "serial.h"
#include <stdint.h>

/* Build in LED is in pin B5 (STM8S103 board) or D3 (STM8S003F3 board) */
#define LED_PORT    PB
#define LED_PIN     PIN5

//  Setup the system clock to run at 16MHz using the internal oscillator.
void InitialiseSystemClock()
{
  CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
  CLK_ICKR |= CLK_ICKR_HSIEN;                 //  Enable the HSI.
  CLK_ECKR = 0;                       //  Disable the external clock.
  while (!(CLK_ICKR & CLK_ICKR_HSIRDY));       //  Wait for the HSI to be ready for use.
  CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
  CLK_PCKENR1 = 0xff;                 //  Enable all peripheral clocks.
  CLK_PCKENR2 = 0xff;                 //  Ditto.
  CLK_CCOR = 0;                       //  Turn off CCO.
  CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
  CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
  CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
  CLK_SWCR = 0;                       //  Reset the clock switch control register.
  CLK_SWCR |= CLK_SWCR_SWEN;                  //  Enable switching.
  while (CLK_SWCR & CLK_SWCR_SWBSY);        //  Pause while the clock switch is busy.
}

void SetPCFloat(unsigned char Pins)
{
  PC_DDR &= ~Pins;
  PC_CR1 &= ~Pins;
}

void SetPCHigh(uint8_t Pins)
{
  PC_DDR |= Pins;
  PC_CR1 |= Pins;
  PC_ODR |= Pins;
}

void SetPCLow(uint8_t Pins)
{
  PC_DDR |= Pins;
  PC_ODR &= ~Pins;
  PC_CR1 &= ~Pins;
}

//PC4 - ADC. 150
//PC5 - 10k
//PC6 - 100k
//PC7 - 750
#define PIN_ADC PC4
#define PIN_FAST_CHARGE PIN5
#define PIN_SLOW_CHARGE PIN6
#define PIN_FAST_DISCHARGE PIN7
static Bool FastCharge = True;
static volatile uint32_t MeasureTickCount = 0;
float Test()
{
  const uint16_t ChargeResistors[2] = {100, 10}; // KOhm
//  CLK_PCKENR1 = 0;  // Disable all Peripheral Clocks - at start
//  CLK_PCKENR2 = 0;
  Analog.Init(2);
  SetPCFloat(PIN_FAST_CHARGE | PIN_SLOW_CHARGE | PIN_FAST_DISCHARGE);
  if(FastCharge)
    SetPCHigh(PIN_FAST_CHARGE);
  else
    SetPCHigh(PIN_SLOW_CHARGE);
  static const int LinearChargeTimeCoeff = 648;
  while(Analog.Read() < LinearChargeTimeCoeff) MeasureTickCount++;
  float pF = ((float)MeasureTickCount * 7700 / ChargeResistors[FastCharge]);
  FastCharge = !MeasureTickCount || MeasureTickCount > 1000;
  SetPCFloat(PIN_FAST_CHARGE | PIN_SLOW_CHARGE);
  SetPCLow(PIN_FAST_DISCHARGE);
  while(Analog.Read() > 0);
  SetPCFloat(PIN7);
  return pF;
}

int main()
{
  int i;
  //  InitialiseSystemClock();
  CLK_CKDIVR = 0; // Set clock to full speed (16 Mhz)
  Serial.Init();
  Serial.Write("Started\r\n");
  //   GPIO setup
  PORT(LED_PORT, DDR) |= LED_PIN; // PB_DDR |= (1 << 5); // Set pin data direction as output
  PORT(LED_PORT, CR1) |= LED_PIN; // PB_CR1 |= (1 << 5); // Set pin as "Push-pull"
  for(i = 0; i < 3; i++)
  {
    PORT(LED_PORT, ODR) &= ~LED_PIN; // PB_ODR &= ~(1 << 5);
    delay(100000L);
    PORT(LED_PORT, ODR) |= LED_PIN; // PB_ODR |= (1 << 5);
    delay(100000L);
  }
  float pFAverage = 0;
  const uint32_t OutputPeriod = 40000;
  uint32_t OutputTickCount = 0;
  while(1)
  {
    float pF;
    pF = Test();
    const uint16_t IterationTickCount = 5000;
    OutputTickCount += MeasureTickCount + IterationTickCount;
    MeasureTickCount = 0;
    if(pF != 0.0f)
    {
      float MeasureWeight = 0.9f;
      pFAverage = pF * MeasureWeight + pFAverage * (1 - MeasureWeight);
    }
    else
    {
      pFAverage = 0;
      OutputTickCount = 0;
    }
    if(OutputTickCount > OutputPeriod)
    {
      OutputTickCount = 0;
      const uint8_t NextMeasureUnit = 100;
      if(pFAverage >= (float)NextMeasureUnit * 1000)
      {
        Serial.WriteFloat(pFAverage / 1000000);
        Serial.Write(" uF\r\n");
      }
      else if(pFAverage >= (float)NextMeasureUnit)
      {
        Serial.WriteFloat(pFAverage / 1000);
        Serial.Write(" nF\r\n");
      }
      else
      {
        Serial.WriteFloat(pFAverage);
        Serial.Write(" pF\r\n");
      }
    }
    PORT(LED_PORT, ODR) &= ~LED_PIN; // PB_ODR &= ~(1 << 5);
    delay(10000L);
    PORT(LED_PORT, ODR) |= LED_PIN; // PB_ODR |= (1 << 5);
  }
  return 0;
}


