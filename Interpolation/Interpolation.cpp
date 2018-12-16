#include <iostream>
#include <random>
using namespace std;

struct Data { double x, y; };

double u_cal(double u, int n)
{
	double temp = u;
	for (int i = 1; i < n; i++) temp *= (u + i);
	return temp;
}

double fact(int n)
{
	double f = 1.;
	for (int i = 2; i <= n; i++) f *= i;
	return f;
}

int main()
{
	mt19937 gen(NULL);
	uniform_int_distribution<> dist(8000, 16000);

	//Количество исходных точек:
	const int TIMES_COUNT = 100;
	//Исходные значения (x,y):
	Data times[TIMES_COUNT];
	for (int i = 0; i < TIMES_COUNT; ++i)
	{
		times[i] = { double(i), double(dist(gen)) };
		cout << dist(gen) << " ";
	}

	//Таблица конечных разностей (назад)
	double y[TIMES_COUNT][TIMES_COUNT];
	for (int i = 0; i < TIMES_COUNT; i++) y[i][0] = times[i].y;
	for (int i = 1; i < TIMES_COUNT; i++)
	{
		for (int j = TIMES_COUNT - 1; j >= i; j--) y[j][i] = y[j][i - 1] - y[j - 1][i - 1];
	}
	//Вывод таблицы значений от x1 до x2 с шагом dx:
	double x1 = 0,
		x2 = 5,
		dx = 0.125;
	cout.width(20); cout << "X"; cout.width(20); cout << "Y" << endl;
	for (double x = x1; x <= x2; x += dx)
	{
		double sum = y[TIMES_COUNT - 1][0];
		double u = (x - times[TIMES_COUNT - 1].x) / (times[1].x - times[0].x);
		for (int i = 1; i < TIMES_COUNT; i++)
		{
			sum += (u_cal(u, i) * y[TIMES_COUNT - 1][i]) / fact(i);
		}
		cout.width(20); cout.precision(14); cout << x;
		cout.width(20); cout.precision(14); cout << sum << endl;
	}

	cin.get();
	system("pause");
	return 0;
}