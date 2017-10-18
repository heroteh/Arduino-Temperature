/*
  WeatherStationWiFi.cpp - Library to process WiFi stuff for my Weather Stations.
  Created by Tony Hills, October 4, 2017.
*/

#include "WeatherStationWiFi.h"

WeatherStationWiFi::WeatherStationWiFi() 
{

}

WeatherStationWiFi::WeatherStationWiFi(char *wireless_ssid, char *wireless_pass,
		char* station_name, char* web_server, char* write_url,
		char* email_server, boolean debug) 
{
	this->wireless_ssid = wireless_ssid;
	this->wireless_pass = wireless_pass;
	this->station_name = station_name;
	this->web_server = web_server;
	this->write_url = write_url;
	this->email_server = email_server;
	this->debug = debug;
}

char *WeatherStationWiFi::Wireless_ssid()
{
	return wireless_ssid;
}

void WeatherStationWiFi::init()
{
	WiFi.begin(wireless_ssid, wireless_pass);
	
	if(debug)
	{
		Serial.print("Connecting to ");
		Serial.println(wireless_ssid); 
	}
	while(WiFi.status() != WL_CONNECTED)
	{
		delay(250);
		if(debug)
		{ 
		  Serial.print(".");
		}
	}
	if(debug)
	{
		Serial.println("");
		Serial.print("Connected to ");
		Serial.println(wireless_ssid); 
	}
}

void WeatherStationWiFi::writeServerData(WeatherStationData *weatherData)
{
  // There will be a maximum five json objects
  const int bufferSize = JSON_OBJECT_SIZE(5);
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject &jsonData = jsonBuffer.createObject();
  char data[255];
  WiFiClient client;

  jsonData["name"] = station_name;
  jsonData["temperature"] = weatherData->Temperature();
  jsonData["humidity"] = weatherData->Humidity();
  jsonData["light"] = weatherData->LightPercent();

  if(weatherData->Battery_present())
  {
	jsonData["battery"] = weatherData->Battery();  
  }
  else
  {
	jsonData["battery"] = -1;
  }
  
  jsonData.printTo(data, sizeof(data));

  client.connect(web_server, 80);
  client.print(String("GET ") + write_url + String("?jsonData=") 
                  + String(data) + " HTTP/1.1\r\n" +
                  "Host: " + web_server + "\r\n" + 
                  "Connection: close\r\n\r\n");
}