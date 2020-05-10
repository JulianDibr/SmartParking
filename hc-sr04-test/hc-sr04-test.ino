const int echoPin = 34                                                                                                                                                                            ;
const int trigPin = 27;

const int redLed = 25;
const int greenLed = 26;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.03434 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  if (distance < 2000) {
    if (distance < 30) {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    }
    else {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
    }
  }
  delay(1000);
}
