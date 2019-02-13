#include <iostream>
#include <Windows.h>
#include <fstream>

const long INTERVAL = 500; // интервал обновления значений

float time_delta_first = 0.;
unsigned long first_inter_count = 0;

float time_delta_second = 0;
unsigned long second_inter_count = 0;

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

			input >> garbage >> time_delta_first >> time_delta_second >> garbage;
			first_inter_count++;
			second_inter_count++;
			output << second_inter_count << "\t\t";

			dif += time_delta_first / time_delta_second;

			// Отладочный вывод
			output << time_delta_first << "\t\t" << time_delta_second << "\t\t" << (90 * first_inter_count - dif * second_inter_count) << std::endl;

		}
	}
}