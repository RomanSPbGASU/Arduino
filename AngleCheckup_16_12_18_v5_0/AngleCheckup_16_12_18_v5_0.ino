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

const unsigned int NUM_VAL_FOR_AVERAGING = 36;     // Количество прерываний на D0 для вычисления скользящего среднего
const unsigned long INTERVAL = 1000;               // Интервал обновления значений в миллисекундах без учёта времени выполнения программы

int complete_difference = 0;    // Разница в количестве срабатываний датчика каждого диска
float current = 0;              // Текущее значение угла

float angles[NUM_VAL_FOR_AVERAGING] = {};
int index = 0;
int total = 0;

unsigned int times1[5] = {0};
byte i1 = 0;
unsigned int times2[5] = {0};
byte i2 = 0;
double start_angle = 0;

boolean flag = false;

void setup()
{
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(2);

  attachInterrupt(0, OnFirstForStartAngle, RISING);
  attachInterrupt(1, OnSecondForStartAngle, RISING);
}

void OnFirstForStartAngle()
{
  if (i1 <= 2)
  {
    times1[i1++] = micros();   
  }
  calcStartAngle();
}

void OnSecondForStartAngle()
{
  if (i2 <= 2)
  {
    times2[i2++] = micros();
  }
  calcStartAngle();
}

void calcStartAngle()
{
  if (i1 = 2 && i2 = 2)
  {
    detachInterrupt(0, OnFirstForStartAngle, RISING);
    detachInterrupt(1, OnSecondForStartAngle, RISING);
    
    // Приближаем график функции изменения угла x(t) параболой вида x = at^2 + bt + c
    // В момент времени t = 0 угол x = c

    start_angle = interpAngle(0);
    
    attachInterrupt(0, OnFirstInterruption, RISING);      //Прерывание по нарастающему фронту на D0
    attachInterrupt(1, OnSecondInterruption, RISING);     //Прерывание по нарастающему фронту на D1
  }
}

// функция для нахождения угла в момент времени t
double interpAngle(unsigned long t)
{
    double a = 20. / ((time1[2] - time1[0]) * (time1[2] - time1[1])) - 10. / ((time1[1] - time1[0]) * (time1[2] - time1[1]));
    double b = 10. / (time1[1] - time1[0]) - a * (time1[1] + time1[0]);
    double c = -b * time1[0] - a * time1[0] * time1[0];
    double angle1 = c + b * t + a * t * t;
  
    a = 20. / ((time2[2] - time2[0]) * (time2[2] - time2[1])) - 10. / ((time2[1] - time2[0]) * (time2[2] - time2[1]));
    b = 10. / (time2[1] - time2[0]) - a * (time2[1] + time2[0]);
    c = -b * time2[0] - a * time2[0] * time2[0];
    double angle2 = c + b * t + a * t * t;

    return angle1 - angle2;  // угол опережения первым диском второго
}

void loop()
{
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(0, 20);
  tft.print(total / NUM_VAL_FOR_AVERAGING, 2);
  
  tft.setCursor(0, 50);
  tft.print(current, 2);

  // Отладочный вывод
  Serial.println("Difference: " + (String)complete_difference);
  Serial.println();
  Serial.println("Current angle: " + (String)current);
  Serial.println("Average angle: " + (String)total / NUM_VAL_FOR_AVERAGING);
  Serial.println();
  //
  
  delay(INTERVAL);
}

void OnFirstInterruption()
{
  complete_difference++;

  
  
  if(flag)
  {
    current = complete_difference * 10 + 5;

    total -= angles[index];
    angles[index] = current;
    total += current;
    index++;
    index %= NUM_VAL_FOR_AVERAGING;
    
    flag = false;
  }
}

void OnSecondInterruption()
{
    complete_difference--;
    
    flag = true;
}
