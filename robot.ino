int enA = 5;
int enB = 6;

int in1 = 2;
int in2 = 4;
int in3 = 7;
int in4 = 8; 

#define audioSensor  A0

const int trigPin = 12;
const int echoPin = 11;
long duration;
int distanceCm, distanceInch;

int speed  = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(audioSensor, INPUT);
  Serial.begin(9600);

}

int optimalDistance =45;
int maxSpeed = 1023;
int calculateSpeed(int distance){
  return maxSpeed;
    double diff = distance - optimalDistance;
    if (diff == 0) return maxSpeed;
    diff = abs(diff);
    if (diff > optimalDistance) diff = optimalDistance;
    return maxSpeed * (1 - ((double)diff / (double)optimalDistance));
  }

void loop() {
  analogWrite(enA,speed);
  analogWrite(enB,speed);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm= duration*0.034/2;
  Serial.println(distanceCm);
  speed = calculateSpeed(distanceCm);
  //Serial.println(speed);
  int audio = analogRead(audioSensor);
  //Serial.println(audio);
  delay(100);
  //if(distanceCm > 20){
   // digitalWrite(in4,LOW);
   // digitalWrite(in2,LOW);
   // digitalWrite(in1,HIGH);
   // digitalWrite(in3,HIGH);
    //delay(1000);

  /*}else{
    
    digitalWrite(in4,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in1,HIGH);
    digitalWrite(in3,HIGH);
    delay(1000);
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    delay(300);
    digitalWrite(in4,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in1,LOW);
    digitalWrite(in3,LOW);
    
  }*/

}
