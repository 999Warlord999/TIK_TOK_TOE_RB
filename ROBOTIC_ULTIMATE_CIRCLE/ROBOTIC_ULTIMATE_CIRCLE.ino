#include<Servo.h>
Servo myServo;
//KHAI CHAN STEP1
const int stepPin1 = 6;
const int dirPin1  = 7;
//KHAI CHAN sTEP2
const int stepPin2 = 8;
const int dirPin2  = 9;

//KHAI BIEN STEP 1
int currentAngle = 0;
int angle = 0;
float stepPerAngle = 1.8/4 ; // full step = 1.8
int   numstep;

//KHAI BIEN STEP2
int currentAngle2 = 0;
int angle2 = 0;
float stepPerAngle2 = 1.8/4 ; // full step = 1.8
int   numstep2;

//KHAI BIEN CHO HAM VE
int arr[100] ;
int arr2[100] ;
uint8_t d = 60;
int delay_time_change = 0;

int arr3[100] ;
int arr4[100] ;

int arr5[100] ;
int arr6[100] ;

//CHE DO:
uint8_t state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT);
  pinMode(dirPin1,OUTPUT);

  pinMode(stepPin2,OUTPUT);
  pinMode(dirPin2,OUTPUT);
  myServo.attach(10);
  myServo.write(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
      char c = Serial.read();
      if(c == 'A'){
        state = 0;
        angle = 0;
        angle2 = 0;
        Move();
      }
      if(c == 'B'){
        state = 0;
        angle = 90;
        angle2 = 90;
        Move();
      }
      if(c == 'C'){
        state = 0;
        angle = 90;
        angle2 = 0;
        Move();
      }
      if(c == 'D'){
        state = 1;
        cal_circle();
         move_circle();
       
        state = 0;
        Move();
      }
      if(c == 'E'){
        state = 2;
        cal_cross1();
        move_cross1();
        state = 0;
        Move();
        state = 3;
        cal_cross2();
        move_cross2();
        state = 0;
        Move();
      }
    }
}

void cal_circle(){
  uint8_t i, i2;
  float t4,x,y,t2,t1;
  int r = 4;
  int x0 = 20;
  int y0 = 10,l1 = 25,l2 =20; 
  for(int t3 = 0; t3 <= 360; t3 += 20)
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

void cal_cross1(){
  uint8_t i, i2;
  float t4,x,y,t2,t1;
  int r = 4;
  int x0 = 25;
  int y0 = 0,l1 = 25,l2 =20; 
  for(int t = 0; t <= 10; t += 2)
  {
    //Serial.println(t4);
    x = 5+x0 - t;
    //Serial.println(x);
    y = 5+y0-t;
    t2 = acos((pow(x,2)+pow(y,2)-pow(l1,2)-pow(l2,2))/(2*l1*l2));
    //Serial.println(t2);
    t1 = atan2(y,x)-asin(l2*sin(t2)/(sqrt(pow(x,2)+pow(y,2))));
    angle2 = t2*180/PI;
    angle = -t1*180/PI;
    //    Serial.print("angle   ");
    //    Serial.print(angle);
    //    Serial.print("    angle2   ");
    //    Serial.println(angle2);

    arr3[i] = angle ; 
    arr4[i2] = angle2 ;
    //    Serial.println(arr[i]);
    i++;
    i2++;
  } 
}
void cal_cross2(){
  uint8_t i, i2;
  float t4,x,y,t2,t1;
  int x0 = 25;
  int y0 = 0,l1 = 25,l2 =20; 
  for(int t = 0; t <= 10; t += 2)
  {
    //Serial.println(t4);
    x = -5+x0 + t;
    //Serial.println(x);
    y = 5+y0-t;
    t2 = acos((pow(x,2)+pow(y,2)-pow(l1,2)-pow(l2,2))/(2*l1*l2));
    //Serial.println(t2);
    t1 = atan2(y,x)-asin(l2*sin(t2)/(sqrt(pow(x,2)+pow(y,2))));
    angle2 = t2*180/PI;
    angle = -t1*180/PI;
    //    Serial.print("angle   ");
    //    Serial.print(angle);
    //    Serial.print("    angle2   ");
    //    Serial.println(angle2);

    arr5[i] = angle ; 
    arr6[i2] = angle2 ;
    //    Serial.println(arr[i]);
    i++;
    i2++;
  }
}

void move_cross1() {
  int arr_length = 0;
  for (int i = 0; arr3[i] != 0; i++) arr_length++;
  for (uint8_t i3 = 0; i3 < arr_length; i3++){
    angle = arr3[i3];
    angle2 = arr4[i3];
    if (arr3[i3] == 0 || arr4[i3] == 0) break;
    Serial.print(arr3[i3]);
    Serial.print("--");
    Serial.println(arr2[i3]);
    if (i3 > arr_length * 1 / 5) delay_time_change += 900;
    if (i3 > arr_length * 3 / 5) delay_time_change += 1200;
    Move();
  }
  delay_time_change = 0;
}

void move_cross2() {
  int arr_length = 0;
  for (int i = 0; arr5[i] != 0; i++) arr_length++;
  for (uint8_t i3 = 0; i3 < arr_length; i3++){
    angle = arr5[i3];
    angle2 = arr6[i3];
    if (arr5[i3] == 0 || arr6[i3] == 0) break;
    Serial.print(arr5[i3]);
    Serial.print("--");
    Serial.println(arr6[i3]);
    if (i3 > arr_length * 1 / 5) delay_time_change += 800;
    if (i3 > arr_length * 3 / 5) delay_time_change += 1500;
    Move();
  }
  delay_time_change = 0;
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
    if (i3 > arr_length * 3 / 5) delay_time_change += 600;
    Move();
  }
  delay_time_change = 0;
}

void Move(){
  uint16_t n;
  uint16_t n2;
    if ((currentAngle==arr[0])&&(state == 1)||((currentAngle==arr3[0])&&(state == 2))||((currentAngle==arr5[0])&&(state == 3))){d=0; delay(1000);}
    if (state == 0){d = 60;}
    
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
    
   if (d == 0)
   {moveXZ_shape(numstep, stepPin1, numstep2, stepPin2);}
   else{moveXZ(numstep, stepPin1, numstep2, stepPin2);}
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
  int Delay = 7500;
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
    if (i > nStepMax * 2 / 3) Delay += 25;
    if (current_axis_min - steps_axis_min >= 1) {
      digitalWrite(stepPinMin, HIGH);
      pulseMin = 1;
      steps_axis_min++;
    }
    Serial.println(Delay);
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

void moveXZ_shape(long nStepX, int stepPinX, long nStepZ, int stepPinZ) {
 
  float nStepMax = nStepX;
  float nStepMin = nStepZ;
  int stepPinMax = stepPinX;
  int stepPinMin = stepPinZ;

  float current_axis_min = 0;
  long steps_axis_min = 0;
  float ratio_max_min = 0;
  int Delay = 8000 + delay_time_change;
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
    Serial.println(Delay);
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
