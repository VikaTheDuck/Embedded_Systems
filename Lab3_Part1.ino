#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "SM-G955W9328";
const char* password = "victoria";
unsigned long previousMillis = 0;
WebServer server(80);

int pos = 0; // init position of servo to 0
Servo servo_12;
int incorrectAttempts = 0; // incorrect attempt counter

//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) { // check if user is already authentified
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  } // if authentication fails, print message in serial monitor
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  digitalWrite(21, LOW);
  digitalWrite(13, LOW);
  servo_12.write(0);

  String msg;
  if (server.hasHeader("Cookie")) { // check if theres a cookie available
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) { // disconnection handler
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) { // check if there is something entered
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin") { // check if entered credentials are correct
      if(incorrectAttempts < 3){
        server.sendHeader("Location", "/");
        server.sendHeader("Cache-Control", "no-cache");
        server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
        server.send(301);

        Serial.println("Log in Successful");
        incorrectAttempts = 0;   // reset incorrect attempts
        digitalWrite(21, HIGH); // turn on green led
        servo_12.write(180);
        return;
      }

    }
    incorrectAttempts++; // increment incorrect attempts
    digitalWrite(13, HIGH); // turn on red led

    msg = "Wrong username/password! try again. ";
    msg += "Failed Attempts: ";
    msg += incorrectAttempts;

    Serial.println("Log in Failed");
    Serial.println(incorrectAttempts);
    
    if (incorrectAttempts >= 3){
      msg += " ...Time out! Please try entering your password again in 2 minutes... ";
    }
  }
  // handle the html content
  String content = "<html><body><form action='/login' method='POST'>To log in, please use : admin/admin<br>";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
  
  if (incorrectAttempts >= 3) { // timeout of 2 minutes happens when 3 incorrect attempts are entered
    //delay(120000); 
    delay(20000); 
    incorrectAttempts = 0; 
  }
}

//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
  if (server.hasHeader("User-Agent")) {
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>"; // disconnect button
  server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound() { // handler for wrong url/invalid handle
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) { // setup
  pinMode(21, OUTPUT);
  pinMode(13, OUTPUT);

  servo_12.attach(12, 500, 2500);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}