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
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
//#include <SPI.h>

//Adafruit_ST7735 tft = Adafruit_ST7735(10, 8, 12);

int count_first = 0;    // количество срабатываний первого
int count_second = 0;   // количество срабатываний второго

void setup()
{
  Serial.begin(9600);

  attachInterrupt(0, OnFirst, RISING);
  attachInterrupt(1, OnSecond, RISING);
}

void loop()
{
  Serial.println("1:" + (String)(count_first) + " 2:" + (String)(count_second) + " d:" + (String)(count_first - count_second));
  delay(1000);
}

void OnFirst()
{
  count_first++;
}

void OnSecond()
{
  count_second++;
}
