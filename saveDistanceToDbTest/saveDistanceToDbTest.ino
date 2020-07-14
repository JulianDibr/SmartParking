#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;
StaticJsonDocument<200> doc1;
const char* ssid = "";
const char* password = "";

IPAddress local_IP(192, 168, 178, 111);
IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // Google DNS

const char* sendStatusURL = "http://192.168.178.60/api/sendStatus";
const char* getSettingsURL = "http://192.168.178.60/api/getSettings";

const short echoPin = 34                                                                                                                                                                            ;
const short trigPin = 27;
const short redLed = 25;
const short greenLed = 26;
int maxDistance = 0;

const String deviceID = "1";

long duration;
short distance;

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
  WiFi.config(local_IP, gateway, subnet, dns);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  getSettingsFromApi();
  maxDistance = doc1["meassure_distance"];
  Serial.print("max: ");
  Serial.println(maxDistance);
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
    if (distance < maxDistance) {
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
  http.begin(sendStatusURL);
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(postData);
  String payload = http.getString();
  Serial.println(payload);

  http.end();
}

void getSettingsFromApi () {
  Serial.println("in api");
  HTTPClient http;
  
  http.begin(getSettingsURL);
  
  http.GET();
  String json = http.getString();
  Serial.println(json);
  http.end();

  
  deserializeJson(doc1, json);
}
