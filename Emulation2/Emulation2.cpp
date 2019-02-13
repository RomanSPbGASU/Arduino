#include <iostream>

int main()
{
	volatile unsigned long first_inter_time = 0;

	volatile unsigned long second_inter_time = 0;
	unsigned long prev_second_inter_time = 0;
	float time_delta_second = 0.;

	float fraction = 0.;

	volatile long count = 0;
	volatile long difference = 0;

	volatile unsigned long delta = 0;

	while(1)
		time_delta_second = second_inter_time - prev_second_inter_time;

		if (time_delta_second)
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
		--difference;
		first_inter_time = micros();
	}

	void OnSecondInterruption()
	{
		++difference;
		//++count;
		second_inter_time = micros();
	}
}
