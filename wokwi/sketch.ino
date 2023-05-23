#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"
#include <BlynkSimpleEsp32.h>

const int DHT_PIN = 15;
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";
const int myChannelNumber = 2149892;
const char* myApiKey = "P86WN10L9XM9FZP9";
const char* server = "api.thingspeak.com";
const int myChannelNumberWaterCheck = 2152110;
const char* myApiKeyWaterCheck = "P5UUOFNFKTGOZELM";
char auth[] = "gCXijymz1FhTZ2pTNSAITuF91-1lOspL";
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

DHTesp dhtSensor;
WiFiClient client;
BlynkTimer timer;

void setup() {
    Serial.begin(115200);
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
    connectToWifi();
    ThingSpeak.begin(client);
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    // Send data to ThingSpeak
    ThingSpeak.setField(1, data.temperature);
    ThingSpeak.setField(2, data.humidity);
    
    int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);
    
    if(x == 200) {
        Serial.println("Temp: " + String(data.temperature, 2) + "°C");
        Serial.println("Humidity: " + String(data.humidity, 1) + "%");
        Serial.println("Data pushed successfully");
    } else {
        Serial.println("Push error: " + String(x));
    }

    Serial.println("---");
    
    // Send data to Blynk
    Blynk.run();
    Blynk.virtualWrite(V4, data.temperature);
    Blynk.virtualWrite(V5, data.humidity);
    int fieldNumber = 1;
    int isWaterNeeded = ThingSpeak.readIntField(myChannelNumberWaterCheck, fieldNumber, myApiKeyWaterCheck);
    Blynk.virtualWrite(V6, isWaterNeeded);

    // Delay 6 seconds
    delay(6000);
}

void connectToWifi() {
    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Loading Wifi ...");
    }
    Serial.println("Wifi connected !");
    Serial.println("Local IP: " + String(WiFi.localIP()));
    WiFi.mode(WIFI_STA);
}