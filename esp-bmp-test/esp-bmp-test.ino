#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

const char* ssid        = "YOUR WIFI SSID"; 
const char* password    = "hiddenpassword"; 
const char* host        = "api.thingspeak.com";
const char* writeAPIKey = "YOUR APIKEY"; 

//BMP280
Adafruit_BMP280 bme;
void setup()
  {
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    Serial.begin(9600);

    //BMP280
    if (!bme.begin(0x76)) {
        Serial.println("No BMP280");
        while (1) {}
        digitalWrite(LED_BUILTIN, LOW);  
        delay(5000);                      
        digitalWrite(LED_BUILTIN, HIGH);  
        delay(5000);     
    } else {
        Serial.println("BMP280 connection done");
    }
    //Connect to WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("No WIFI");
        digitalWrite(LED_BUILTIN, LOW);  
        delay(500);                      
        digitalWrite(LED_BUILTIN, HIGH);  
        delay(500); 
      }
  }
  
void loop()
  {
    //BMP280
      String t = String(bme.readTemperature());
      String p = String(bme.readPressure());
    //TCP CONNECTION
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
      {
        Serial.println("No CONNECTION to Website");
        digitalWrite(LED_BUILTIN, LOW);  
        delay(500);                      
        digitalWrite(LED_BUILTIN, HIGH);  
        delay(500); 
        digitalWrite(LED_BUILTIN, LOW);  
        delay(5000);                      
        digitalWrite(LED_BUILTIN, HIGH);  
        delay(5000); 
      }
    String url = "/update?key=";
    url += writeAPIKey;
    url +="&field1=";
    url +=String(bme.readTemperature()); //BMP280 CELSIUS
    url +="&field2=";
    url +=String(bme.readPressure()/100); //BMP280 MILLIBAR
    url +="&field3=";
    url +=String(bme.readAltitude(1013.25)); //BMP280 METER
    url +="&field4=";
    url +=(bme.readAltitude(1013.25)); //BMP280 METER
    url += "\r\n";
    // Send request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
   Serial.println("Success, server got temp:" + String(bme.readTemperature()));
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);  
    delay(500);                      
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(500); 
    digitalWrite(LED_BUILTIN, LOW);  
    delay(500);                      
    digitalWrite(LED_BUILTIN, HIGH);  
}
