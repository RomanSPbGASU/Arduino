#include <iostream>
#include <random.h>



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