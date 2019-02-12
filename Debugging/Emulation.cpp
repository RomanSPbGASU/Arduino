#include <iostream>
#include <Windows.h>
#include <fstream>

const long INTERVAL = 500; // интервал обновления значений

float time_delta_first = 0.;
unsigned long first_inter_count = 0;

float time_delta_second = 0;
unsigned long second_inter_count = 0;
float second_delta = 0;

float garbage;

float dif = 0;

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

			input >> first_inter_count >> second_inter_count >> time_delta_first >> time_delta_second >> garbage;
			if (time_delta_first == 196608 || time_delta_first == 262144 || time_delta_first == 16777216 || 
				time_delta_second == 196608 || time_delta_second == 262144 || time_delta_second == 16777216)
				continue;

			output << second_inter_count << "\t\t";
			second_delta =  time_delta_first / time_delta_second * second_inter_count;

			dif += first_inter_count - second_delta;

			// Отладочный вывод
			output << time_delta_first << "\t\t" << time_delta_second << "\t\t" << 90 * dif << std::endl;

		}
	}
}