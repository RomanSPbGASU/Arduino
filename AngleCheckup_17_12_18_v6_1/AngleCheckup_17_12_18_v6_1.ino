//Wiring:
//TFT display -> Arduino Nano
//VCC -> 5V
//GND -> GND
//CS -> D10
//RESET -> D12
//A0 -> D8
//SDA -> D11
//SCK -> D13
//LED -> D9
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(10, 8, 12);

int complete_difference = 0;    // Разница в количестве срабатываний датчика каждого диска

void setup()
{
  Serial.begin(9600);

  attachInterrupt(0, OnFirstInterruption, RISING);
  attachInterrupt(1, OnSecondInterruption, RISING);
}

void loop()
{
  if (!(millis() % 1000))
  {
    // Отладочный вывод
    Serial.println("Difference: " + (String)complete_difference);
    //
  }
}

void OnFirstInterruption()
{
  complete_difference++;
}

void OnSecondInterruption()
{
  complete_difference--;
}
