/*
  WeatherStationDeviceDHT.h - Library to contain code to retreive and print measurements for my Weather Station with a DHT22 device.
  Created by Tony Hills, September 28, 2017.
*/
#ifndef WeatherStationDeviceDHT_h
#define WeatherStationDeviceDHT_n

#include "Arduino.h"
#include <DHT.h>
#include <WeatherStationData.h>

class WeatherStationDeviceDHT
{
public:
	WeatherStationDeviceDHT(int pin, WeatherStationData *data);
	void begin();
	void readHumidity();
	void readTemperature();
private:
	DHT *dht;
	WeatherStationData *data;
};

#endif