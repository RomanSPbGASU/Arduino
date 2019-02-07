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

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const long INTERVAL = 200; // интервал обновления значений

unsigned long first_inter_time = 0;
unsigned long prev_first_inter_time = 0;
unsigned long time_delta_first = 0;
unsigned long aver_first_inter_time = 0;
unsigned long first_inter_count = 0;
unsigned long first_delta_angle = 0;

unsigned long second_inter_time = 0;
unsigned long prev_second_inter_time = 0;
unsigned long time_delta_second = 0;
unsigned long aver_second_inter_time = 0;
unsigned long second_inter_count = 0;
unsigned long second_delta_angle = 0;

long angle = 0;


void setup()
{
	Serial.begin(9600);
	tft.initR(INITR_BLACKTAB);
	tft.fillScreen(ST7735_BLACK);
	tft.setRotation(1);
	tft.setTextColor(ST7735_BLUE);
	tft.setTextSize(2);
	attachInterrupt(1, OnFirstInterruption, RISING);
	attachInterrupt(0, OnSecondInterruption, RISING);
	Serial.println("Count_first\tCount_second\tDelta_first\tDelta_second\tAngle");  // отладочный вывод
}

void loop()
{
	// Будем считать изменения угла за 1 секунду (или другой интервал времени) 
	// отдельно для первого и второго прерывания,
	// а потом вычислять их разницу.

	time_delta_first = first_inter_time - prev_first_inter_time;
	prev_first_inter_time = first_inter_time;
	aver_first_inter_time = time_delta_first / first_inter_count;  // время прохода угла в 90 градусов
	Serial.print(first_inter_count);
	Serial.print("\t\t");
	first_inter_count = 0;
	first_delta_angle = 90l * INTERVAL * 1000l / aver_first_inter_time;

	time_delta_second = second_inter_time - prev_second_inter_time;
	prev_second_inter_time = second_inter_time;
	aver_second_inter_time = time_delta_second / second_inter_count;
	Serial.print(second_inter_count);
	Serial.print("\t\t");
	second_inter_count = 0;
	second_delta_angle = 90l * INTERVAL * 1000l / aver_second_inter_time;

	angle += long(first_delta_angle - second_delta_angle);

	// Отладочный вывод
	Serial.println((String)time_delta_first + "\t\t" + (String)time_delta_second + "\t\t" + (String)angle);
	delay(INTERVAL);
}

void OnFirstInterruption()
{
	first_inter_time = micros();
	first_inter_count++;
}

void OnSecondInterruption()
{
	second_inter_time = micros();
	second_inter_count++;
}