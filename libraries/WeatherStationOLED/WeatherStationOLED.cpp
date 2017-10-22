/*
  WeatherStationOLED.h - Library to contain OLED stuff for my Weather Stations.
  Created by Tony Hills, October 3, 2017.
*/
#include "Arduino.h"
#include "WeatherStationOLED.h"

WeatherStationOLED::WeatherStationOLED() 
{
	display = new Adafruit_SSD1306(OLED_RESET);
	display->begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
}

WeatherStationOLED::WeatherStationOLED(boolean off_at_night) : WeatherStationOLED()
{
	this->off_at_night = off_at_night;
} 

void WeatherStationOLED::displayData(WeatherStationData *data, String currentStarSensor)
{
	display->ssd1306_command(SSD1306_DISPLAYON);
	display->clearDisplay();
	display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setCursor(0,0);
    display->println(String("Humidity   : " + String((int)data->Humidity()) + String("%")));
    display->println(String("Temperature: " + String((int)data->Temperature()) + String("F")));
    display->println(String("Light      : " + String((int)data->LightPercent()) + String("%")));
	
	if(data->Battery_present())
	{
	    display->println(String("Battery    : " + String((int)data->Battery()) + String("V")));
	}
	
    display->setTextSize(4);
	if(currentStarSensor.equals("Temperature"))
	{
		display->println(String((int)data->Temperature()) + String("F T"));
	}
	else if(currentStarSensor.equals("Humidity"))
	{
		display->println(String((int)data->Humidity()) + String("% H"));		
	}
	else if(currentStarSensor.equals("Light"))
	{
		display->println(String((int)data->LightPercent()) + String("% L"));				
	}
    display->display();	
}

void WeatherStationOLED::displayOff()
{
	display->ssd1306_command(SSD1306_DISPLAYOFF);
}

int WeatherStationOLED::Off_at_night()
{
	return off_at_night;
}



