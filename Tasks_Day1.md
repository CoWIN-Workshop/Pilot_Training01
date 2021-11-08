# Tasks for Day 1
********************************************************************

### Task 0: Installation of Arduino IDE

### Task 1: Test the device
Connect the TTGO deivce to computer and follow [ESP32 Installation Manual](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/ESP32_Installation.md) to setup the environment

### Task 2: Include Arduino Libraries
Try to download the Arduino libraries from [Library page](https://github.com/CoWIN-Workshop/Pilot_Training01/tree/main/Libraries)
<img width="1056" alt="螢幕截圖 2021-09-24 下午1 40 24" src="https://user-images.githubusercontent.com/90884001/134624165-1674d8d5-5a78-485d-a787-55febad6d596.png">

Add the libraries to Arduino IDE by clicking ```Sketch``` -> ```Include Library``` -> ```Add .ZIP Library```
<img width="880" alt="Add zip libraries" src="https://user-images.githubusercontent.com/90884001/134623767-2cd22af6-be38-47b3-a863-75a3e451dac0.png">

After completion, you may find the libraries under ```File``` -> ```Examples```
<img width="731" alt="Search Library" src="https://user-images.githubusercontent.com/90884001/134623803-4bd65480-eb06-4147-8ed1-ef96ac8d958d.png">

### Task 3: Load the script for showing text on the monitor (SSD1306)
Test your device with this sample script
Download here: [Monitor_SSD1306.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Samples/Monitor_SSD1306.ino)

or copy and paste the below script:

```
// Include the required Arduino libraries:
#include <Wire.h>  
#include "SSD1306Wire.h"

SSD1306Wire  display(0x3c, 21, 22);

// Adapted from Adafruit_SSD1306
void drawCircle(void) {
  for (int16_t i=0; i<display.getHeight(); i+=2) {
    display.drawCircle(display.getWidth()/2, display.getHeight()/2, i);
    display.display();
    delay(10);
  }
  delay(1000);
  display.clear();

  // This will draw the part of the circel in quadrant 1
  // Quadrants are numberd like this:
  //   0010 | 0001
  //  ------|-----
  //   0100 | 1000
  //
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000001);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000011);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000111);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00001111);
  display.display();
}

void printBuffer(void) {
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(65, 5, "HKU");  //Try to edit your school name here 
    display.drawHorizontalLine(0, 35, 150);
    display.setFont(ArialMT_Plain_10);
    display.drawString(65, 40, "Pilot Workshop (Day 1)");
    display.display();
  };

void setup() {
  display.init();

  drawCircle();
  delay(1000);
  display.clear();

  printBuffer();
  delay(1000);
  display.clear();
}

void loop() { }
```

### Task 4: Connect circuitry with breadboard (DS18B20 Thermometer)
- Connect the positive pole of DS18B20 -> 3.3V
- Connect the negative pole of DS18B20 -> GND
- Connect the data pole of DS18B20 -> Pin 25 of device
- Add a 4.7K Ohm resistor between positive and data wire

<img width="592" alt="DS18B20 Schematic _ V2" src="https://user-images.githubusercontent.com/90884001/134139253-05905ad6-8d27-4b13-b0b0-0bdbbfc80d2e.png">

### Task 5: Load the script for detecting temperature with DS18B20 Thermometer
Test your device with this sample script Download here: [Monitor_DS18B20.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Samples/Monitor_DS18B20.ino)

or copy and paste the below script:
```
// Include the required Arduino libraries:
#include <SPI.h>
#include <Wire.h>          
#include "SSD1306Wire.h"            //For SSD1306 Monitor

#include <OneWire.h>                //For DS18B20 Thermometer
#include <DallasTemperature.h>      //For DS18B20 Thermometer
#define ONE_WIRE_BUS 25            //Connect DS18B20 to pin 25

SSD1306Wire display(0x3c, 21, 22); //Connect SDA to pin 21 and SCL to pin 22

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void printBuffer(void) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(48, 5, "HKU");  //Try to edit your school name here 
    display.drawHorizontalLine(0, 30, 150);
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    display.setFont(ArialMT_Plain_16);

    display.drawString(5, 45, "Temp: "+String(tempC)+" "+(char)247+"C");
    Serial.println(tempC);
    display.display();
  };

void setup() {
  Serial.begin(115200);
  sensors.begin();
  display.init();
  display.clear();
}

void loop() { 
  printBuffer();
  delay(1000);
  }
```
### Task 6: Setup ThingSpeak IoT Account
Register for a free account on [ThingSpeak webpage](https://thingspeak.com) :
<img width="1440" alt="Thinkspeak Register" src="https://user-images.githubusercontent.com/90884001/134031113-0e2d4c2d-ee16-433a-b769-2b3193462513.png">

Set your own channel layout and request API for data sending:
<img width="877" alt="Thinkspeak API" src="https://user-images.githubusercontent.com/90884001/134031226-c776e37f-23aa-4cd4-bdf8-47ba877b995a.png">


### Task 7: Load the script for data uploading with Wi-Fi transmission
Test your device with this sample script Download here: [PilotWorkshop_WiFi.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Samples/PilotWorkshop_WiFi.ino)

or copy and paste the below script:
```
// Include the required Arduino libraries:
#include <SPI.h>
#include <Wire.h>          
#include "SSD1306Wire.h"            //For SSD1306 Monitor

#include <OneWire.h>                //For DS18B20 Thermometer
#include <DallasTemperature.h>      //For DS18B20 Thermometer
#define ONE_WIRE_BUS 25            //Connect DS18B20 to pin 25

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
    http.begin(url1);
    Serial.println(url1);

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

void loop() { 
  wifi();
  delay(20000);
  }
```

### Task 8: DS18B20 Calibration
--> Refer to powerpoint slides

### Task 9: Divide Groups
--> Refer to powerpoint slides

### Task 10: Touch on 3D CAD and 3D printing tools
--> Refer to powerpoint slides
