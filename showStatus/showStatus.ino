#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

const char* ssid = "Gali";
const char* password = "6899319311154917laura";

const char* getStatusURL = "http://192.168.178.60/api/getAllStatus";

const short redLed = 26;
const short greenLed = 25;

void setup() {
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
  getStatusUpdate();
  delay(1000);
}


void getStatusUpdate () {
  HTTPClient http;

  http.begin(getStatusURL);

  http.GET();
  String json = http.getString();
  Serial.println(json);
  http.end();

  deserializeJson(doc, json);

  if (doc["opened"] == "true") {
    Serial.println("yes");
    String status = doc["spaces"][0]["status"];
    Serial.println( status == "0");
    if( status == "0") {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    } else {
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
    }
  } else {
    Serial.println("no");
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
  }
}
