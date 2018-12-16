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

volatile unsigned long time1 = 1;         // Время срабатывания первого датчика
volatile unsigned long time1_prev = 0;    // Время предыдущего срабатывания первого датчика
volatile unsigned long time2 = 0;         // Время срабатывания второго датчика
volatile int complete_difference = 0;     // Разница в количестве срабатываний датчика каждого диска
float current = 0;                        // Текущее значение угла

long average_angle = 0;          // Среднее значение угла за последнюю минуту
const int num_val_for_averaging = 36; // Количество прерываний на D0 для вычисления скользящего среднего
float angles[num_val_for_averaging];
int index = 0;
int total = 0;

boolean first_flag = false;
boolean second_flag = false;

void setup()
{
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);

  for(int i = 0; i < num_val_for_averaging; i++)
  {
    angles[i] = 0;
  }
  
  attachInterrupt(0, OnFirstInterruption, RISING);      //Прерывание по нарастающему фронту на D0
  attachInterrupt(1, OnSecondInterruption, RISING);     //Прерывание по нарастающему фронту на D1
}

void loop()
{
  // Флаги устанавливаются таким образом, чтобы расчёт происходил после 
  // последовательного прерывания сначала на D1, а затем на D0
  // Условие time1 >= time1_prev нужно, чтобы при обнулении таймера не происходил некорректный расчёт
  if (first_flag && second_flag && time1 >= time1_prev)
  {
    current = (complete_difference + float(time2 - time1_prev)/float(time1 - time1_prev)) * 10.0;
    
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_BLUE);
    tft.setTextSize(2);

    total -= angles[index];
    angles[index] = current;
    total += current;
    index++;
    if(index >= num_val_for_averaging)
    {
     index = 0; 
    }
    average_angle = total / num_val_for_averaging;
    tft.setCursor(0, 20);
    tft.print(average_angle, 6);


    tft.setCursor(0, 50);
    tft.print(current, 6);

    delay(500);

    first_flag = false;
    second_flag = false;
  }
}

void OnFirstInterruption()
{
  time1_prev = time1;
  time1 = micros();
  complete_difference++;
  first_flag = true;
}

void OnSecondInterruption()
{
  time2 = micros();
  complete_difference--;
  first_flag = false;
  second_flag = true;
}
