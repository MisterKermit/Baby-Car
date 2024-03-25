//Joystick Setup
#define VRXPIN A0
#define VRYPIN A1
int xValue = 0;
int yValue = 0;

//Motor1 Setup
int motor1a = 2;
int motor1b = 3;

//Motor 2 Setup
int motor2a = 6;
int motor2b = 7;

//Linear Actuator Setup
int LinacA = 10;
int LinacB = 11;
int ENA = 12;
int LinacPosPin = A2;
int RealLinacPos;

//Button Setup
int ForwardButton = 22;
int RightButton = 23;
int LeftButton = 24;
int BackButton = 25;

int Forward;
int Right;
int Left;
int Back;

//Misc Setup
int Potentiometer = A3;

int xRestMin;
int xRestMax;
int LinacRestMax;
int LinacRestMin;
int LinacExtendMax;
int LinacExtendMin;

int yRestMin;
int yRestMax;
int ButtonMotorSpeed;
int i;


void setup() {
//Motors 
 pinMode(motor1a, OUTPUT);
 pinMode(motor1b, OUTPUT);
 pinMode(motor2a, OUTPUT);
 pinMode(motor2b, OUTPUT);

//Linear Actuator
 pinMode(LinacA, OUTPUT);
 pinMode(LinacB, OUTPUT);
 pinMode(LinacPosPin, INPUT);

//Button
 pinMode(ForwardButton, INPUT);
 pinMode(RightButton, INPUT);
 pinMode(LeftButton, INPUT);
 pinMode(BackButton, INPUT);

//Misc
 Potentiometer = 0;

 xRestMin = 500;
 xRestMax = 540;
 LinacRestMin = 502;
 LinacRestMax = 522;
 LinacExtendMax = 1020;
 LinacExtendMin = 4;

 yRestMin = 512;
 yRestMax = 650;
 i = 1;

 
 Serial.begin(9600);
}


void loop() {

//Changing Values
xValue = analogRead(VRXPIN);
yValue = analogRead(VRYPIN);
Serial.print("x = ");
Serial.print(xValue);
Serial.print(", y = ");
Serial.println(yValue);

int MaxMotorSpeed = map(analogRead(Potentiometer),0,1023,0,255);//last number value is max motor speed 
int NegativeyValue = abs(yValue - 612);
int PositiveyValue = (yValue - 512);

RealLinacPos = analogRead(LinacPosPin);
Serial.print("Linear Actuator Position:");
Serial.println(RealLinacPos);
//Serial.println(NegativeyValue);

Forward = digitalRead(ForwardButton);
Right = digitalRead(RightButton);
Left = digitalRead(LeftButton);
Back = digitalRead(BackButton);

//Joystick Motor Controls
if (yValue > yRestMax) {
   digitalWrite(motor1a, LOW);
   analogWrite(motor1b, map(PositiveyValue,0,512,0,MaxMotorSpeed));
   digitalWrite(motor2a, LOW);
   analogWrite(motor2b, map(PositiveyValue,0,512,0,MaxMotorSpeed));
} else if (yValue < yRestMin) {
     analogWrite(motor1a, map(NegativeyValue,0,512,0,MaxMotorSpeed));
     digitalWrite(motor1b, LOW);
     analogWrite(motor2a, map(NegativeyValue,0,512,0,MaxMotorSpeed));
     digitalWrite(motor2b, LOW);
     }

//Joystick Steering Control
if ((xValue < xRestMax) && (xValue > xRestMin) && (digitalRead(ForwardButton) == LOW) && (digitalRead(RightButton) == LOW) && (digitalRead(LeftButton) == LOW) && (digitalRead(BackButton) == LOW)) {
   if ((RealLinacPos < LinacRestMax) && (RealLinacPos > LinacRestMin)) {
   digitalWrite(LinacA, LOW);
   digitalWrite(LinacB, LOW);
   }
     else if (RealLinacPos < 512) {
     digitalWrite(LinacA, LOW);
     digitalWrite(LinacB, HIGH);
     analogWrite(ENA, 255);
   }
       else if (RealLinacPos > 512) {
       digitalWrite(LinacA, HIGH);
       digitalWrite(LinacB, LOW);
       analogWrite(ENA, 255);
   }                                        //End of Linear Actuator No Input Commands
} 
else if ((xValue > RealLinacPos) && (xValue > xRestMax)) {
   digitalWrite(LinacA, LOW);
   digitalWrite(LinacB, HIGH);
   analogWrite(ENA, 255);
   } 
    else if ((xValue < RealLinacPos) && (xValue < xRestMin)) {
    digitalWrite(LinacA, HIGH);
    digitalWrite(LinacB, LOW);
     analogWrite(ENA, 255);
     }


//Button Controls
if ((xValue < xRestMax) && (xValue > xRestMin) && (yValue < yRestMax) && (yValue > yRestMin)){ //Joystick Resting Value Ranges

//if ((digitalRead(ForwardButton) == HIGH) || (digitalRead(RightButton) == HIGH) || (digitalRead(LeftButton) == HIGH) || (digitalRead(BackButton) == HIGH)){
//for (int i = 1; i < MaxMotorSpeed; ++i) {
//    analogWrite(ButtonMotorSpeed, i);
//    delay(10);
//    Serial.println(i);
//  }

if (Forward == HIGH) {
   digitalWrite(motor1a, LOW);
   analogWrite(motor1b, ButtonMotorSpeed); //WIP
   digitalWrite(motor2a, LOW);
   analogWrite(motor2b, ButtonMotorSpeed);
}
  else if (Right == HIGH) {
     digitalWrite(motor1a, LOW);
     analogWrite(motor1b, ButtonMotorSpeed); //WIP
     digitalWrite(motor2a, LOW);
     analogWrite(motor2b, ButtonMotorSpeed);
     Serial.println("WORk");
   if (RealLinacPos < LinacExtendMax){
       digitalWrite(LinacA, LOW);
       digitalWrite(LinacB, HIGH);
       analogWrite(ENA, 255);
  }}
    else if (Left == HIGH) {
      digitalWrite(motor1a, LOW);
       analogWrite(motor1b, ButtonMotorSpeed); //WIP
      digitalWrite(motor2a, LOW);
       analogWrite(motor2b, ButtonMotorSpeed);
     if (RealLinacPos > LinacExtendMin){ 
         digitalWrite(LinacA, HIGH);
         digitalWrite(LinacB, LOW);
         analogWrite(ENA, 255);
    }}
       else if (Back == HIGH) {
         digitalWrite(motor1a, ButtonMotorSpeed);
         analogWrite(motor1b, LOW);
         digitalWrite(motor2a, ButtonMotorSpeed);
         analogWrite(motor2b, LOW);
      }
}
  else {
   digitalWrite(motor1a, LOW);
   digitalWrite(motor1b, LOW);
   digitalWrite(motor2a, LOW);
   digitalWrite(motor2b, LOW); 
   analogWrite(i, 1);
  }
//}

  delay(20);
  }
