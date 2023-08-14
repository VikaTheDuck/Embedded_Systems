

void setup() {
  Serial.begin(38400); // Default communication rate of the Bluetooth module
  Serial2.begin(38400); // basically SerialBT in Arduino code 
}

void loop() { // send string beacons over and over, with the correct string being "hi"
  Serial2.write("hello ");
  delay(4000);
  Serial2.write("wassup");
  delay(4000);
  Serial2.write("hi");
  delay(4000);

}