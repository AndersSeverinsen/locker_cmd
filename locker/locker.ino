#include <ESP32Servo.h>

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OpenSesame";
const char* password = "12345678";

WiFiServer server(8080);

static const int LED_BLUE = 5;
static const int LED_RED = 27;

static const int servoPin = 13;

static const int solenoidPin = 21;

static const int buttonPin = 33;

int buttonState = 0;
bool lockerStateClosed = true;

Servo servo1;

void setup() {

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  printWifiStatus();

  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(buttonPin, INPUT);

  servo1.attach(servoPin);
  pinMode(solenoidPin, OUTPUT);

  close();
}

void open() {
  digitalWrite(LED_BLUE, HIGH); 
  digitalWrite(LED_RED, LOW);
  servo1.write(20);
  delay(1000);
  digitalWrite(solenoidPin, HIGH);
}

void close() {
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, HIGH); 
  servo1.write(110);
  delay(1000);
  digitalWrite(solenoidPin, LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    Serial.println("Locker closed");
    lockerStateClosed = true;
    close();
  }

  if (lockerStateClosed) {
    while (true) {
      WiFiClient client = server.available();

      if (client) {
        Serial.println("new client");
        String currentLine = "";  
        // an http request ends with a blank line
        bool currentLineIsBlank = true;
        while (client.connected() && lockerStateClosed) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            if (c == '\n') {                    // if the byte is a newline character
              if (currentLine.length() == 0) {
                // break out of the while loop:
                break;
              } else {    // if you got a newline, then clear currentLine:
                currentLine = "";
              }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
              currentLine += c;      // add it to the end of the currentLine
            }

            // Check to see if the client request was "GET /H" or "GET /L":
            if (currentLine.endsWith("GET /unlock")) {
              Serial.println("Locker opened");
              open();
              lockerStateClosed = false;
              break;
            }
          }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
      }
    }
  }
}

void printWifiStatus() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
}
