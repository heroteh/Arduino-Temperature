#include <EEPROM.h>
#include <WeatherStationData.h>
#include <WeatherStationDeviceDHT.h>
#include <WeatherStationOLED.h>
#include <WeatherStationWiFi.h>

// Chip type
#define NODEMCU 0

// What is attached
#define DEBUG 1
#define LCD_CONNECTED 0
#define BATTERY_CONNECTED 0
#define ON_BATTERY 0
#define OLED_CONNECTED 1
#define WIFI 1
#define MULTIPLEXER_PRESENT 1

// Multiplexer
#define S0 12
#define S1 13
#define S2 14
#define PHOTO_RESISTOR_PIN 1
#define BATTERY_PIN 0

// Temperature chip
#define DHTPIN 2
#define DHTTYPE DHT22

// EEPROM Stuff
#define EEPROM_LIGHT_ADDR 0
#define EEPROM_SIZE 1024

// Light stuff
// The value of 12 seems to work well for bedroom
// The value of 750 works well in the computer room to allow control with desk lamp
#define LIGHT_LEVEL_OFF 750

// Wireless
#define WIRELESS_SSID "network"
#define WIRELESS_PASS "hHitjtg3"
// You must URI(L?) encode any spaces here (%20)
// Limit to 64 characters
#define STATION_NAME "Computer%20Room"
#define WEB_SERVER "172.16.72.8"
#define WRITE_URL "/Temperature/processStation.php"
#define EMAIL_SERVER "172.16.72.8"

// When should the system perform measurements 300 = 5 minutes
#define INTERVAL_SECONDS 30

// Global variables
WeatherStationData *data;
WeatherStationDeviceDHT *dht22;
WeatherStationOLED *oled;
WeatherStationWiFi *wifi;
unsigned long previousMillis = 0;

void setup() 
{
  if(DEBUG)
  {
    Serial.begin(115200);
    while(!Serial); 
    Serial.println(" ");
    Serial.println("Starting up...");
  }

  if(MULTIPLEXER_PRESENT)
  {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
  }

  if(WIFI)
  {
    wifi = new WeatherStationWiFi(WIRELESS_SSID, WIRELESS_PASS, STATION_NAME,
      WEB_SERVER, WRITE_URL, EMAIL_SERVER, DEBUG);
    wifi->init();
  }
    
  if(OLED_CONNECTED)
  {
    oled = new WeatherStationOLED();
  }

  data = new WeatherStationData(BATTERY_CONNECTED);
  dht22 = new WeatherStationDeviceDHT(DHTPIN, data);
  dht22->begin();
  
  EEPROM.begin(EEPROM_SIZE);
}

void loop() 
{ 
  unsigned long currentMillis = millis();
  unsigned long displaySensorLength;
  unsigned long currentSensor;
  float fullLight;

  if(previousMillis == 0 || (currentMillis - previousMillis) > (INTERVAL_SECONDS * 1000))
  {
    previousMillis = currentMillis;
    
    dht22->readHumidity();
    dht22->readTemperature();
  
    if(DEBUG)
    {
      Serial.println(String("Humidity: ") + String((int)data->Humidity()));
      Serial.println(String("Temperature: ") + String((int)data->Temperature()));
    } 
  
    // Light stuff
    if(MULTIPLEXER_PRESENT)
    {
      activateMultiplexer(PHOTO_RESISTOR_PIN);
    }
    
    EEPROM.get(EEPROM_LIGHT_ADDR, fullLight);
    // If EEPROM has not yet been initialized
    if(isnan(fullLight))
    {
      fullLight = 0;  
    }
    
    data->Rawlight(analogRead(A0));
   
    // Allow a reset here
    // Good, idea but need to allow full reset somewhere, probably a function
    if(data->Rawlight() > fullLight || false)
    {
      EEPROM.put(EEPROM_LIGHT_ADDR, data->Rawlight());
      EEPROM.commit();
      fullLight = data->Rawlight();      
    }
    
    data->calculateLightPercent(fullLight);
    
    if(DEBUG)
    {
      Serial.println("Light stuff:");  
      Serial.println("Stored max light value: " + (String)fullLight);
      Serial.println("Current light value   : " + (String)data->Rawlight());
      Serial.println("Current light %       : " + (String)data->LightPercent());
      // Need to check and see if it even should be off at night
      if(data->Rawlight() < LIGHT_LEVEL_OFF)
      {
        Serial.println("Display should be off!");  
      }
    }

    if(WIFI)
    {
      wifi->writeServerData(data);
    }

    // Battery stuff needs dealt with
    if(OLED_CONNECTED)
    {
      // Need to check and see if it even should be off at night
      if(data->Rawlight() < LIGHT_LEVEL_OFF)
      {
        oled->displayOff();
      }
      else
      { 
        // Calculate how long each sensor should be displayed
        displaySensorLength = INTERVAL_SECONDS / data->numberOfSensors() * 1000;
        currentMillis = millis();
        currentSensor = 1;
        oled->displayData(data, data->getCurrentSensor(currentSensor));
        if(DEBUG)
        {
          Serial.println("Current Sensor " + data->getCurrentSensor(currentSensor) + " " + (String)currentSensor);
        }
        while((currentMillis - previousMillis) < (INTERVAL_SECONDS * 1000))
        {
          if((currentMillis - previousMillis) > displaySensorLength)
          {
            currentSensor++;
            displaySensorLength += displaySensorLength;  
            if(DEBUG)
            {
              Serial.println("Current Sensor " + data->getCurrentSensor(currentSensor) + " " + (String)currentSensor);
            }
            oled->displayData(data, data->getCurrentSensor(currentSensor));
          }
          currentMillis = millis();
          delay(1);
        }
      }
    }
    
    if(BATTERY_CONNECTED && ON_BATTERY)
    {
      ESP.deepSleep(INTERVAL_SECONDS * 1000000, WAKE_RF_DEFAULT);
    }
  }
}

void activateMultiplexer(int pin)
{
  // S0 is least significant bit
  if(pin & 1)
  {
    digitalWrite(S0, HIGH);    
  }
  else
  {
    digitalWrite(S0, LOW);        
  }
  if(pin & 2)
  {
    digitalWrite(S1, HIGH);    
  }
  else
  {
    digitalWrite(S1, LOW);        
  }
  if(pin & 4)
  {
    digitalWrite(S2, HIGH);    
  }
  else
  {
    digitalWrite(S2, LOW);        
  }
}

