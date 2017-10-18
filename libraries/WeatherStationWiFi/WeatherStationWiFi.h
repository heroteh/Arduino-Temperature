/*
  WeatherStationWiFi.h - Library to process WiFi stuff for my Weather Stations.
  Created by Tony Hills, October 4, 2017.
*/

#ifndef WeatherStationWiFi_h
#define WeatherStationWiFi_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WeatherStationData.h>
#include <ArduinoJson.h>

class WeatherStationWiFi
{
public:
	WeatherStationWiFi();
	WeatherStationWiFi(char *wireless_ssid, char *wireless_pass,
		char* station_name, char* web_server, char* write_url,
		char* email_server, boolean debug);
	char *Wireless_ssid();
	void init();
	void writeServerData(WeatherStationData *weatherData);
private:
	char* wireless_ssid;
	char* wireless_pass;
	// Must URI %20 encode any spaces in station_name
	char* station_name;
	char* web_server;
	char* write_url;
	char* email_server;
	boolean debug;
};

#endif