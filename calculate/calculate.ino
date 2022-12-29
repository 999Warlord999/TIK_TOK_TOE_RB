#include<Servo.h>
Servo myServo;
// defines pins numbers
const int stepPin1 = 6;
const int dirPin1  = 7;

const int stepPin2 = 8;
const int dirPin2  = 9;

const int enPin  = 8;
int currentAngle = 0;
int angle = 0;
float stepPerAngle = 1.8/4 ; // full step = 1.8
int   numstep;

int currentAngle2 = 0;
int angle2 = 0;
float stepPerAngle2 = 1.8/4 ; // full step = 1.8
int   numstep2;

int arr[100] ;
int arr2[100] ;
int delay_time_change = 0;
uint8_t d = 60;

int tf,tc,chosen_accel;

//Test 2nd commit
void setup() {
  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT);
  pinMode(dirPin1,OUTPUT);

  pinMode(stepPin2,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  digitalWrite(dirPin1,HIGH);
  Serial.begin(9600);
  cal_circle();
  myServo.attach(10);
}
void loop() {

  if (Serial.available()){
    char c = Serial.read();
    if( c == 'A'){
        angle = 0;
        angle2 = 0; 
        move_robot();
    }
    else if(  c == 'B'){
        move_circle();
        d = 60;
        
    }
     else if(  c == 'C'){
        angle = 90;
        angle2 = 90; 
        move_robot();
    }

     else if(  c == 'D'){
        angle = 46;
        angle2 = 116; 
        move_robot();
    }
  }
 myServo.write(d);
  
}

void cal_circle(){
  uint8_t i, i2;
  float t4,x,y,t2,t1;
  int r = 4;
  int x0 = 20;
  int y0 = 0,l1 = 25,l2 =20; 
  for(int t3 = 0; t3 <= 360; t3 += 36)
  {
    //Serial.println(t3);
    t4 = t3*PI/180;
    //Serial.println(t4);
    x = r *cos(t4) + x0;
    //Serial.println(x);
    y = r *sin(t4) + y0;
    t2 = acos((pow(x,2)+pow(y,2)-pow(l1,2)-pow(l2,2))/(2*l1*l2));
    //Serial.println(t2);
    t1 = atan2(y,x)-asin(l2*sin(t2)/(sqrt(pow(x,2)+pow(y,2))));
    angle2 = t2*180/PI;
    angle = -t1*180/PI;
    //    Serial.print("angle   ");
    //    Serial.print(angle);
    //    Serial.print("    angle2   ");
    //    Serial.println(angle2);

    arr[i] = angle ; 
    arr2[i2] = angle2 ;
    //    Serial.println(arr[i]);
    i++;
    i2++;
  }

  
}

void move_circle() {
  int arr_length = 0;
  for (int i = 0; arr[i] != 0; i++) arr_length++;
  for (uint8_t i3 = 0; i3 < arr_length; i3++){
    angle = arr[i3];
    angle2 = arr2[i3];
    if (arr[i3] == 0 || arr2[i3] == 0) break;
    Serial.print(arr[i3]);
    Serial.print("--");
    Serial.println(arr2[i3]);
    if (i3 > arr_length * 1 / 5) delay_time_change += 300;
    if (i3 > arr_length * 3 / 5) delay_time_change += 1000;
    move_robot_shape();
  }
  delay_time_change = 0;
}
    
void moveXZ_shape(long nStepX, int stepPinX, long nStepZ, int stepPinZ) {
 
  float nStepMax = nStepX;
  float nStepMin = nStepZ;
  int stepPinMax = stepPinX;
  int stepPinMin = stepPinZ;

  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0;
  int Delay = 5000 + delay_time_change;
  int pulseMax = 0; //print to monitor
  int pulseMin = 0; //print to monitor
  
  if (nStepZ > nStepX) {
    nStepMax = nStepZ;
    nStepMin = nStepX;
    stepPinMax = stepPinZ;
    stepPinMin = stepPinX;
  }

  ratio_max_min = nStepMax / nStepMin;
 
  for (int i = 1 ; i <= nStepMax ; i = i + 1) {
    current_axis_min = i / ratio_max_min;
    if (current_axis_min - steps_axis_min >= 1) {
      digitalWrite(stepPinMin, HIGH);
      pulseMin = 1;
      steps_axis_min++;
    }

    digitalWrite(stepPinMax, HIGH);
    pulseMax = 1;
    //    Serial.print(pulseMax);
    //    Serial.print(" ");
    //    Serial.println(pulseMin);
    delayMicroseconds(Delay);

    digitalWrite(stepPinMax, LOW);
    digitalWrite(stepPinMin, LOW);
    pulseMax = 0;
    pulseMin = 0;
    //    Serial.print(pulseMax);
    //    Serial.print(" ");
    //    Serial.println(pulseMin);
    delayMicroseconds(Delay);
  }
}

