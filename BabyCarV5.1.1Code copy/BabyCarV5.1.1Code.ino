
//Joystick Setup
#define VRXPIN A0
#define VRYPIN A1

#define motor1a 5
#define motor1b 4

#define motor2a 6
#define motor2b 7

#define LinacA 10
#define LinacB 11
#define LinacPosPin A2
#define ENA 12


//back and front proximity sensors
#define Ultra1Trig 52
#define Ultra1Echo 50

#define Ultra2Trig 53
#define Ultra2Echo 51

//Head array sensors
// #define ultra_left_trig 
// #define ultra_left_echo

// #define ultra_right_trig
// #define ultra_right_echo

// #define ultra_fender_trig
// #define ultra_right_echo

//control panel
#define UltrasoundButton 36 //ultra toggle
#define Potentiometer A3//motor speed
//kill switch
#define StopButton 35
#define StopLED 37

int xValue = 0;
int yValue = 0;

//Motor1 Setup
// int motor1a = 5;
// int motor1b = 4;

// //Motor 2 Setup
// int motor2a = 6;
// int motor2b = 7;

// //Linear Actuator Setup
// int LinacA = 10;
// int LinacB = 11;
// int ENA = 12;
// int LinacPosPin = A2;
int RealLinacPos;

//Ultrasonic Setup


// int Ultra1Trig = 52;
// int Ultra1Echo = 50;
int ProxFront;
long duration1;
int distance1;

// int Ultra2Trig = 53;
// int Ultra2Echo = 51;
int ProxBack;
long duration2;
int distance2;

int ForwardButton = 35;
int RightButton = 37;
int LeftButton = 39;
int BackButton = 41;

int Forward;
int Right;
int Left;
int Back;

int ButtonMotorSpeed;
int i;

//change values
// int ultra_left_trig = 
// int ultra_left_echo = 

// int ultra_right_trig =
// int ultra_right_echo = 

// int ultra_fender_trig = 
// int ultra_fender_echo = 


// int UltrasoundButton = 36;

//Misc Setup
// int Potentiometer = A3;

//Killswitch Setup
// int StopButton = 35;
int PreviousStopButtonState;
int CurrentStopButtonState;
int StopStatus;
// int StopLED = 37;

int xRestMin;
int xRestMax;
int LinacRestMax;
int LinacRestMin;
int LinacExtendMax;
int LinacExtendMin;

int yRestMin;
int yRestMax;
  

class Ultrasound {
  private:
    byte echo_pin;
    byte trigger_pin;
    float duration;
    float distance;
  public:
    Ultrasound(byte e_pin, byte t_pin) {
      this->echo_pin = e_pin;
      this->trigger_pin = t_pin;
      init();
    }

    void init() {
      pinMode(echo_pin, OUTPUT);
      pinMode(trigger_pin, INPUT);
      update();
    }
      
    void update() {
      digitalWrite(trigger_pin, LOW);
      //digitalWrite(Ultra2Trig, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds


      digitalWrite(trigger_pin, HIGH);
      //digitalWrite(Ultra2Trig, HIGH);
      delayMicroseconds(10);

      digitalWrite(trigger_pin, LOW);
      //digitalWrite(Ultra2Trig, LOW);

      duration = pulseIn(echo_pin, HIGH);
      //duration2 = pulseIn(Ultra2Echo, HIGH);
      // Reads the echoPin, returns the sound wave travel time in microseconds

      // Calculating the distance
      distance = duration * 0.034 / 2;
    }

    float get_distance() {
      update();
      return distance;
    }

    bool proximity_fb() {
      update();
      return distance < 40;
    } 
  };

Ultrasound ultra_front(Ultra1Echo, Ultra1Trig);
Ultrasound ultra_back(Ultra2Echo, Ultra2Trig);

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

  //Ultrasound Sensors
  pinMode(Ultra1Trig, OUTPUT);
  pinMode(Ultra1Echo, INPUT);
  pinMode(Ultra2Trig, OUTPUT);
  pinMode(Ultra2Echo, INPUT);
  pinMode(UltrasoundButton, INPUT);
  ProxFront = 0;
  ProxBack = 0;

  //Misc
  pinMode(Potentiometer, INPUT);

  //Killswitch
  pinMode(StopButton, INPUT);
  pinMode(StopLED, OUTPUT);
  StopStatus = 0;

  //Buttons
  pinMode(ForwardButton, INPUT);
  pinMode(RightButton, INPUT);
  pinMode(LeftButton, INPUT);
  pinMode(BackButton, INPUT);

  xRestMin = 375;
  xRestMax = 455;
  LinacRestMin = 415;
  LinacRestMax = 425;
  LinacExtendMax = 1020;
  LinacExtendMin = 4;

 

  // Ultrasound my_ultrasounds[] = {ultra_front, ultra_back};

  yRestMin = 512;
  yRestMax = 650;


  Serial.begin(9600);
}

int get_distance(Ultrasound sensor) {
  // float ultra_distance;
  if (digitalRead(UltrasoundButton) == 1) {
    if (sensor.proximity_fb()) {
      if (i == 0) {
        ProxFront = 1;
        return ProxFront;
      } else {
        ProxBack = 1;
        return ProxBack;
      }
    } else {
      if (i == 0) {
        ProxFront = 0;
        return ProxFront;
      } else {
        ProxBack = 0;
        return ProxBack;
      }
    }
  } else {
    return 0;
  }
}

