//------------------------------------------------------------------------------
// Configure MySensors
//------------------------------------------------------------------------------
#define MY_DEBUG
// #define MY_DEBUG_VERBOSE_RF24
#define MY_RADIO_NRF24
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 10
//------------------------------------------------------------------------------
// DHT Sensor
//------------------------------------------------------------------------------
#define DHTTYPE DHT22
#define DHT_DATA_PIN 3
// #define SENSOR_TEMP_OFFSET 0
//static const uint64_t DHT_UPDATE_INTERVAL = 60000;
static const uint64_t DHT_UPDATE_INTERVAL = 5000;
// static const uint8_t FORCE_UPDATE_N_READS = 10;
// float lastTemp;
// float lastHum;
// uint8_t nNoUpdatesTemp;
// uint8_t nNoUpdatesHum;
//------------------------------------------------------------------------------
// Node
//------------------------------------------------------------------------------
#define MY_NODE_ID 11
#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_ID_IS_STATIC
#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
//------------------------------------------------------------------------------
// Included Libraries
//------------------------------------------------------------------------------
#include <SPI.h>
#include<Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MySensors.h>
//------------------------------------------------------------------------------
// DHT dht;
DHT_Unified dht(DHT_DATA_PIN, DHTTYPE);
uint32_t delayMS;
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
//------------------------------------------------------------------------------
// Setup
//------------------------------------------------------------------------------
void setup(){
  // This is the default serial baud rate of MySensors. We need to use this speed 
  // if we have the MySensor debug ouput enabled.
  Serial.begin(115200);
  // Initialize the DHT sensor
  dht.begin();
  
  sensor_t sensor;
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  if(DHT_UPDATE_INTERVAL <= (uint64_t)sensor.min_delay){
    Serial.println("Warning: DHT_UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  sleep(delayMS);
}
//------------------------------------------------------------------------------
// Presentation
//------------------------------------------------------------------------------
void presentation(){
  sendSketchInfo("TemperatureAndHumidity", "1.1");
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);
}
//------------------------------------------------------------------------------
// Configure MySensors
//------------------------------------------------------------------------------
void loop(){

  sleep(DHT_UPDATE_INTERVAL);

  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    send(msgTemp.set(event.temperature, 1));
  }
  // Delay between measurements.
  sleep(delayMS);
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    send(msgHum.set(event.relative_humidity, 1));
  }
}






