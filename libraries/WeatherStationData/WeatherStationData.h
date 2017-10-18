/*
  WeatherStationData.h - Library to contain data for my Weather Stations.
  Created by Tony Hills, September 28, 2017.
*/
#ifndef WeatherStationData_h
#define WeatherStationData_h

#include "Arduino.h"
#include <bitset>

#define TEMPERATURE_SENSOR 0x1
#define HUMIDITY_SENSOR 0x2
#define LIGHT_SENSOR 0x4
#define BATTERY_SENSOR 0x8

class WeatherStationData
{
public:
	WeatherStationData();
	WeatherStationData(int battery_present);
	void calculateLightPercent(float fullLight);
	float Temperature();
	void Temperature(float temperature);
	float Humidity();
	void Humidity(float humidity);
	float Rawlight();
	void Rawlight(float rawlight);
	float LightPercent();
	void Battery(float battery);
	float Battery();
	bool Battery_present();
	int numberOfSensors();
	String getCurrentSensor(int currentSensor);
private:
	float temperature;
	float humidity;
	float rawlight;
	float lightPercent;
	float battery;
	int battery_present;
	// 0x1 - temperature
	// 0x2 - humidity
	// 0x4 - light
	// 0x8 - battery
	int sensors;
};

#endif