void loop() {

  //Changing Values
  xValue = map(analogRead(VRXPIN), 0, 1023, 12, 830);
  yValue = analogRead(VRYPIN);
//  Serial.print("x = ");
//  Serial.println(xValue);
  // Serial.print(", y = ");
  // Serial.println(yValue);

  int MaxMotorSpeed = map(analogRead(Potentiometer), 0, 1023, 25, 255); //last number value is max motor speed
  int NegativeyValue = abs(yValue - 612);
  int PositiveyValue = (yValue - 512);
  //Serial.println(ProxFront);

  RealLinacPos = analogRead(LinacPosPin);
  //Serial.print("Linear Actuator Position:");
  // Serial.println(RealLinacPos);
  //Serial.println(NegativeyValue);
  Forward = digitalRead(ForwardButton);
  Right = digitalRead(RightButton);
  Left = digitalRead(LeftButton);
  Back = digitalRead(BackButton);

  // Serial.println(Forward);

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

  //Joystick Steering Controls
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

                                       //End of Linear Actuator No Input Commands
// else if ((xValue > RealLinacPos) && (xValue > xRestMax) && (StopStatus == 0)) {
//   digitalWrite(LinacA, HIGH);
//   digitalWrite(LinacB, LOW);
// }
// else if ((xValue < RealLinacPos) && (xValue < xRestMin) && (StopStatus == 0)) {
//   digitalWrite(LinacA, LOW);
//   digitalWrite(LinacB, HIGH);
// }
// else {
//   digitalWrite(LinacA, LOW);
//   digitalWrite(LinacB, LOW);
// }

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
//Ultrasound Sensors
// //if (UltrasoundButton == 1){
// digitalWrite(Ultra1Trig, LOW);
// //digitalWrite(Ultra2Trig, LOW);
// delayMicroseconds(2);
// // Sets the trigPin on HIGH state for 10 micro seconds


// digitalWrite(Ultra1Trig, HIGH);
//digitalWrite(Ultra2Trig, HIGH);
// delayMicroseconds(10);

// digitalWrite(Ultra1Trig, LOW);
// //digitalWrite(Ultra2Trig, LOW);

// duration1 = pulseIn(Ultra1Echo, HIGH);
// //duration2 = pulseIn(Ultra2Echo, HIGH);
// // Reads the echoPin, returns the sound wave travel time in microseconds

// // Calculating the distance
// distance1 = duration1 * 0.034 / 2;
//distance2 = duration2 * 0.034 / 2;
// Prints the distance on the Serial Monitor
//  Serial.print("Distance1: ");
//  Serial.println(distance1);


// digitalWrite(Ultra2Trig, LOW);
// delayMicroseconds(2);
// // Sets the trigPin on HIGH state for 10 micro seconds



// digitalWrite(Ultra2Trig, HIGH);
// delayMicroseconds(10);


// digitalWrite(Ultra2Trig, LOW);


// duration2 = pulseIn(Ultra2Echo, HIGH);
// // Reads the echoPin, returns the sound wave travel time in microseconds

// // Calculating the distance

// distance2 = duration2 * 0.034 / 2;
// Prints the distance on the Serial Monitor
//  Serial.print("Distance1: ");
//  Serial.println(distance1);

// if ((distance1 <= 35) && (digitalRead(UltrasoundButton) == 1)) {
//   ProxFront = 1;
// } else {
//   ProxFront = 0;
// }

// if ((distance2 <= 45) && (digitalRead(UltrasoundButton) == 1)) {
//   ProxBack = 1;
// } else {
//   ProxBack = 0;
// }

//change these which one is which


//head array
// Ultrasound ultra_left(ultra_left_echo, ultra_left_trig);
// Ultrasound ultra_right(ultra_right_echo, ultra_right_trig);
// Ultrasound ultra_fender(ultra_fender_echo, ultra_fender_trig);

// Ultrasound my_ultrasounds[] = {ultra_front, ultra_back, ultra_left, ultra_right, ultra_fender};


ProxFront = get_distance(ultra_front);
ProxBack = get_distance(ultra_back);

Serial.println(digitalRead(UltrasoundButton));


// float distance_values[];

// for (int j = 0; j < 4; j++) {
//   distance_values[j] = my_ultrasounds[j].get_distance();
// }

// float distance_chosen = max(max(distance_values[0], distance_values[1]), distance_values[2]);

//put threshold
// if (distance_chosen > threshold && StopStatus == 0 && UltrasoundButton == 1) {
//   switch(distance_chosen) {
//     case distance_values[0]:
//       Serial.println("lfet");
//       //motor left
//     case distance_values[1]:
//       //motor right
//       Serial.println("right");
//     case distance_values[2]:
//       //motor fender
//       Serial.println("fnder");
//   break;
//   }
// }


//Killswitch
PreviousStopButtonState = CurrentStopButtonState;
CurrentStopButtonState = (digitalRead(StopButton));

if ((CurrentStopButtonState == 1) && (PreviousStopButtonState == 0)) {
  if (StopStatus == 0) {
    StopStatus = 1;
  } else if (StopStatus == 1) {
    StopStatus = 0;
  }
}
if (StopStatus == 1) {
  digitalWrite(StopLED, HIGH);
} else if (StopStatus == 0) {
  digitalWrite(StopLED, LOW);
}


// Serial.println(duration1);
  delay(20);
}

