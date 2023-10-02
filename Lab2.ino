// Victoria Solodovnikova
// ID 40133650

// Traffic light set one
int red1 = 6;
int yellow1 = 8;
int green1 = 7;

// Traffic light set two
int red2 = 9;
int yellow2 = 11; 
int green2 = 10;

//vars to store whether each button has been pushed
bool button1Pushed = false;
bool button2Pushed = false;

void setup () {
// setup Traffic light set one
  pinMode(red1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(3, INPUT_PULLUP);

  // setup Traffic light set two
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);  
  pinMode(2, INPUT_PULLUP);
  
  // setup an interrupt for each button
  attachInterrupt(digitalPinToInterrupt(3), blink1, FALLING); // call blink1() when button is pushed
  attachInterrupt(digitalPinToInterrupt(2), blink2, FALLING); // call blink2() when button is pushed
}

void loop () {
  cross();// when nothing is happening, repeat the sequence starting from red2 and yellow1
}

void blink1(){ // when interrupt is triggered for button 1
  if (digitalRead(green1) == HIGH) // if green1 is on, register the button as pushed
  {
	button1Pushed = true;
  }

}

void blink2(){ // when interrupt is triggered for button 2
  if (digitalRead(green2) == HIGH)  // if green2 is on, register the button as pushed
  {
	button2Pushed = true;
  }
}

void cross(){ // sequence starting at a High red2 and a High yellow1
  while (true){ 
    digitalWrite(red2, HIGH); 
    digitalWrite(green1, LOW);
    digitalWrite(yellow1, HIGH);
    delay (1000);
    digitalWrite(yellow1, LOW);
    digitalWrite(red2, LOW);
    digitalWrite(red1, HIGH);
    digitalWrite(green2, HIGH);

    for (int i = 0; i <= 16; i++){ // delay of 4 seconds split into 16 to allow change of flow if button 2 is pushed
      if (button2Pushed){restarter2();}
        delay (250);
    }

    digitalWrite(green2, LOW);
    digitalWrite(yellow2, HIGH);
    delay (1000);
    digitalWrite(red1, LOW);
    digitalWrite(yellow2, LOW);
    digitalWrite(green1, HIGH);
    digitalWrite(red2, HIGH);

    for (int i = 0; i <= 16; i++){ // delay of 4 seconds split into 16 to allow change of flow if button 1 is pushed
      if (button1Pushed){restarter1();}
        delay (250);
    }
  }
}

void cross2(){ // sequence starting at a High red1 and a High yellow2
  while (true){
    digitalWrite(red1, HIGH);
    digitalWrite(green2, LOW);
    digitalWrite(yellow2, HIGH);
    delay (1000);
    digitalWrite(yellow2, LOW);
    digitalWrite(red1, LOW);
    digitalWrite(red2, HIGH);
    digitalWrite(green1, HIGH);

    for (int i = 0; i <= 16; i++){ // delay of 4 seconds split into 16 to allow change of flow if button 1 is pushed
      if (button1Pushed){restarter1();}
        delay (250);
    }

    digitalWrite(green1, LOW);
    digitalWrite(yellow1, HIGH);
    delay (1000);
    digitalWrite(red2, LOW);
    digitalWrite(yellow1, LOW);
    digitalWrite(green2, HIGH);
    digitalWrite(red1, HIGH);

    for (int i = 0; i <= 16; i++){ // delay of 4 seconds split into 16 to allow change of flow if button 2 is pushed
      if (button2Pushed){restarter2();}
        delay (250);
    }
  }
}

void restarter1(){ // "restarts" the button1Pushed to false and calls cross() for a new cycle
	button1Pushed = false;
  	cross();
}

void restarter2(){ // "restarts" the button2Pushed to false and calls cross2() for a new cycle
	button2Pushed = false;
  	cross2();
}
