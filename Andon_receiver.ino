//Receiver code(led side)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "8029567010";      // Replace with your WiFi SSID
const char* password = "8029567010";  // Replace with your WiFi password

ESP8266WebServer server(80);

int ledPin1 = D0; // Connect LED 1 to D1 (GPIO5)
int ledPin2 = D1; // Connect LED 2 to D2 (GPIO4)
int ledPin3 = D2; // Connect LED 3 to D3 (GPIO0)
int ledPin4 = D3;
const int buzzerPin = D4;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/led/1", []() {
    digitalWrite(ledPin1, !digitalRead(ledPin1));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    delay(10);
    digitalWrite(ledPin1, digitalRead(ledPin1));
    digitalWrite(buzzerPin, digitalRead(buzzerPin));
    server.send(200, "text/plain", "LED 1 toggled");
  });

  server.on("/led/2", []() {
    digitalWrite(ledPin2, !digitalRead(ledPin2));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    delay(10);
    digitalWrite(ledPin2, digitalRead(ledPin2));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    server.send(200, "text/plain", "LED 2 toggled");
  });

  server.on("/led/3", []() {
    digitalWrite(ledPin3, !digitalRead(ledPin3));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    delay(10);
    digitalWrite(ledPin3, digitalRead(ledPin3));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    server.send(200, "text/plain", "LED 3 toggled");
  });

  server.on("/led/4", []() {
    digitalWrite(ledPin4, !digitalRead(ledPin4));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    delay(10);
    digitalWrite(ledPin4, digitalRead(ledPin4));
    digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    server.send(200, "text/plain", "LED 4 toggled");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("Server started");
  
}

void loop() {
  server.handleClient();
}
