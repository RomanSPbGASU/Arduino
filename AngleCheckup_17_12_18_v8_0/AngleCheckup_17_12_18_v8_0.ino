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

/*
Программа предназначена для следующих параметров:
  скорость вращения - 1 оборот в секунду;
  4 датчика, расположенных под углом в 90 градусов
*/

unsigned long inter_time = 0; // время последнего прерывания
long theory_time = 0; // время, равное времени последнего прерывания, для диска вращающегося со скоростью 1 об./мин.
unsigned long first_inter_time = 0;
unsigned long prev_fit = 0;
unsigned long inter_count = 0;  // количество прерываний
unsigned long first_inter_count = 0;
float angle = 0;
float rps = 0;
//unsigned long theory_angle = 0; // угол поворота вала
//unsigned long delta_angle = 0;  // угол диска, относительно вала


void setup()
{
	Serial.begin(9600);
	tft.initR(INITR_BLACKTAB);
	tft.fillScreen(ST7735_BLACK);
	tft.setRotation(1);
	tft.setTextColor(ST7735_BLUE);
	tft.setTextSize(2);
	attachInterrupt(0, OnFirstInterruption, RISING);
	attachInterrupt(1, OnSecondInterruption, RISING);
	Serial.println("Time\t\tAngle\tSecond\t|\tFirst\tSpeed\tTime first");  // отладочный вывод
}

void loop()
{
	theory_time = inter_count * 250000;

	rps = 250000. / (float)(first_inter_time - prev_fit);

	angle = (inter_time - theory_time) * 0.00036; // 0.00036 = 4 ср./с * 90 гр. / 10^6 мкс

	tft.fillScreen(ST7735_BLACK);

	tft.setCursor(0, 20);
	tft.print(inter_time);

	tft.setCursor(0, 50);
	tft.print(angle);

	// Отладочный вывод
	Serial.println((String)inter_time + "\t" + (String)angle + "\t" + (String)inter_count + "\t|\t" + (String)first_inter_count + "\t" + (String)rps + "\t" + (String)first_inter_time);
	//
	delay(1000);
}

void OnFirstInterruption()
{
	prev_fit = first_inter_time;
	first_inter_time = micros();
	first_inter_count++;
}

void OnSecondInterruption()
{
	inter_time = micros();
	inter_count++;
}
