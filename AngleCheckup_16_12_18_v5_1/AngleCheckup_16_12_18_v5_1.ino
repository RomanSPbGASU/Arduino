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
#include <algorithm>

#define TFT_CS     10
#define TFT_RST    12  // you can also connect this to the Arduino reset
      // in which case, set this #define pin to 0!
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const unsigned int NUM_VAL_FOR_AVERAGING = 36;     // Количество прерываний на D0 для вычисления скользящего среднего
const unsigned long INTERVAL = 1000;               // Интервал обновления значений в миллисекундах без учёта времени выполнения программы

int complete_difference = 0;    // Разница в количестве срабатываний датчика каждого диска
float current = 0;              // Текущее значение угла

float angles[NUM_VAL_FOR_AVERAGING] = {};
int index = 0;
int total = 0;

unsigned long times1[3];
byte i1 = 0;
unsigned long times2[3];
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

  attachInterrupt(0, OnFirstInterruption, RISING);      //Прерывание по нарастающему фронту на D0
  attachInterrupt(1, OnSecondInterruption, RISING);     //Прерывание по нарастающему фронту на D1
}

void loop()
{
  unsigned long * ms = &millis();
  if (!(*ms % INTERVAL))
  {
    unsigned long t1[3], t2[3];
    std::copy(times1, times1 + 12, t1);
    std::copy(times2, times2 + 12, t2);
    current = complete_difference * 10 + start_angle + interpAngle(*ms, t1 , t2, i1, i2);
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
  }
}

unsigned long *ms, *t;
void OnFirstInterruption()
{
  complete_difference++;

  if ((ms = &micros()) - (t = &times1[i1++ % 3]) > 24)
    t = ms;
}

void OnSecondInterruption()
{
  complete_difference--;

  if ((ms = &micros()) - (t = &times2[i2++ % 3]) > 24)
    t = ms;
}

// функция для нахождения угла в момент времени t
double interpAngle(unsigned long t, unsigned long time1[3], unsigned long time2[3], byte i_1, byte i_2)
{
  // Приближаем график функции изменения угла x(t) параболой вида x = at^2 + bt + c
  // В момент времени t = 0 угол x = c
  byte i1_0 = (i_1 + 1) % 3;
  byte i1_1 = (i_1 + 2) % 3;
  byte i1_2 = i_1;
  double a = 20. / ((time1[i1_2] - time1[i1_0]) * (time1[i1_2] - time1[i1_1])) - 10. / ((time1[i1_1] - time1[i1_0]) * (time1[i1_2] - time1[i1_1]));
  double b = 10. / (time1[i1_1] - time1[i1_0]) - a * (time1[i1_1] + time1[i1_0]);
  double c = -b * time1[i1_0] - a * time1[i1_0] * time1[i1_0];
  double angle1 = c + b * t + a * t * t;

  byte i2_0 = (i_2 + 1) % 3;
  byte i2_1 = (i_2 + 2) % 3;
  byte i2_2 = i_2;
  a = 20. / ((time2[i2_2] - time2[i2_0]) * (time2[i2_2] - time2[i2_1])) - 10. / ((time2[i2_1] - time2[i2_0]) * (time2[i2_2] - time2[i2_1]));
  b = 10. / (time2[i2_1] - time2[i2_0]) - a * (time2[i2_1] + time2[i2_0]);
  c = -b * time2[i2_0] - a * time2[i2_0] * time2[i2_0];
  double angle2 = c + b * t + a * t * t;

  return angle1 - angle2;  // угол опережения первым диском второго
}
