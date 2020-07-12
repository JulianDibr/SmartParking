#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;

const char* ssid = "";
const char* password = "";

const char* sendStatusURL = "http://192.168.178.60/api/sendStatus";
const char* getSettingsURL = "http://192.168.178.60/api/getSettings";

const short echoPin = 34                                                                                                                                                                            ;
const short trigPin = 27;
const short redLed = 25;
const short greenLed = 26;
int maxDistance = 0;

const String deviceID = "2";

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

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  JsonObject& data = getSettingsFromApi();
  maxDistance = data["meassure_distance"];
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
  delay(1000);
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

  http.end();
}

JsonObject& getSettingsFromApi () {
  HTTPClient http;
  
  http.begin(getSettingsURL);
  
  http.GET();
  String payload = http.getString();
  JsonObject& data = jsonBuffer.parseObject(payload);

  http.end();
  Serial.println(payload);

  return data;
}
