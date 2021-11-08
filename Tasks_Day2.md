# Agenda for Day 2
********************************************************************

### Task 1: Group Presentation

### Task 2: Arduino Programming

#### **Step 1

Download the sample script here: [What_is_function.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/What_is_function.ino)

or copy and paste the below script:
```
int sum_func (int x, int y) // function declaration {
   int z = 0;
   z = x+y ;
   return z; // return the value
}

void setup () {
   Statements // group of statements
}

Void loop () {
   int result = 0 ;
   result = Sum_func (5,6) ; // function call
}
```
#### **Step 2

Download the sample script here: [How_to_write_script.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/How_to_write_script.ino)

or copy and paste the below script:
```

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

    Serial.println("Start connection");                              // Print out a notice on Serial monitor tell 
                                                                     // you the sensor starting the conection
                                                                     
    HTTPClient http;                                                 // Start to connect with the network
    
    wifirssi = WiFi.RSSI();                                          // Receive the signal strength and stored as 
                                                                     // variable: wifirssi
    
    sensors.requestTemperatures();       
    float tempC = sensors.getTempCByIndex(0);                        // Receive the temperature data and stored 
                                                                     // as variable: tempC
                                                                     
    float tempCorrected = slope*tempC + intercept;                   // %%% Task B: Edit with your own calibrated 
                                                                     // results: y = mx + c (linear equation)
     
    String url1 = url + "&field1=" + tempCorrected + "&field2=" + wifirssi;  // Construct a string that to be 
                                                                             // submitted to thingspeak
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

    display.clear();                                                // Clear the text or graphics on OLED screen
    
    display.setTextAlignment(TEXT_ALIGN_LEFT);                      // Setup orientation and font size of your text 
                                                                    // on OLED screen
    display.setFont(ArialMT_Plain_16); 
    display.drawString(10, 5, "HKU Wi-Fi");                         // %%% Task C: You may try to edit your 
                                                                    //             school name here    
                                                                    
    display.drawHorizontalLine(0, 30, 150);                         // A solid line will be drawn to separate 
                                                                    // your school name and measurement data
    display.setFont(ArialMT_Plain_16);
    display.drawString(5, 45, "Temp: "+String(tempCorrected)+" "+(char)247+"C");
    Serial.println(tempC);
       
    display.display();
  };


// *** Step 5: Put your main code here to run repeatedly

void loop() { 
  wifi();                    // Call to run the function "wifi"
  delay(20000);              // %%% Task D: You can setup your own delay time for the loop, 
                             //             1000 ms = 1 second -> 20000 ms = 20 seconds. 
                             //             Let's say I need to send data every minute, what should I set?
  }
```


### Task 3: Activate GPS Module

#### **Step 1

Download the new Arduino libraries [AXP202X_Library.zip](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Libraries/AXP202X_Library.zip)
and [TinyGPSPLUS.zip](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Libraries/TinyGPSPLUS.zip)

Add the libraries to Arduino IDE by clicking ```Sketch``` -> ```Include Library``` -> ```Add .ZIP Library```
<img width="880" alt="Add zip libraries" src="https://user-images.githubusercontent.com/90884001/134623767-2cd22af6-be38-47b3-a863-75a3e451dac0.png">

After completion, you may find the libraries under ```File``` -> ```Examples```

<img width="600" alt="Example" src="https://user-images.githubusercontent.com/90884001/140744956-66fed485-001e-47dd-8460-219a785016e3.png">



#### **Step 2

Download the sample script here: [PilotWorkshop_GPS.ino](https://github.com/CoWIN-Workshop/Pilot_Training01/blob/main/Samples/PilotWorkshop_GPS.ino)

or copy and paste the below script:
```
// Include the required Arduino libraries:
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
    url_with_data += "&latitude="+String(latitude, 6);
    url_with_data += "&longitude="+String(longitude, 6);
  }
  if (altitude_available) {
    url_with_data += "&elevation="+String(altitude, 6);
  }
  if (GPS.date.isValid() && GPS.time.isValid()) {
    char buffer[64];
    snprintf(
      buffer, sizeof buffer,
      "&time=%04u%02u%02uT%02u%02u%02u",
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
```

### Task 4: Field Sampling - HKU Weather Orienteering

A: 5 portals were setup inside the HKU campus, please find all the portal and complete the tasks there.

B: Put your sensors side-by-side to our stationary sensor for ## 5 Minutes

<img width="688" alt="螢幕截圖 2021-11-08 下午10 39 46" src="https://user-images.githubusercontent.com/90884001/140761847-48142a64-be5e-4d90-b2ac-b29550ccb9f8.png">


### Task 5: Data Downloading + Visualization
