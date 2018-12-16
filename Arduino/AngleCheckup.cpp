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

#define U_LONG_MAX 4294967295

#define TFT_CS     10
#define TFT_RST    12  // you can also connect this to the Arduino reset
					  // in which case, set this #define pin to 0!
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const unsigned int NUM_VAL_FOR_AVERAGING = 36;     // Количество прерываний на D0 для вычисления скользящего среднего
const unsigned long INTERVAL = 1000;               // Интервал обновления значений в миллисекундах без учёта времени выполнения программы

unsigned long time1 = 0;        // Время срабатывания первого датчика
unsigned long time1_prev = 0;   // Время предыдущего срабатывания первого датчика
unsigned long time2 = 0;        // Время срабатывания второго датчика

int complete_difference = 0;    // Разница в количестве срабатываний датчика каждого диска
float current = 0;              // Текущее значение угла

float angles[NUM_VAL_FOR_AVERAGING] = {};
int index = 0;
int total = 0;

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
	// Условие time1 > time1_prev + 24 нужно, чтобы фильтровать ошибочные срабатывания датчика
	if (time1 > time1_prev + 24)
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
		Serial.println("time1: " + (String)time1 + " time1_prev: " + (String)time1_prev + " time2: " + (String)time2);
		Serial.println("delta2: " + (String)(time2 - time1_prev) + " delta1: " + (String)(time1 - time1_prev));
		Serial.println("fraction: " + (String)(float(time2 - time1_prev) / (time1 - time1_prev)));
		Serial.println();
		//

		delay(INTERVAL);
	}

	void OnFirstInterruption()
	{
		if (time1 > time1_prev + 24)
		{
			time1_prev = time1;
			time1 = micros();
			complete_difference++;

			if (flag)
			{
				current = (complete_difference + float(time2 - time1_prev) / (time1 - time1_prev)) * 10;

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
			time2 = micros();
			flag = true;
		}