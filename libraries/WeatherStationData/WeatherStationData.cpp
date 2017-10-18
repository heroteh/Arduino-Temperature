/*
  WeatherStationData.h - Library to contain data for my Weather Stations.
  Created by Tony Hills, September 28, 2017.
*/

#include "WeatherStationData.h"

WeatherStationData::WeatherStationData() 
{
	temperature = 0;
	humidity = 0;
	rawlight = 0;
	lightPercent = 0;
	battery = 0;
	battery_present = 0;
	sensors = 0;
	// Currently everything should have temperature, humidity and light
	sensors = sensors | (TEMPERATURE_SENSOR + HUMIDITY_SENSOR + LIGHT_SENSOR);
}

// This calls the no argument constructor
WeatherStationData::WeatherStationData(int battery_present) : WeatherStationData()
{
	if(battery_present)
	{
		sensors = sensors | BATTERY_SENSOR;
	}
}

void WeatherStationData::calculateLightPercent(float fullLight)
{
	lightPercent = rawlight / fullLight * 100;
}

float WeatherStationData::LightPercent()
{
	return lightPercent;
}

float WeatherStationData::Humidity()
{
	return humidity;
}

void WeatherStationData::Humidity(float humidity)
{
	this->humidity = humidity;
}

float WeatherStationData::Temperature()
{
	return temperature;
}

void WeatherStationData::Temperature(float temperature)
{
	this->temperature = temperature;
}

float WeatherStationData::Rawlight()
{
	return rawlight;
}

void WeatherStationData::Rawlight(float rawlight)
{
	this->rawlight = rawlight;
}

void WeatherStationData::Battery(float battery)
{
	this->battery = battery;
}

float WeatherStationData::Battery()
{
	return battery;
}

bool WeatherStationData::Battery_present()
{
	return (sensors & BATTERY_SENSOR) ? true : false;
}

int WeatherStationData::numberOfSensors()
{
	return std::bitset<8>(sensors).count();
}

String WeatherStationData::getCurrentSensor(int currentSensor)
{
	for(int count = 0, sensor = 0;count <= currentSensor;++count)
	{
		while(!(0x1 & (sensors << sensor)))
		{
			++sensor;
		}
	}
}

