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

volatile unsigned long first_inter_time = 0;
volatile unsigned long prev_first_inter_time = 0;
volatile float time_delta_first = 0.;
volatile unsigned int first_inter_count = 0;

volatile unsigned long second_inter_time = 0;
volatile unsigned long prev_second_inter_time = 0;
volatile float time_delta_second = 0.;
float aver_second_inter_time = 0;
volatile unsigned int second_inter_count = 0;
float second_delta = 0;

float angle = 0;

void setup()
{
	Serial.begin(9600);
	attachInterrupt(1, OnFirstInterruption, RISING);
	attachInterrupt(0, OnSecondInterruption, RISING);
	Serial.println("Count_first\tCount_second\tDelta_first\tDelta_second\tAngle");  // отладочный вывод
}

void loop()
{
	time_delta_first = first_inter_time - prev_first_inter_time;
	time_delta_second = second_inter_time - prev_second_inter_time;
	if ((time_delta_first > 0) && (time_delta_second > 0))
	{
		if (!(time_delta_second == 196608 || time_delta_second == 262144 || time_delta_second == 16777216) &&
			!(time_delta_first == 196608 || time_delta_first == 262144 || time_delta_first == 16777216))
		{
			prev_first_inter_time = first_inter_time;
			prev_second_inter_time = second_inter_time;

			Serial.print((String)first_inter_count + "\t\t");
			first_inter_count = 0;

			Serial.print((String)second_inter_count + "\t\t");
			second_inter_count = 0;
			second_delta = time_delta_first / time_delta_second * second_inter_count;

			dif += first_inter_count - second_delta;

			// Отладочный вывод
			Serial.println((String)time_delta_first + "\t\t" + (String)time_delta_second + "\t\t" + (String)(90 * dif));
		}
		else
			Serial.println("Invalid value");
	}
}

void OnFirstInterruption()
{
	++first_inter_count;
	first_inter_time = micros();
}

void OnSecondInterruption()
{
	++second_inter_count;
	second_inter_time = micros();
}