/*
  WeatherStationOLED.h - Library to contain OLED stuff for my Weather Stations.
  Created by Tony Hills, October 3, 2017.
*/
#ifndef WeatherStationOLED_h
#define WeatherStationOLED_h

#include "Arduino.h"
#include <Adafruit_SSD1306.h>
#include <WeatherStationData.h>

#define OLED_RESET -1

class WeatherStationOLED
{
public:
	WeatherStationOLED();
	WeatherStationOLED(boolean off_at_night);
	void displayData(WeatherStationData *data, String currentStarSensor);
	void displayOff();
	void displayInit();
	int Off_at_night();
	void setSensors();
private:
	Adafruit_SSD1306 *display;
	int off_at_night;
};

#endif