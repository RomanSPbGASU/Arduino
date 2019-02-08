#include <iostream>
#include <Windows.h>
#include <fstream>

const long INTERVAL = 500; // интервал обновления значений

unsigned long time_delta_first = 0;
float aver_first_inter_time = 0;
unsigned long first_inter_count = 0;
float first_delta_angle = 0;

unsigned long time_delta_second = 0;
float aver_second_inter_time = 0;
unsigned long second_inter_count = 0;
float second_delta_angle = 0;

float angle = 0;

int main()
{
	std::ifstream input("Input.txt");
	std::ofstream output("Output.txt");
	if (input.is_open() && output.is_open())
	{
		while (!input.eof())
		{
			// Будем считать изменения угла за 1 секунду (или другой интервал времени) 
			// отдельно для первого и второго прерывания,
			// а потом вычислять их разницу.

			input >> first_inter_count >> time_delta_first >> second_inter_count >> time_delta_second;

			aver_first_inter_time = time_delta_first / first_inter_count;  // время прохода угла в 90 градусов
			std::cout << first_inter_count << "\t\t";
			first_delta_angle = 90l * INTERVAL * 1000l / aver_first_inter_time;

			aver_second_inter_time = time_delta_second / second_inter_count;
			std::cout << second_inter_count << "\t\t";
			second_delta_angle = 90l * INTERVAL * 1000l / aver_second_inter_time;

			angle += first_delta_angle - second_delta_angle;

			// Отладочный вывод
			std::cout << time_delta_first << "\t\t" << time_delta_second << "\t\t" << angle << std::endl;
			output << time_delta_first << "\t\t" << time_delta_second << "\t\t" << angle << std::endl;

		}
	}
}