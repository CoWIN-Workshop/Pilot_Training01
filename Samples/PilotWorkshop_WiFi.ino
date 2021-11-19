// Include the required Arduino libraries:
#include <SPI.h>                    // Serial Peripheral Interface
#include <Wire.h>          
#include "SSD1306Wire.h"            // For SSD1306 Monitor

#include <OneWire.h>                // For DS18B20 Thermometer
#include <DallasTemperature.h>      // For DS18B20 Thermometer
#define ONE_WIRE_BUS 25             // DS18B20 is connected to pin 25

#include <WiFi.h>
#include <HTTPClient.h>

SSD1306Wire display(0x3c, 21, 22); //Connect SDA to pin 21 and SCL to pin 22

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String rssi;
String packSize = "--";
String packet ;
String tempC;
String humidity;
String wifirssi;
String station;
String vbat;

//WiFi Setting--------------------------------------------
char ssid[] = ""; //Edit your Wi-Fi name
char password[] = ""; //Edit your Wi-Fi password
String url = "http://api.thingspeak.com/update?api_key=XXXXXXXX"; //Edit your API Key

void wifi(void) {
  Serial.println("Start connection");
  HTTPClient http;
  wifirssi = WiFi.RSSI();
  
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  
  String url1 = url + "&field1=" + tempC + "&field2=" + wifirssi;
  Serial.println(url1);
  http.begin(url1);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    String payload = http.getString();
    Serial.print("Webpage Content=");
    Serial.println(payload);
  } else {
    Serial.println("Transmission Failed");
  }
  http.end();

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 5, "HKU Wi-Fi");  //Try to edit your school name here 
  display.drawHorizontalLine(0, 30, 150);
  display.setFont(ArialMT_Plain_16);
  display.drawString(5, 45, "Temp: "+String(tempC)+" "+(char)247+"C");
  Serial.println(tempC);
  
  display.display();
};

void setup(void) {
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

void loop(void) {
  wifi();
  delay(20000);
}
