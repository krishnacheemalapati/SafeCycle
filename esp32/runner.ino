#include <LiquidCrystal.h>  
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Network credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Server details
const char* serverUrl = "http://your-server:8000/api/readings/";

// Hardware definitions
#define LED_ONBOARD 2
#define LED_RED 5
#define LED_GREEN 15
#define LED_YELLOW 4

#define LEFT_US_TRIG 19
#define LEFT_US_ECHO 21

#define RIGHT_US_TRIG 16
#define RIGHT_US_ECHO 17

#define BACK_US_TRIG 22
#define BACK_US_ECHO 23

#define BUZZER_PIN 18 

// Moving average setup
const int numReadings = 8;
int leftReadings[numReadings] = {0}; 
int rightReadings[numReadings] = {0};
int backReadings[numReadings] = {0};
int leftIndex = 0, rightIndex = 0, backIndex = 0;
int leftSum = 0, rightSum = 0, backSum = 0;
LiquidCrystal lcd(32, 33, 25, 14, 27, 26);

// Device identification
String deviceId = "SAFECYCLE_001";

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Initialize hardware
  pinMode(LED_ONBOARD, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LEFT_US_TRIG, OUTPUT);
  pinMode(LEFT_US_ECHO, INPUT);
  pinMode(RIGHT_US_TRIG, OUTPUT);
  pinMode(RIGHT_US_ECHO, INPUT);
  pinMode(BACK_US_TRIG, OUTPUT);
  pinMode(BACK_US_ECHO, INPUT);

  lcd.begin(8, 2);
}

void writeMsg(const char* LOCATION) {
  lcd.setCursor(0, 0);
  lcd.print("WATCH");
  lcd.setCursor(0, 1);
  lcd.print(LOCATION);
}

int readFromSensor(int ECHO_PIN, int TRIG_PIN, const char* LOCATION) {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.0343) / 2; // Convert to cm

  Serial.print(LOCATION);
  Serial.print(" sensor distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

int getMovingAverage(int newReading, int* readings, int& index, int& sum) {
  sum -= readings[index];
  readings[index] = newReading;
  sum += newReading;
  index = (index + 1) % numReadings;
  return sum / numReadings;
}

void sendDataToServer(float leftDist, float rightDist, float backDist, const char* alertLevel) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Create JSON document
    StaticJsonDocument<200> doc;
    doc["device_id"] = deviceId;
    doc["left_distance"] = leftDist;
    doc["right_distance"] = rightDist;
    doc["back_distance"] = backDist;
    doc["alert_level"] = alertLevel;

    String jsonString;
    serializeJson(doc, jsonString);

    int httpResponseCode = http.POST(jsonString);
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  }
}

void loop() {
  int leftDist = readFromSensor(LEFT_US_ECHO, LEFT_US_TRIG, "LEFT");
  int rightDist = readFromSensor(RIGHT_US_ECHO, RIGHT_US_TRIG, "RIGHT");
  int backDist = readFromSensor(BACK_US_ECHO, BACK_US_TRIG, "BACK");

  int avgLeft = getMovingAverage(leftDist, leftReadings, leftIndex, leftSum);
  int avgRight = getMovingAverage(rightDist, rightReadings, rightIndex, rightSum);
  int avgBack = getMovingAverage(backDist, backReadings, backIndex, backSum);

  Serial.print("Avg Left: "); Serial.println(avgLeft);
  Serial.print("Avg Right: "); Serial.println(avgRight);
  Serial.print("Avg Back: "); Serial.println(avgBack);

  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  const char* alertLevel = "NONE";

  if (avgLeft <= 10 || avgRight <= 10 || avgBack <= 10) {
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER_PIN, 300, 1000);
    Serial.println("<10! RED ON");
    alertLevel = "RED";
    if (avgLeft <= avgRight && avgLeft <= avgBack) {
      writeMsg("LEFT");
    } else if (avgRight <= avgLeft && avgRight <= avgBack) {
      writeMsg("RIGHT");
    } else {
      writeMsg("BACK");
    }
  } else if (avgLeft <= 20 || avgRight <= 20 || avgBack <= 20) {
    digitalWrite(LED_YELLOW, HIGH);
    noTone(BUZZER_PIN);
    Serial.println("<20! YELLOW ON");
    alertLevel = "YELLOW";
    if (avgLeft <= avgRight && avgLeft <= avgBack) {
      writeMsg("LEFT");
    } else if (avgRight <= avgLeft && avgRight <= avgBack) {
      writeMsg("RIGHT");
    } else {
      writeMsg("BACK");
    }
  } else {
    noTone(BUZZER_PIN);
    Serial.println(">20! LEDs OFF");
  }

  // Send data to server
  sendDataToServer(avgLeft, avgRight, avgBack, alertLevel);

  delay(100);  // Short delay for stability
}
