#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "ThingSpeak.h"

const char* ssid = "IoTC604";
const char* password = "ccsadmin";

unsigned long ChannelNumber = 2338026;
const char* WriteAPIKey = "NJWEHZ6FVHMNPW55";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

float temperature;
float humidity;
DHT dht11(D4, DHT11);

WiFiClient client;

void setup() {
  // Initialize serial
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  // setup DHT11
  temperature = 0.0;
  humidity = 0.0;
  dht11.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        delay(5000);
      }
      Serial.println("Connected.");
    }

    // Get Value on DHT11
    humidity = (dht11.readHumidity());
    temperature = (dht11.readTemperature());

    // set the fields with the values
    ThingSpeak.setField(1, humidity);
    ThingSpeak.setField(2, temperature);

    // Send Humidity & Temperature to Thingspeak
    int value = ThingSpeak.writeFields(ChannelNumber, WriteAPIKey);
    if (value == 200) {
      Serial.println("Humidity Channel & Temperature Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(value));
    }

    lastTime = millis();
  }
}