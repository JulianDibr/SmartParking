#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;
  
const char* ssid = "WLAN1996";
const char* password = "Julian1996";

const char host = "http://192.168.2.107/api/sendStatus";
//const char updateStatusURL = "/sendStatus";
//const char getSettingsURL = "";

const int echoPin = 34                                                                                                                                                                            ;
const int trigPin = 27;
const int redLed = 25;
const int greenLed = 26;

const int deviceID = 1;

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
  delay(2000);
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
  //0 = no => Not Occupied; 1 = yes => Occupied
  postData =  "status=" + spaceOccupied + "&device_id=" + deviceID;
  http.begin(host);
  Serial.println(host);
  
  http.addHeader("Content_Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);

  http.end();
}
