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

volatile unsigned long second_inter_time = 0;
unsigned unsigned long prev_second_inter_time = 0;
float time_delta_second = 0.;

volatile long difference = 0;

volatile long delta = 0;


void setup()
{
	Serial.begin(9600);
	attachInterrupt(1, OnFirstInterruption, RISING);
	attachInterrupt(0, OnSecondInterruption, RISING);
}

void loop()
{
	if (time_delta_second = second_inter_time - prev_second_inter_time)
	{
		if (first_inter_time > prev_second_inter_time)
		{
			delta = second_inter_time - first_inter_time;
			Serial.println((String)difference + "\t" + (String)delta + "\t" + (String)time_delta_second + "\t" + (String)(90 * (difference - delta / time_delta_second)));

		}
		prev_second_inter_time = second_inter_time;
	}
}

void OnFirstInterruption()
{
	++difference;
	first_inter_time = micros();
}

void OnSecondInterruption()
{
	--difference;
	second_inter_time = micros();
}