void move_robot_shape() {
  uint16_t n;
  uint16_t n2;
  if (currentAngle==arr[0]){d = 0;}
  myServo.write(d);
  if(( currentAngle == angle )&&( currentAngle2 != angle2 )){
    numstep = 0;  
    if( currentAngle2 < angle2){
      digitalWrite(dirPin2, HIGH);
      n2 = angle2 - currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    else if( currentAngle2 > angle2){
      digitalWrite(dirPin2, LOW);
      n2 = currentAngle2 - angle2;
      if( angle2 == 0) n2 = currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    moveXZ_shape(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
  else if(( currentAngle != angle )&&( currentAngle2 == angle2 )){
    numstep2 = 0;
    if( currentAngle < angle){
      digitalWrite(dirPin1, HIGH);
      n = angle - currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    else if( currentAngle > angle){
      digitalWrite(dirPin1, LOW);
      n = currentAngle - angle;
      if( angle == 0) n = currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    moveXZ_shape(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
  else if(( currentAngle != angle )&&( currentAngle2 != angle2 )){
    if( currentAngle < angle){
      digitalWrite(dirPin1,HIGH);
      n = angle - currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    else if( currentAngle > angle){
      digitalWrite(dirPin1,LOW);
      n = currentAngle - angle;
      if( angle == 0) n = currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }  
    if( currentAngle2 < angle2){
      digitalWrite(dirPin2,HIGH);
      n2 = angle2 - currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    else if( currentAngle2 > angle2){
        digitalWrite(dirPin2,LOW);
        n2 = currentAngle2 - angle2;
        if( angle2 == 0) n2 =currentAngle2;
        numstep2 = (n2 / stepPerAngle2) * 3;
    }
    moveXZ_shape(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
}

void moveXZ(long nStepX, int stepPinX, long nStepZ, int stepPinZ) {
 
  float nStepMax = nStepX;
  float nStepMin = nStepZ;
  int stepPinMax = stepPinX;
  int stepPinMin = stepPinZ;

  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0;
  int Delay = 5000;
  int pulseMax = 0; //print to monitor
  int pulseMin = 0; //print to monitor
  
  if (nStepZ > nStepX) {
    nStepMax = nStepZ;
    nStepMin = nStepX;
    stepPinMax = stepPinZ;
    stepPinMin = stepPinX;
  }

  ratio_max_min = nStepMax / nStepMin;
 
  for (int i = 1 ; i <= nStepMax ; i = i + 1) {
    current_axis_min = i / ratio_max_min;
    if (i > nStepMax * 2 / 3) Delay += 17;
    if (current_axis_min - steps_axis_min >= 1) {
      digitalWrite(stepPinMin, HIGH);
      pulseMin = 1;
      steps_axis_min++;
    }

    digitalWrite(stepPinMax, HIGH);
    pulseMax = 1;
    //    Serial.print(pulseMax);
    //    Serial.print(" ");
    //    Serial.println(pulseMin);
    delayMicroseconds(Delay);

    digitalWrite(stepPinMax, LOW);
    digitalWrite(stepPinMin, LOW);
    pulseMax = 0;
    pulseMin = 0;
    //    Serial.print(pulseMax);
    //    Serial.print(" ");
    //    Serial.println(pulseMin);
    delayMicroseconds(Delay);
  }
}
 
void move_robot() {
  uint16_t n;
  uint16_t n2;
  
  if(( currentAngle == angle )&&( currentAngle2 != angle2 )){
    numstep = 0;  
    if( currentAngle2 < angle2){
      digitalWrite(dirPin2, HIGH);
      n2 = angle2 - currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    else if( currentAngle2 > angle2){
      digitalWrite(dirPin2, LOW);
      n2 = currentAngle2 - angle2;
      if( angle2 == 0) n2 = currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    moveXZ(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
  else if(( currentAngle != angle )&&( currentAngle2 == angle2 )){
    numstep2 = 0;
    if( currentAngle < angle){
      digitalWrite(dirPin1, HIGH);
      n = angle - currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    else if( currentAngle > angle){
      digitalWrite(dirPin1, LOW);
      n = currentAngle - angle;
      if( angle == 0) n = currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    moveXZ(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
  else if(( currentAngle != angle )&&( currentAngle2 != angle2 )){
    if( currentAngle < angle){
      digitalWrite(dirPin1,HIGH);
      n = angle - currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }
    else if( currentAngle > angle){
      digitalWrite(dirPin1,LOW);
      n = currentAngle - angle;
      if( angle == 0) n = currentAngle;
      numstep = (n / stepPerAngle) * 3;
    }  
    if( currentAngle2 < angle2){
      digitalWrite(dirPin2,HIGH);
      n2 = angle2 - currentAngle2;
      numstep2 = (n2 / stepPerAngle2) * 3;
    }
    else if( currentAngle2 > angle2){
        digitalWrite(dirPin2,LOW);
        n2 = currentAngle2 - angle2;
        if( angle2 == 0) n2 =currentAngle2;
        numstep2 = (n2 / stepPerAngle2) * 3;
    }
    moveXZ(numstep, stepPin1, numstep2, stepPin2);
    currentAngle = angle;
    currentAngle2 = angle2;
  }
  
}
