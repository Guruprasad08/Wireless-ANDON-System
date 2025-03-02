//Transmitter code (button side)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "8029567010";
const char* password = "8029567010";

ESP8266WebServer server(80);

IPAddress receiverIP(192, 168, 1, 65); // Replace with the IP address of the receiver ESP8266

const int buttonPin1 = D1; // Connect push button switch to D1 (GPIO5)
const int buttonPin2 = D2; // Connect push button switch to D2 (GPIO4)
const int buttonPin3 = D3; // Connect push button switch to D3 (GPIO0)
const int buttonPin4 = D7;

const int ledPin1 = D4; // Connect LED to D4 (GPIO2)
const int ledPin2 = D5; // Connect LED to D5 (GPIO14)
const int ledPin3 = D6; // Connect LED to D6 (GPIO12)
const int ledPin4 = D8;

// Variables to store LED states
bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;
bool ledState4 = false;

void handleButtonPress(int buttonNumber) {
  // Send the button number to the receiver
  String url = "/led/" + String(buttonNumber);
  WiFiClient client;
  if (client.connect(receiverIP, 80)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + receiverIP.toString() + "\r\n" +
                 "Connection: close\r\n\r\n");
    client.stop();
  }
  
  // Toggle the corresponding LED state
  switch (buttonNumber) {
    case 1:
      ledState1 = !ledState1;
      digitalWrite(ledPin1, ledState1 ? HIGH : LOW);
      break;
    case 2:
      ledState2 = !ledState2;
      digitalWrite(ledPin2, ledState2 ? HIGH : LOW);
      break;
    case 3:
      ledState3 = !ledState3;
      digitalWrite(ledPin3, ledState3 ? HIGH : LOW);
      break;
    case 4:
      ledState4 = !ledState4;
      digitalWrite(ledPin4, ledState4 ? HIGH : LOW);
      break;
    default:
      break;
  }
}

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  Serial.begin(115200);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", []() {
    server.send(200, "text/plain", "Hello from ESP8266!");
  });

  server.on("/button/1", []() {
    handleButtonPress(1);
    server.send(200, "text/plain", "Button 1 pressed");
  });

  server.on("/button/2", []() {
    handleButtonPress(2);
    server.send(200, "text/plain", "Button 2 pressed");
  });

  server.on("/button/3", []() {
    handleButtonPress(3);
    server.send(200, "text/plain", "Button 3 pressed");
  });

  server.on("/button/4", []() {
    handleButtonPress(4);
    server.send(200, "text/plain", "Button 4 pressed");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();

  // Check button state and debounce
  static unsigned long lastDebounceTime1 = 0;
  static unsigned long lastDebounceTime2 = 0;
  static unsigned long lastDebounceTime3 = 0;
  static unsigned long lastDebounceTime4 = 0;
  static const unsigned long debounceDelay = 50;

  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);
  int buttonState3 = digitalRead(buttonPin3);
  int buttonState4 = digitalRead(buttonPin4);

  if (buttonState1 == LOW && millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    handleButtonPress(1);
    Serial.println("Button 1 pressed");
  } else if (buttonState2 == LOW && millis() - lastDebounceTime2 > debounceDelay) {
    lastDebounceTime2 = millis();
    handleButtonPress(2);
    Serial.println("Button 2 pressed");
  } else if (buttonState3 == LOW && millis() - lastDebounceTime3 > debounceDelay) {
    lastDebounceTime3 = millis();
    handleButtonPress(3);
    Serial.println("Button 3 pressed");
  } else if (buttonState4 == LOW && millis() - lastDebounceTime4 > debounceDelay) {
    lastDebounceTime4 = millis();
    handleButtonPress(4);
    Serial.println("Button 4 pressed");
  }
}
