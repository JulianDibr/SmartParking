const int echoPin = 34                                                                                                                                                                            ;
const int trigPin = 27;
const int redLed = 25;
const int greenLed = 26;

const int deviceID = 1;
const char updateStatusURL = "";
const char getSettingsURL = "";

long duration;
int distance;

void setup() {
  //Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //LED outputs
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  Serial.begin(115200);
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

void sendStatusToApi (int spaceOccupied){
  //0 = no => Not Occupied; 1 = yes => Occupied
  
}
