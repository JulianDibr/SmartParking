#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

const char* host = "http://192.168.178.60/api/sendStatus";
//const char updateStatusURL = "/sendStatus";
//const char getSettingsURL = "";

const int echoPin = 34                                                                                                                                                                            ;
const int trigPin = 27;
const int redLed = 25;
const int greenLed = 26;

const String deviceID = "2";

long duration;
int distance;

void setup() {
  //Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //LED outputs
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  Serial.begin(115200); //Serieller Monitorausgabe starten

  //WIFI
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  //Trigger and Measure the Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Convert and output Sensor data in cm
  distance = duration * 0.03434 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  checkForStatusUpdate(distance);
  delay(7000);
}

void checkForStatusUpdate (int distance) {
  if (distance < 2000) {
    if (distance < 30) {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      sendStatusToApi(1);
    }
    else {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      sendStatusToApi(0);
    }
  }
}

void sendStatusToApi (int spaceOccupied) {
  HTTPClient http;
  String postData;
  String status = String(spaceOccupied);
  
  //0 = no => Not Occupied; 1 = yes => Occupied
  postData =  "status=" + status + "&device_id=" + deviceID;
  http.begin(host);
  
http.addHeader("Content-Type", "application/x-www-form-urlencoded");  ^     ^^^       
  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);

  http.end();
}
