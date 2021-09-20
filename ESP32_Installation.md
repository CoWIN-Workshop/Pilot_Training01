# ESP32 Installation Manual
********************************************************************

Instead of commonly used ATmega328P Microcontroller (in Arduino UNO, Arduino Nano), ESP32 Microcontroller is used in this project.
You may have to install a ESP32 compiling module in Arduino IDE in order to detect your device.

```
https://dl.espressif.com/dl/package_esp32_index.json
```

### Step 1: 
Select **Preferences** on Arduino IDE toolbar

<img width="746" alt="Step 1" src="https://user-images.githubusercontent.com/90884001/133961275-fa1f08db-ff6c-44a0-86ba-8a7ecfd0bd89.png">

### Step 2:
Edit the line **Additional Boards Manager URLs:**

<img width="695" alt="Step 2" src="https://user-images.githubusercontent.com/90884001/133961383-f411b30d-2d73-4116-877f-b8f37b083ccf.png">

### Step 3:
Restart the Arduino IDE and select the item on **Tools** as below:
- Board: ESP32 Arduino -> T-Beam
- Upload Speed: "115200"
- Port: ```Your USB Port Connected```

<img width="552" alt="Step 3" src="https://user-images.githubusercontent.com/90884001/133961758-a9bf2240-7fe3-45e5-b806-1ce5b56d9c88.png">

### Step 4:
Test your device with this sample script

```