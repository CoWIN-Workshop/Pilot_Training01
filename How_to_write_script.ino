
// YOU CAN WRITE YOUR NOTES IN THE SCRIPT WITHOUT RUNNING THEM 
// < - (LIKE THIS, WITH "//" AT THE BEGINNING OF THE SENTENCE)

// *** Step 1: Include the required Arduino libraries:

#include <SPI.h>
#include <Wire.h>          
#include "SSD1306Wire.h"              // For SSD1306 Monitor
#include <OneWire.h>                  // For DS18B20 Thermometer
#include <DallasTemperature.h>        // For DS18B20 Thermometer
#include <WiFi.h>                     // For WiFi data transmission
#include <HTTPClient.h>               // For WiFi data transmission

#define ONE_WIRE_BUS 25               // Connect DS18B20 to pin 25
SSD1306Wire display(0x3c, 21, 22);    // Connect SDA to pin 21 and SCL to pin 22

OneWire oneWire(ONE_WIRE_BUS);        // Activate OneWire sensors
DallasTemperature sensors(&oneWire);  // Activate OneWire sensors

// *** Step 2: Define variables that will be used in the script:

String rssi;
String packSize = "--";
String packet ;
String tempC;
String tempCorrected;
String humidity;
String wifirssi;

char ssid[] = "";                                                  // %%% Task A: Edit your WiFi name
char password[] = "";                                              // %%% Task A: Edit your WiFi password
String url = "http://api.thingspeak.com/update?api_key=XXXXXXXX";  // %%% Task A: Edit your thingspeak's API Key


// *** Step 3: Configure the sensors setup and baud rate of the system

void setup() {
  Serial.begin(115200);
  sensors.begin();
  display.init();
  display.clear();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("WiFi Connected");
}


// *** Step 4: You can develop a function for convenience:

void wifi(void) {                                                    // Set up a function called wifi
    Serial.println("Start connection");                              // Print out a notice on Serial monitor tell you the sensor starting the conection
    HTTPClient http;                                                 // Start to connect with the network
    wifirssi = WiFi.RSSI();                                          // Receive the signal strength and stored as variable: wifirssi
    
    sensors.requestTemperatures();       
    float tempC = sensors.getTempCByIndex(0);                        // Receive the temperature data and stored as variable: tempC
    float tempCorrected = slope*tempC + intercept;                   // %%% Task B: Edit with your own calibrated results: y = mx + c (linear equation)
     
    String url1 = url + "&field1=" + tempCorrected + "&field2=" + wifirssi;  // Construct a string that to be submitted to thingspeak
    http.begin(url1);
    Serial.println(url1);

    int httpCode = http.GET();                                       // Try to connect with thingspeak
    if (httpCode == HTTP_CODE_OK)      {                             // If successfully connected, print out the result
    
      String payload = http.getString();
    
      Serial.print("Webpage Content=");
      Serial.println(payload);
    } else {
      Serial.println("Transmission Failed");
    }
    http.end();

    display.clear();                                                               // Clear the text or graphics on OLED screen
    display.setTextAlignment(TEXT_ALIGN_LEFT);                                     // Setup orientation and font size of your text on OLED screen
    display.setFont(ArialMT_Plain_16); 
    display.drawString(10, 5, "HKU Wi-Fi");                                        // %%% Task C: You may try to edit your school name here    
    display.drawHorizontalLine(0, 30, 150);                                        // A solid line will be drawn to separate your school name and measurement data
    display.setFont(ArialMT_Plain_16);
    display.drawString(5, 45, "Temp: "+String(tempCorrected)+" "+(char)247+"C");
    Serial.println(tempC);
    
    display.display();
  };


// *** Step 5: Put your main code here to run repeatedly

void loop() { 
  wifi();                    // Call to run the function "wifi"
  delay(20000);              // %%% Task D: You can setup your own delay time for the loop, 1000 ms = 1 second -> 20000 ms = 20 seconds. 
                             //             Let's say I need to send data every minute, what should I set?
  }
