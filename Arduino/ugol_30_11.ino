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
int s = 60;
int w = 6;

volatile unsigned long Time1=0;        //Время срабатывания первого датчика
volatile unsigned long Time2=0;        //Время срабатывания второго
long value = 0;                        //время пролета пули между датчиками
boolean flag=false;
void setup(){
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  attachInterrupt(0, Start, RISING);   //Прерывание по нарастающему фронту на D0
  attachInterrupt(1, End, RISING);     //Прерывание по нарастающему фронту на D1
}

void loop(){
  if (flag==true){
    value = (Time2-Time1);             //вычисляем задержку
    //value = (value/100000) * s * w;             //вычисляем скорость для расстояния между датчиками 5см
    
    Serial.println(Time2-Time1);
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 50);
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(3);
    tft.print(((Time2-Time1) / 1000000.0000) * 360.0000, 4);
    delay(1000);
    flag=false;
  }
}

void Start(){
  Time1=micros();
}
void End(){
  Time2=micros();
  flag=true;
}
