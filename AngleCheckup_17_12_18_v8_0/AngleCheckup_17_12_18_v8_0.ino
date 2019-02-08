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

#define TFT_CS     10
#define TFT_RST    12  // you can also connect this to the Arduino reset
  // in which case, set this #define pin to 0!
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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

// можно попробовать оба варианта функции
//unsigned long micros() {
//	unsigned long m;
//	uint8_t oldSREG = SREG, t;
//
//	cli();
//	m = timer0_overflow_count;
//#if defined(TCNT0)
//	t = TCNT0;
//#elif defined(TCNT0L)
//	t = TCNT0L;
//#else
//#error TIMER 0 not defined
//#endif
//
//
//#ifdef TIFR0
//	if ((TIFR0& _BV(TOV0)) && (t & 255))
//		m++;
//#else
//	if ((TIFR& _BV(TOV0)) && (t & 255))
//		m++;
//#endif
//
//	SREG = oldSREG;
//
//	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
//}

static unsigned long micros() {
	extern volatile unsigned long timer0_overflow_count;
	uint8_t oldSREG = SREG;
	cli();
	uint32_t t = TCNT0;
	if ((TIFR0& _BV(TOV0)) && (t == 0))
		t = 256;
	uint32_t m = timer0_overflow_count;
	SREG = oldSREG;
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

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
	Serial.println("Count_first\tCount_second\tDelta_first\tDelta_second\tAngle");  // отладочный вывод
}

void loop()
{
	time_delta_first = first_inter_time - prev_first_inter_time;
	time_delta_second = second_inter_time - prev_second_inter_time;
	if ((time_delta_first > 0) && (time_delta_second > 0) && !(time_delta_second == 196608 || time_delta_second == 262144 || time_delta_second == 16777216))
	{
		prev_first_inter_time = first_inter_time;
		prev_second_inter_time = second_inter_time;

		aver_first_inter_time = float(time_delta_first) / float(first_inter_count);  // время прохода угла в 90 градусов
		Serial.print(first_inter_count);
		first_inter_count = 0;
		Serial.print("\t\t");
		first_delta_angle = 90l * time_delta_first / aver_first_inter_time;

		aver_second_inter_time = float(time_delta_second) / float(second_inter_count);
		Serial.print(second_inter_count);
		second_inter_count = 0;
		Serial.print("\t\t");
		second_delta_angle = 90l * time_delta_first / aver_second_inter_time;

		angle += first_delta_angle - second_delta_angle;

		// Отладочный вывод
		Serial.println((String)time_delta_first + "\t\t" + (String)time_delta_second + "\t\t" + (String)angle);
	}
}

void OnFirstInterruption()
{
	++first_inter_count;
	first_inter_time = micros();
}

void OnSecondInterruption()
{
	++second_inter_count;
	second_inter_time = micros();
}