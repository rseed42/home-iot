//------------------------------------------------------------------------------
// Hardware Configuration
//------------------------------------------------------------------------------
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 10
#define DHT_DATA_PIN 3
//------------------------------------------------------------------------------
// Configure MySensors
//------------------------------------------------------------------------------
#define MY_DEBUG
#define MY_DEBUG_VERBOSE_RF24
// The included mysensors library needs this definition before it is imported
#define MY_RADIO_NRF24
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
// DHT Sensor
//------------------------------------------------------------------------------
#define DHTTYPE DHT22
static const uint64_t DHT_UPDATE_INTERVAL = 4000;
static const uint8_t MAX_RETRY_COUNT = 5;
//------------------------------------------------------------------------------
// Sensor Reading Data Structure
//------------------------------------------------------------------------------
class SensorReading {
  private:
    float humidityReading;
    float temperatureReading;
    uint8_t retryCount;
    MyMessage msgHumidity;
    MyMessage msgTemperature;

  public:
    SensorReading () : 
      humidityReading(0.0), 
      temperatureReading(0.0), 
      retryCount(0), 
      msgHumidity(CHILD_ID_HUM, V_HUM),
      msgTemperature(CHILD_ID_TEMP, V_TEMP)
    {}

    uint8_t getRetryCount() { return retryCount; }

    bool readSensor(DHT_Unified& dht) {
      sensors_event_t event;
      dht.humidity().getEvent(&event);
      humidityReading = event.relative_humidity;
      dht.temperature().getEvent(&event);
      temperatureReading = event.temperature;

      if(!isnan(humidityReading) && !isnan(temperatureReading)){
        retryCount = 0;
        return true;
      }
      retryCount++;
      return false; 
    }

    void printSensorReading() {
      Serial.print("Read Attempt: "); Serial.println(retryCount);
      Serial.print("Humidity: "); Serial.println(humidityReading); Serial.println();
      Serial.print("Temperature: "); Serial.println(temperatureReading);
    }

    void sendSensorReading() {
      send(msgHumidity.set(humidityReading, 1));
      send(msgTemperature.set(temperatureReading, 1));
    }
};

//------------------------------------------------------------------------------
// Node
//------------------------------------------------------------------------------
// #define MY_NODE_ID 11
// #define MY_PARENT_NODE_ID 0
// #define MY_PARENT_NODE_ID_IS_STATIC
// #define MY_RF24_PA_LEVEL RF24_PA_MAX
//------------------------------------------------------------------------------
// DHT dht;
DHT_Unified dht(DHT_DATA_PIN, DHTTYPE);
uint32_t delayMS;
SensorReading sensorReading;
//------------------------------------------------------------------------------
// Setup
//------------------------------------------------------------------------------
void setup(){
  // This is the default serial baud rate of MySensors. We need to use this speed 
  // if we have the MySensor debug ouput enabled.
  Serial.begin(115200);
  // Initialize the DHT sensor
  dht.begin();

  #ifdef MY_DEBUG
    Serial.println("--- Starting DHT test ---");
  #endif
  
  sensor_t sensor;
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  #ifdef MY_DEBUG
    Serial.print("Minimum sensor delay: "); Serial.println(delayMS);
  #endif

  if(DHT_UPDATE_INTERVAL <= (uint64_t)sensor.min_delay){
    Serial.println("Warning: DHT_UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
//  sleep(delayMS);
  delay(delayMS);
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

  // unsigned long start_send = micros();

  // Send the data only if we are sure that it is correct. Otherwise, retry 
  // MAX_RETRY_COUNT times. If this number is exceeded, we wait the full update
  // interval until the next try.
  if(sensorReading.readSensor(dht)) {
    sensorReading.printSensorReading();
    sensorReading.sendSensorReading();
    delay(DHT_UPDATE_INTERVAL);
  }
  else if(sensorReading.getRetryCount() >= MAX_RETRY_COUNT) {
    delay(DHT_UPDATE_INTERVAL);
  }
  else {
    // It doesn't make sense to read immediately, we need to wait for the sensor
    // to be ready for the next attempt
    delay(delayMS);
  }
}

