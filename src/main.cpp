#include<Arduino.h>
#include<DHT.h>
#include "PWMDevice.h"

#define DHT_PIN 4
#define DHTTYPE DHT11
pwmDevice heater(16, 0);
pwmDevice fan(17, 1);
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8 // 0-255


DHT dht(DHT_PIN, DHTTYPE);

//Setting the state 
enum State{HOT, AMBIENT, COLD};
State currentState = AMBIENT;

//Thresholds for fan temperature
const float WARNING_HIGH_TEMP_C = 30.0;
const float WARNING_EXTREME_HIGH_TEMP_C = 35.0;

//Thresholds for heater temperature
const float WARNING_LOW_TEMP_C = 5.0;
const float WARNING_EXTREME_LOW_TEMP_C = 0.0;

//Defining the brightness levels
const int FAN_LEVEL_MAX = 255;
const int FAN_LEVEL_MIN = 80;
const int HEATER_LEVEL_MIN = 80;
const int HEATER_LEVEL_MAX = 255;


//Timings
const unsigned long DHT_SENSOR_TIMING = 2000;
const unsigned long PWM_RAMP = 20;
unsigned long previousSensorTime = 0;
unsigned long previousRampTime = 0;

//Variables to store the readings
float temperatureC = 0;
float humidity = 0;

//Functions
bool readSensorData();
void controlOutputs();
void printReadings();

void setup()
{
  Serial.begin(115200);
  delay(500);

  heater.begin(5000, 8);
  fan.begin(5000, 8);

  //Initialize the Sensor
  dht.begin();
  Serial.println("ESP32 Temperature and Humidity Monitor");
  Serial.println("Using DHT sensor with millis()");
  Serial.println("Warning LED above 30°C");
  Serial.println("Fan/Relay ON above 35°C");

}

void loop()
{
  unsigned long currentSensorTime = millis();
  
    if (currentSensorTime - previousSensorTime >= DHT_SENSOR_TIMING)
    {
        previousSensorTime = currentSensorTime;

        if (readSensorData())
        {
            controlOutputs();
        }
    }

    // Fast timer: ramp outputs smoothly
    if (currentSensorTime - previousRampTime >= PWM_RAMP)
    {
        previousRampTime = currentSensorTime;
        fan.update();
        heater.update();
    }

}

bool readSensorData()
{
  float newtemperature = dht.readTemperature();
  float newhumidity = dht.readHumidity();

  //Exit early if the readings failed and try again
  if(isnan(newtemperature) || isnan(newhumidity))
  {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  temperatureC = newtemperature;
  humidity = newhumidity;
  return true;
}

void controlOutputs()
{
 if (temperatureC >= WARNING_EXTREME_HIGH_TEMP_C) 
 {
    currentState = HOT;
    fan.setTarget(FAN_LEVEL_MAX);
    heater.setTarget(0);
  } 
  else if (temperatureC >= WARNING_HIGH_TEMP_C) 
  {
    currentState = HOT;
    fan.setTarget(FAN_LEVEL_MIN);
    heater.setTarget(0);
  } 
  else if (temperatureC <= WARNING_EXTREME_LOW_TEMP_C) 
  {
    currentState = COLD;
    heater.setTarget(HEATER_LEVEL_MAX);
    fan.setTarget(0);
  } 
  else if (temperatureC <= WARNING_LOW_TEMP_C) 
  {
    currentState = COLD;
    heater.setTarget(HEATER_LEVEL_MIN);
    fan.setTarget(0);
  } 
  else 
  {
    currentState = AMBIENT;
    heater.setTarget(0);
    fan.setTarget(0);
  }


}

void printReadings() 
{
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" °C | Humidity: ");
    Serial.print(humidity);
    Serial.print(" % | Warning LED: ");
    if (currentState == HOT)
    {
        Serial.print("HOT");
    }
    else if (currentState == COLD)
    {
        Serial.print("COLD");
    }
    else
    {
        Serial.print("AMBIENT");
    }

    Serial.print(" | Fan PWM: ");
    Serial.print(fan.getCurrentLevel());
    Serial.print(" | Heater PWM: ");
    Serial.println(heater.getCurrentLevel());
}

