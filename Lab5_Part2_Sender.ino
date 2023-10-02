/*********
  LoRa code was inspired by Rui Santos' tutorial.
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

#include <BluetoothSerial.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME  5000

#define USER1 "ZZQ S+"
#define USER2 "MIKTOP"

static bool btScanAsync = true;
static bool btScanSync = true;
//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  SerialBT.begin(115200);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void loop() {
  // show different info on lcd screen
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);    
  display.display();

  if (SerialBT.available()) { // read sent data by BT 
    String packets = SerialBT.readStringUntil('\n'); // read until the end
    packets.remove(packets.length()-1, 1);
     if(packets == "starwars"){ // send strwars packet
      LoRa.beginPacket();
      LoRa.print("starwars");
      LoRa.endPacket();
      Serial.println("Sent starwars");
     }
      else if(packets == "tetris"){ // send tetris packet
      LoRa.beginPacket();
      LoRa.print("tetris");
      LoRa.endPacket();
      Serial.println("Sent tetris");
     }
      else if(packets == "redOn"){ // send redOn packet
      LoRa.beginPacket();
      LoRa.print("redOn");
      LoRa.endPacket();
      Serial.println("Sent redOn");
     }
     else if(packets == "redOff"){ // send redOff packet
      LoRa.beginPacket();
      LoRa.print("redOff");
      LoRa.endPacket();
      Serial.println("Sent redOff");
     }
      else if(packets == "greenOn"){ // send greenOn packet
      LoRa.beginPacket();
      LoRa.print("greenOn");
      LoRa.endPacket();
      Serial.println("Sent greenOn");
     }
      else if(packets == "greenOff"){ // send greenOff
      LoRa.beginPacket();
      LoRa.print("greenOff");
      LoRa.endPacket();
      Serial.println("Sent greenOff");
     }


  }
  delay(100);
  delay(5000);
}
