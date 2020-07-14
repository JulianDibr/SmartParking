#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

const char* ssid = "";
const char* password = "";

const char* getStatusURL = "http://192.168.2.107/api/getAllStatus";

const short redLed = 26;
const short greenLed = 25;

void setup() {
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
}

void loop() {
  getStatusUpdate(); //Aktuellen Status aller Geräte abrufen
  delay(1000);
}


void getStatusUpdate () {
  HTTPClient http;

  http.begin(getStatusURL);
  http.GET();//API Status URL aufrufen
  String json = http.getString(); //JSON return abspeichern
  Serial.println(json); //Ausgabe des JSON
  
  http.end(); //Verbindung schließen

  deserializeJson(doc, json);

  if (doc["opened"] == "true") { //Ist Parkplatz überhaupt offen?
    Serial.println("Innerhalb Öffnungszeiten");
    
    String status = doc["spaces"][0]["status"]; //Status des Geräts an erster Stelle
   
    if( status == "0") { //Ist der Parkplatz frei?
      //Grün an
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    } else { //Parkplatz ist nicht frei
      //Rot an
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
    }
  } else {
    Serial.println("Nicht geöffnet");
    //Rot an
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
  }
}
