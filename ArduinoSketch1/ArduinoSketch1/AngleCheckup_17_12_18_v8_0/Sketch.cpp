/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
/*End of auto generated code by Atmel studio */

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
//Beginning of Auto generated function prototypes by Atmel Studio
void OnFirstInterruption();
void OnSecondInterruption();
//End of Auto generated function prototypes by Atmel Studio

const long INTERVAL = 500; // интервал обновления значений

volatile unsigned long first_inter_time = 0;
unsigned long prev_first_inter_time = 0;
unsigned long time_delta_first = 0;
float aver_first_inter_time = 0;
volatile unsigned long first_inter_count = 0;
float first_delta_angle = 0;

volatile unsigned long second_inter_time = 0;
unsigned long prev_second_inter_time = 0;
unsigned long time_delta_second = 0;
float aver_second_inter_time = 0;
volatile unsigned long second_inter_count = 0;
float second_delta_angle = 0;

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
	// Будем считать изменения угла за 1 секунду (или другой интервал времени) 
	// отдельно для первого и второго прерывания,
	// а потом вычислять их разницу.

	if ((time_delta_first = first_inter_time - prev_first_inter_time) && (time_delta_second = second_inter_time - prev_second_inter_time))
	{
		prev_first_inter_time = first_inter_time;
		prev_second_inter_time = second_inter_time;

		aver_first_inter_time = float(time_delta_first) / float(first_inter_count);  // время прохода угла в 90 градусов
		Serial.print(first_inter_count);
		first_inter_count = 0;
		Serial.print("\t\t");
		first_delta_angle = 90l * time_delta_first / aver_first_inter_time;
		//Serial.print(first_delta_angle);
		//Serial.print("\t\t");


		aver_second_inter_time = float(time_delta_second) / float(second_inter_count);
		Serial.print(second_inter_count);
		second_inter_count = 0;
		Serial.print("\t\t");
		second_delta_angle = 90l * time_delta_first / aver_second_inter_time;
		//Serial.print(second_delta_angle);
		//Serial.print("\t\t");

		angle += first_delta_angle - second_delta_angle;

		// Отладочный вывод
		Serial.println((String)time_delta_first + "\t\t" + (String)time_delta_second + "\t\t" + (String)angle);
	}
}

void OnFirstInterruption()
{
	first_inter_count++;
	first_inter_time = micros();
}

void OnSecondInterruption()
{
	second_inter_count++;
	second_inter_time = micros();
}