#include <iostream>

using namespace std;

int main()
{
	int a, b, c;
	if ((a = 5) + (b = 10) == 15)
	{
		cout << "It's working!" << endl;
	}
	system("pause");
}





//#include <iostream>
//#include <random>
//using namespace std;
//
//int main()
//{
//	//random_device rd;
//	//mt19937_64 gen(rd());
//	//for (int i = 0; i < 5; ++i)
//	//{
//	//	cout << gen() << " ";
//	//}
//
//	//random_device rd;
//	mt19937 gen(NULL);
//	uniform_int_distribution<> dist(8000, 16000);
//	const int TIMES_COUNT = 10000;
//	int times[TIMES_COUNT];
//	for (int i = 0; i < TIMES_COUNT; ++i)
//	{
//		times[i] = dist(gen);
//	}
//	int delta = 10;
//
//
//
//	system("pause");
//}
