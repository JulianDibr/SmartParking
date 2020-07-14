#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;
StaticJsonDocument<200> doc1;

const char* ssid = "";
const char* password = "";

const char* sendStatusURL = "http://192.168.2.107/api/sendStatus";
const char* getSettingsURL = "http://192.168.2.107/api/getSettings";

const short echoPin = 34                                                                                                                                                                            ;
const short trigPin = 27;
const short redLed = 25;
const short greenLed = 26;
int maxDistance = 30; //Def Wert für Messgrenze

const String deviceID = "1"; //ID des Geräts = device_id in DB-Tabelle (parking_spaces)

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

  //Verbindung zum WLAN Netzwerk
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Verbindung zum WLAN wird hergestellt");
  }
  Serial.print("Mit ");
  Serial.print(ssid);
  Serial.print(" verbunden");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  getSettingsFromApi(); //Laden der Settings über die API
  
  maxDistance = doc1["meassure_distance"]; //Distanz auf eingestellten Wert setzen
  
  Serial.print("Max Distance: ");
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
  Serial.print("Distanz: ");
  Serial.print(distance);
  Serial.println("cm");

  statusUpdate(distance); //Übermitteln des neuen Status
  delay(1500);
}

void statusUpdate (int distance) {
  if (distance < 2000) { //Filtern von unplausiblen Werten
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
  //Neuen Status an API übermitteln
  HTTPClient http;
  String postData;
  String status = String(spaceOccupied); //Status des Geräts

  //0 = no => Not Occupied; 1 = yes => Occupied
  postData =  "status=" + status + "&device_id=" + deviceID; //Formulardaten generieren
  
  http.begin(sendStatusURL); //Verbindung zur URL aufbauen
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(postData); //Übermitteln des Formulars
  
  String payload = http.getString(); //Antwort der API speichern und ausgeben
  Serial.println(payload);

  http.end(); //Verbindung schließen
}

void getSettingsFromApi () {
  Serial.print("Getting Settings from: ");
  Serial.println(getSettingsURL);
  
  HTTPClient http;
  http.begin(getSettingsURL); //Anfrage an API stellen
  http.GET();
  
  String json = http.getString(); //Erhaltenes JSON speichern
  Serial.print("Setting JSON: ");
  Serial.println(json);

  int httpCode = http.GET();
  http.end();

  deserializeJson(doc1, json); //JSON decodieren
}
