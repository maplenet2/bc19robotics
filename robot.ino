int speedPinA = 8; // EnA
int speedPinB = 13; // Enb

int motorHat1 = 9; //IN1
int motorHat2 = 10; //IN2
int motorHat3 = 11; //IN3
int motorHat4 = 12; //IN4

#define trackingSensor1 2   // rightmost
#define trackingSensor2 3
#define trackingSensor3 4  // Center
#define trackingSensor4 5
#define trackingSensor5 6  // leftmost

#define audioSensor  A3
#define echoPin A1 // Distance sensor out
#define trigPin A2 // Distance sensor in

long duration;
int distanceCm;

int speed  = 200;

#define enableMotors true

void setup() {
  // put your setup code here, to run once:
#if enableMotors
  pinMode(speedPinA, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  pinMode(motorHat1, OUTPUT);
  pinMode(motorHat2, OUTPUT);
  pinMode(motorHat3, OUTPUT);
  pinMode(motorHat4, OUTPUT);
#endif
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(audioSensor, INPUT);
  pinMode(trackingSensor1, INPUT);
  pinMode(trackingSensor2, INPUT);
  pinMode(trackingSensor3, INPUT);
  pinMode(trackingSensor4, INPUT);
  pinMode(trackingSensor5, INPUT);
  Serial.begin(9600);
}

#define optimalDistance 45
#define maxSpeed 1023
int calculateSpeed(int distance) {
  //return maxSpeed;
  double diff = distance - optimalDistance;
  if (diff == 0) return maxSpeed;
  diff = abs(diff);
  if (diff > optimalDistance) diff = optimalDistance;
  return maxSpeed * (1 - ((double)diff / (double)optimalDistance));
}

int calculateAudioSpeed(int loudness)
{
   int loud = loudness*3;
   if (loud > maxSpeed) return maxSpeed;
   return loud;
}

char sensor[5];
/*read sensor value string, 1 stands for black, 0 starnds for white, i.e 10000 means the first sensor(from left) detect black line, other 4 sensors detected white ground */
String read_sensor_values()
{ int sensorvalue = 32;
  sensor[0] = digitalRead(trackingSensor1);

  sensor[1] = digitalRead(trackingSensor2);

  sensor[2] = digitalRead(trackingSensor3);

  sensor[3] = digitalRead(trackingSensor4);

  sensor[4] = digitalRead(trackingSensor5);
  sensorvalue += sensor[0] * 16 + sensor[1] * 8 + sensor[2] * 4 + sensor[3] * 2 + sensor[4];

  String senstr = String(sensorvalue, BIN);
  //Serial.println(senstr);
  return senstr.substring(1, 6);
}

void auto_tracking() {
  if (!enableMotors) return;
  String sensorval = read_sensor_values();
  //Serial.println(sensorval);
  if (sensorval == "10100" or sensorval == "00100" or sensorval == "01100" or sensorval == "11100" or sensorval == "11000" or sensorval == "10000"  or sensorval == "01000")
  {
    //The black line is in the left of the car, need  left turn
    go_Left();  //Turn left
    setMotorspeed(0, speed);
    delay(200);
    stop_Stop();
  }

  else if (sensorval == "00101" or sensorval == "00110" or sensorval == "00111" or sensorval == "00011" or sensorval == "00001"  or sensorval == "00010" ) { //The black line is  on the right of the car, need  right turn

    go_Right();  //Turn right
    setMotorspeed(speed, 0);
    delay(200);
    stop_Stop();
  }

  else if (sensorval == "11111" or sensorval == "01111" or sensorval == "11110") {
    stop_Stop();   //The car front touch stop line, need stop
    setMotorspeed(0, 0);
  }
  else if (sensorval == "00000" ) {
    go_Back();   //The car front touch stop line, need stop
    setMotorspeed(150, 150);
    delay(100);
    stop_Stop();
  }
  else {
    setMotorspeed(speed, speed);
    go_Advance();
  }
}

void setMotorspeed(int speed_L, int speed_R)
{
  analogWrite(speedPinB, speed_L);
  analogWrite(speedPinA, speed_R);
}

void go_Advance(void)  //Forward
{
  Serial.println("forward");
  digitalWrite(motorHat1, HIGH);
  digitalWrite(motorHat2, LOW);
  digitalWrite(motorHat3, HIGH);
  digitalWrite(motorHat4, LOW);
}
void go_Left(void)  //Turn left
{
  Serial.println("left");
  digitalWrite(motorHat1, HIGH);
  digitalWrite(motorHat2, LOW);
  digitalWrite(motorHat3, LOW);
  digitalWrite(motorHat4, HIGH);
}
void go_Right(void)  //Turn right
{
    Serial.println("right");
  digitalWrite(motorHat1, LOW);
  digitalWrite(motorHat2, HIGH);
  digitalWrite(motorHat3, HIGH);
  digitalWrite(motorHat4, LOW);
}
void go_Back(void)  //Reverse
{
  digitalWrite(motorHat1, LOW);
  digitalWrite(motorHat2, HIGH);
  digitalWrite(motorHat3, LOW);
  digitalWrite(motorHat4, HIGH);
}
void stop_Stop()    //Stop
{
  digitalWrite(motorHat1, LOW);
  digitalWrite(motorHat2, LOW);
  digitalWrite(motorHat3, LOW);
  digitalWrite(motorHat4, LOW);
}
bool useDistance = false;
void loop() {
  //setMotorspeed(speed, speed);
  int audio;
  if (useDistance)
  {
  // Get distance
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  Serial.print(distanceCm); Serial.print(" ");
  speed = calculateSpeed(distanceCm);
  Serial.print(speed); Serial.print(" ");
  }
  else // use audio
  {
    // Read audio
    audio = analogRead(audioSensor);
    speed = calculateAudioSpeed(audio);
    Serial.println(speed);
  }

  // read tracking sensors
  bool t1 = digitalRead(trackingSensor1);
  bool t2 = digitalRead(trackingSensor2);
  bool t3 = digitalRead(trackingSensor3);
  bool t4 = digitalRead(trackingSensor4);
  bool t5 = digitalRead(trackingSensor5);
  //Serial.print(t1); Serial.print(" ");
  //Serial.print(t2); Serial.print(" ");
  //Serial.print(t3); Serial.print(" ");
  //Serial.print(t4); Serial.print(" ");
  //Serial.print(t5); Serial.print(" ");

  //go_Advance();
  auto_tracking();



  // Show more stars the louder it is
  int stars = 1 + audio / 50;
  do {
    Serial.print('*');
    stars--;
  }
  while (stars > 0);
  Serial.println(" ");

}
