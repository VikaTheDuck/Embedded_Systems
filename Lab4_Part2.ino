#include <BluetoothSerial.h>

// define errors
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

// time to discover bluetooth devices available
#define BT_DISCOVER_TIME    5000


static bool btScanAsync = true;
static bool btScanSync = true;
bool isDetected = false; // flag that saves whether it is the correct device or not


void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) { // Prints out the names, addreses, cod and rssi of detected devices
    Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str()); // print found devices to the Serial 

  if(pDevice->getAddress().toString() == "30:6a:85:5e:37:b1"){ // Checks if the detected device's address corresponds to mine
    isDetected = true;
  }
  if (isDetected){ // turn on builtin led if my device detected
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW); // turn off the led if my device is not detected
  }
}

void setup() { // one time setup
  Serial.begin(115200);
  SerialBT.begin("HC-05_TTGO"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED_BUILTIN, OUTPUT); // setup builtin led
}

void bluetoothDetection(){ // constantly detecting if there a BT device available
  isDetected = false;
  if (btScanAsync) {
    Serial.print("Starting discoverAsync...");
    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) { // get names, addresses, cod, rssi of found devices and print them
      Serial.println("Findings will be reported in \"btAdvertisedDeviceFound\"");
    } else {
      Serial.println("Error on discoverAsync f.e. not workin after a \"connect\"");
    }
  }
  
  if (btScanSync) {
    Serial.println("Starting discover...");
    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
    
    if (pResults){ // if there is a succesfull scan
      pResults->dump(&Serial); // show found devices
    }
    else
      Serial.println("Error on BT Scan, no result!");
  }
}
void loop() { // run over and over
  bluetoothDetection();
  delay(1000);
}