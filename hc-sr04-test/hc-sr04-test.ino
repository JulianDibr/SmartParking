const int echoPin = 34                                                                                                                                                                            ;
const int trigPin = 27;
long duration;
int distance;

void setup() {
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
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
 delay(300);
}