#include <Arduino.h>
#include <Wire.h>
#include <axp20x.h>                // Power management
#include "SSD1306Wire.h"           // For SSD1306 Monitor
#include <OneWire.h>               // For DS18B20 Thermometer
#include <DallasTemperature.h>     // For DS18B20 Thermometer
#include <TinyGPSPlus.h>           // For processing GPS output
#include <WiFi.h>
#include <HTTPClient.h>            // For data upload

#define INTERVAL_TO_MEASURE 20000 // milliseconds
#define ONE_WIRE_BUS 25            // Thermometer is connected to pin 25

/* WiFi and network settings */
static const char ssid[] = ""; // Fill in your Wi-Fi name
static const char password[] = ""; // Fill in your Wi-Fi password
static const String url = "http://api.thingspeak.com/update?api_key=XXXXXXXX"; // Edit your API Key

static SSD1306Wire display(0x3c, 21, 22); // Connect SDA to pin 21 and SCL to pin 22
static OneWire oneWire(ONE_WIRE_BUS);
static DallasTemperature sensors(&oneWire);
static AXP20X_Class AXP;
static TinyGPSPlus GPS;

static unsigned long int last_updated;
static bool location_available, altitude_available;
static float latitude, longitude, altitude;

void setup(void) {
  Serial.begin(115200);

  /*
    Initialize thermometer
  */
  sensors.begin();

  /*
    Initialize OLED display
  */
  display.init();

  /*
    Connect WiFi and wait for connected
  */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 24, "Connecting WiFi");
    display.display();
    delay(1000); // 1000 milliseconds = 1 second
  }
  Serial.println("WiFi Connected");
  display.clear();
  display.display();

  /*
    Turn on power of GPS module
    AXP pin LDO3 is connected to GPS
  */
  Wire.begin();
  if (AXP.begin(Wire, AXP192_SLAVE_ADDRESS) == AXP_PASS)
    Serial.println("AXP192 PASS");
  else
    Serial.println("AXP192 FAIL");
  AXP.setPowerOutPut(AXP192_LDO3, true);

  /*
    GPS chips default setting of board T-Beam 1.1
    bit-rate: 9600 baud
    RX: pin 34
    TX: pin 12
  */
  Serial1.begin(9600, SERIAL_8N1, 34, 12);

  last_updated = millis() - INTERVAL_TO_MEASURE;
  location_available = false;
  altitude_available = false;

  /*
    UBX Protocol
    Message: CFG-CFG (clear, save and load configurations)
    Header: B5 62
    ID: 06 09

    Length of payload: 000D (bytes)
    clear mask: 0000FFFF
    save mask: 00000000
    load mask: 0000FFFF
    device mask: 04

    Checksum A: 1C
    Checksum B: 9B
  */
  static byte const UBX_CFG_CFG[] = {
    0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0x04, 0x1C, 0x9B
  };
  Serial1.write(UBX_CFG_CFG, sizeof UBX_CFG_CFG);
}

static void process_GPS_data(void) {
  while (Serial1.available())
    GPS.encode(Serial1.read());
}

static void read_GPS(void) {
  location_available = GPS.location.isValid() && GPS.location.isUpdated();
  altitude_available = GPS.altitude.isValid() && GPS.altitude.isUpdated();
  if (location_available) {
    latitude = GPS.location.lat();
    longitude = GPS.location.lng();
  }
  if (altitude_available)
    altitude = GPS.altitude.meters();
}

void show_info(float tempC) {
  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(10, 5, "HKU Wi-Fi"); // Try to edit your school name here
  display.drawHorizontalLine(0, 30, 128);

  display.setFont(ArialMT_Plain_10);
  display.drawString(5, 35, "Temperature: "+String(tempC)+" C");
  Serial.println(tempC);
  if (location_available) {
    char buffer[32];
    snprintf(buffer, sizeof buffer, "%f,%f", latitude, longitude);
    display.drawString(5, 50, buffer);
  } else {
    display.drawString(5, 50, "No GPS");
  }

  display.display();
};

static void upload_data(float tempC) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  Serial.println("Start connection");
  HTTPClient http;

  String url_with_data = url+"&field1="+tempC;
  if (location_available) {
    url_with_data += "&field2="+String(latitude, 6);
    url_with_data += "&field3="+String(longitude, 6);
  }
  if (altitude_available) {
    url_with_data += "&field4="+String(altitude, 6);
  }
  if (GPS.date.isValid() && GPS.time.isValid()) {
    char buffer[64];
    snprintf(
      buffer, sizeof buffer,
      "&field5=%04u%02u%02uT%02u%02u%02u",
      GPS.date.year(), GPS.date.month(), GPS.date.day(),
      GPS.time.hour(), GPS.time.minute(), GPS.time.second()
    );
    url_with_data += buffer;
  }
  Serial.println(url_with_data);

  http.begin(url_with_data);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    String payload = http.getString();
    Serial.print("Webpage Content: ");
    Serial.println(payload);
  } else {
    Serial.println("Transmission Failed");
  }
  http.end();
}

void loop(void) {
  process_GPS_data();
  unsigned long int now = millis();
  if (now - last_updated > INTERVAL_TO_MEASURE) {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    read_GPS();
    show_info(tempC);
    upload_data(tempC);
    last_updated = now;
  }
}
