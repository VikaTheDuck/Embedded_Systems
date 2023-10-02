/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins 2 - 8 as an output.
  for (int i = 2; i <= 7; i++){
    pinMode(i, OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  lightUp(2, 5000);
  lightUp2(7, 4000);
  lightUp(3, 4000);
  lightUp2(6, 1000);
  lightUp(4, 1000);
  lightUp2(5, 5000);


}

void lightUp(int pinColor, int d){
  digitalWrite(pinColor, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(d);                       // wait for a second
  digitalWrite(pinColor, LOW);    // turn the LED off by making the voltage LOW
  delay(0);                       // wait for a second
}
void lightUp2(int pinColor, int d){
  digitalWrite(pinColor, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(d);                       // wait for a second
  digitalWrite(pinColor, LOW);    // turn the LED off by making the voltage LOW
  delay(0);                       // wait for a second
}
