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
#include <SPI.h>
#include <math.h>

#define TFT_CS     10
#define TFT_RST    12  // you can also connect this to the Arduino reset
  // in which case, set this #define pin to 0!
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const int INTERVAL = 1000; // интервал обновления значений - 1 секунда

unsigned long first_inter_time = 0;
unsigned long prev_first_inter_time = 0;
unsigned long time_delta_first = 0;
unsigned float aver_first_inter_time = 0;
unsigned int first_inter_count = 0;
unsigned int first_delta_angle = 0;

unsigned long second_inter_time = 0;
unsigned long prev_second_inter_time = 0;
unsigned long time_delta_second = 0;
unsigned float aver_second_inter_time = 0;
unsigned int second_inter_count = 0;
unsigned int second_delta_angle = 0;


void setup()
{
	Serial.begin(9600);
	tft.initR(INITR_BLACKTAB);
	tft.fillScreen(ST7735_BLACK);
	tft.setRotation(1);
	tft.setTextColor(ST7735_BLUE);
	tft.setTextSize(2);
	attachInterrupt(1, OnFirstInterruption, RISING);
	attachInterrupt(0, OnSecondInterruption, RISING);
	Serial.println("Time\t\tAngle\tSecond\t|\tFirst\tSpeed\tTime first");  // отладочный вывод
}

void loop()
{
	// будем считать изменения угла за 1 секунду (или другой интервал времени) 
	// отдельно для первого и второго прерывания
	// а потом вычислять их разницу
	// для контроля можно вычислять среднюю скорость и общий угловой путь для каждого диска

	time_delta_first = first_inter_time - prev_first_inter_time;
	prev_first_inter_time = first_inter_time;
	aver_first_inter_time = time_delta_first / float(first_inter_count);	// время прохода угла в 90 градусов
	first_inter_count = 0;
	first_delta_angle = 90 * INTERVAL * 1000 / aver_first_inter_time;

	time_delta_second = second_inter_time - prev_second_inter_time;
	prev_second_inter_time = second_inter_time;
	aver_second_inter_time = time_delta_second / second_inter_count;
	second_inter_count = 0;
	second_delta_angle = 90 * INTERVAL * 1000 / aver_second_inter_time;

	angle = first_delta_angle - second_delta_angle;
	Serial.println((String)time_delta_first + "\t" + (String)time_delta_second + "\t" + (String)angle)

	delay(INTERVAL);	// интервал задаётся в миллисекундах
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
