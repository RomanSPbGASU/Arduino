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

#define TFT_CS     10
#define TFT_RST    12  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

volatile unsigned long time1 = 0;        // Время срабатывания первого датчика
volatile unsigned long time1_prev = -0.027;   // Время предыдущего срабатывания первого датчика
volatile unsigned long time2 = 0;        // Время срабатывания второго
volatile int complete_difference = 0;    // Разница в количестве срабатываний между дисками

boolean flag = false;

void setup()
{
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  
  attachInterrupt(0, OnFirstFlip, RISING);      //Прерывание по нарастающему фронту на D0
  attachInterrupt(1, OnSecondFlip, RISING);     //Прерывание по нарастающему фронту на D1
}

void loop()
{
  if (flag == true)
  {
    round1_delta = time1 - time1_prev;            // Время между соседними срабатываниями датчика на первом диске
    rounds_delta = time2 - time1_prev;            // Время между срабатыванием датчика на первом и на втором устройстве
    
    //Serial.println(time2 - time1);
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 50);
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(3);

    tft.print((complete_difference + (time2 - time1_prev)/(time1 - time1_prev)) * 10., 4)

    flag = false;
  }
}

void OnFirstFlip()
{
  time1_prev = time1;
  time1 = micros();
  complete_difference++;
  flag = true;
}
void OnSecondFlip()
{
  time2 = micros();
  complete_difference--;
  flag = true;
}
