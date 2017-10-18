/*
  WeatherStationDeviceDHT.h - Library to contain code to retreive and print measurements for my Weather Station with a DHT22 device.
  Created by Tony Hills, September 28, 2017.
*/
#include "WeatherStationDeviceDHT.h"

WeatherStationDeviceDHT::WeatherStationDeviceDHT(int pin, WeatherStationData *data)
{
	dht = new DHT(pin, DHT22);
	this->data = data;
}

void WeatherStationDeviceDHT::begin()
{
	dht->begin();
	// It takes a bit...
	delay(1000);
}

void WeatherStationDeviceDHT::readHumidity()
{
	data->Humidity(dht->readHumidity());
}

void WeatherStationDeviceDHT::readTemperature()
{
	// Reads in fahrenheit
	data->Temperature(dht->readTemperature(true));
}

