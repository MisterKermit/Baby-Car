//Joystick Setup
#define VRXPIN A0
#define VRYPIN A1
#define LinacPosPin A2

//Potentiometer
#define Potentiometer A3

//TODO: pound define all the pins for ultrasonic Trig and Echo
#define left_Trig 24
// #define right_Trig
// #define front_Trig
#define left_Echo 22
// #define right_Echo
// #define front_Echo

#define Ultra1Trig 52
#define Ultra2Trig 53

#define Ultra1Echo 50
#define Ultra2Echo 51

//Motors
#define motor1a 2
#define motor1b 3

#define motor2a 6
#define motor2b 7

//Button Pins
#define F_Button 35
#define R_Button 37
#define B_Button 41
#define L_Button 39

//linear actuator setup
#define LinacA 10
#define LinacB 11
#define ENA 12

//Kill switch plus led
#define StopLED 30
#define StopButton 29


//Stop switches for Ultrasonic and head array
#define UltrasoundButton 31
//TODO: define pin for HA
#define HA_Button 28



int xValue = 0;
int yValue = 0;

//Motor1 Setup
// int motor1a = 5;
// int motor1b = 4;

// Motor 2 Setup
// int motor2a = 6;
// int motor2b = 7;

//Linear Actuator Setup


int RealLinacPos;

//Ultrasonic Setup

int ProxFront;
long duration1;
int distance1;


int ProxBack;
long duration2;
int distance2;

//Baby Car 2 Head Array Ultrasonic Setup
//Todo: enter in values for pin locations in setup

// const int left_Trig, right_Trig, front_Trig;

// const int left_Echo, right_Echo, front_Echo;

unsigned long left_duration, right_duration, front_duration;

int left_distance, right_distance, front_distance;

bool prox_Left, prox_Right, prox_Front;

int distance_array[3];

int chosen_index; 

int StartingButtonSpeed, CurrentButtonSpeed, PreviousButtonSpeed, SpeedIncreaseIncrement;



//Misc Setup


//Killswitch Setup

int PreviousStopButtonState;
int CurrentStopButtonState;
int StopStatus;


int xRestMin;
int xRestMax;
int LinacRestMax;
int LinacRestMin;
int LinacExtendMax;
int LinacExtendMin;

int yRestMin;
int yRestMax;


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

  //Head array ultrasonic setup
  pinMode(left_Trig, OUTPUT);
  // pinMode(right_Trig, OUTPUT);
  // pinMode(front_Trig, OUTPUT);

  pinMode(left_Echo, INPUT);
  // pinMode(right_Echo, INPUT);
  // pinMode(front_Echo, INPUT);
  
  //Buttons 
  pinMode(F_Button, INPUT);
  pinMode(R_Button, INPUT);
  pinMode(L_Button, INPUT);
  pinMode(B_Button, INPUT);

  StartingButtonSpeed = 25;
  SpeedIncreaseIncrement = 2;

  //Misc
  pinMode(Potentiometer, INPUT);

  //Killswitch
  pinMode(StopButton, INPUT);
  pinMode(StopLED, OUTPUT);
  StopStatus = 0;

  xRestMin = 375;
  xRestMax = 455;
  LinacRestMin = 415;
  LinacRestMax = 425;
  LinacExtendMax = 1020;
  LinacExtendMin = 4;

  yRestMin = 512;
  yRestMax = 650;


  Serial.begin(9600);
}

int determine_distance(int TriggerPin, int EchoPin) {
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TriggerPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(TriggerPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  // Calculating the distance
  return (pulseIn(EchoPin, HIGH) * 0.034) / 2;
}

int find_index(int array[], int value) {
  int index = 0;
  for (int i; i < sizeof(array); i++) {
    if (array[i] == value) {
      if (array[i] <= 35) {
        return index;
      }
    }
    index += 1;
  }
  return 3;
}

void loop() {

  //Changing Values
  xValue = map(analogRead(VRXPIN), 0, 1023, 12, 830);
  yValue = analogRead(VRYPIN);
  //  Serial.print("x = ");
  //  Serial.println(xValue);
  // Serial.print(", y = ");
  // Serial.println(yValue);

  int MaxMotorSpeed = map(analogRead(Potentiometer), 0, 1023, 25, 150);  //last number value is max motor speed
  Serial.println(MaxMotorSpeed);
  int NegativeyValue = abs(yValue - 612);
  int PositiveyValue = (yValue - 512);

  //Button maxspeed
  PreviousButtonSpeed = CurrentButtonSpeed;
  
  //Serial.println(ProxFront);

  RealLinacPos = analogRead(LinacPosPin);
  //Serial.print("Linear Actuator Position:");
  // Serial.println(RealLinacPos);
  //Serial.println(NegativeyValue);

  //Motor Controls
  if ((yValue > yRestMax) && (ProxFront == 0) && (StopStatus == 0)) {
    digitalWrite(motor1b, LOW);
    analogWrite(motor1a, map(PositiveyValue, 0, 512, 0, MaxMotorSpeed));
  } else if ((yValue < yRestMin) && (ProxBack == 0) && (StopStatus == 0)) {
    analogWrite(motor1b, map(NegativeyValue, 0, 612, 0, MaxMotorSpeed));
    digitalWrite(motor1a, LOW);
  } else {
    digitalWrite(motor1a, LOW);
    digitalWrite(motor1b, LOW);
    digitalWrite(motor2a, LOW);
    digitalWrite(motor2b, LOW);
  }

  //Button Motor control + Head Array
  if ((digitalRead(F_Button) == HIGH || digitalRead(R_Button) == HIGH || digitalRead(L_Button) == HIGH) && (StopStatus == 0) && ProxFront == 0) {
    analogWrite(motor1a, CurrentButtonSpeed);
    digitalWrite(motor1b, LOW);
    if (CurrentButtonSpeed < MaxMotorSpeed) {
      CurrentButtonSpeed = PreviousButtonSpeed + SpeedIncreaseIncrement;
    }
  } else if ((digitalRead(B_Button) == HIGH) && (StopStatus == 0) && ProxBack == 0) {
    digitalWrite(motor1a, LOW);
    analogWrite(motor1b, CurrentButtonSpeed);
    if (CurrentButtonSpeed < MaxMotorSpeed) {
      CurrentButtonSpeed = PreviousButtonSpeed + SpeedIncreaseIncrement;
    }
  } else if (prox_Front) {
    analogWrite(motor1a, CurrentButtonSpeed);
    digitalWrite(motor1b, LOW);
    if (CurrentButtonSpeed < MaxMotorSpeed) {
      CurrentButtonSpeed = PreviousButtonSpeed + SpeedIncreaseIncrement;
    }
  } else {
    digitalWrite(motor1a, LOW);
    digitalWrite(motor1b, LOW);
    CurrentButtonSpeed = StartingButtonSpeed;
  }

  //Steering Controls + Head Array
  if ((xValue < xRestMax) && (xValue > xRestMin) && (StopStatus == 0)) {
    if ((RealLinacPos < LinacRestMax) && (RealLinacPos > LinacRestMin)) {
      digitalWrite(LinacA, LOW);
      digitalWrite(LinacB, LOW);
    } else if (RealLinacPos < LinacRestMin) {
      digitalWrite(LinacA, LOW);
      digitalWrite(LinacB, HIGH);
      analogWrite(ENA, 255);
    } else if (RealLinacPos > LinacRestMax) {
      digitalWrite(LinacA, HIGH);
      digitalWrite(LinacB, LOW);
      analogWrite(ENA, 255);
    }
  }
  //End of Linear Actuator No Input Commands
  else if ((xValue > RealLinacPos) && (xValue > xRestMax) && (StopStatus == 0)) {
    digitalWrite(LinacA, HIGH);
    digitalWrite(LinacB, LOW);
  } else if ((xValue < RealLinacPos) && (xValue < xRestMin) && (StopStatus == 0)) {
    digitalWrite(LinacA, LOW);
    digitalWrite(LinacB, HIGH);
  } else if ((digitalRead(R_Button) == HIGH) && (LinacExtendMax > RealLinacPos) && (StopStatus == 0)) {
    digitalWrite(LinacA, HIGH);
    digitalWrite(LinacB, LOW);
  } else if ((digitalRead(L_Button) == HIGH) && (LinacExtendMax > RealLinacPos) && (StopStatus == 0)) {
    digitalWrite(LinacA, LOW);
    digitalWrite(LinacB, HIGH);
  } else if (prox_Left && (LinacExtendMax > RealLinacPos) && (StopStatus == 0)) {
    digitalWrite(LinacA, LOW);
    digitalWrite(LinacB, HIGH);
  } else if (prox_Right && (LinacExtendMax > RealLinacPos) && (StopStatus == 0)) {
    digitalWrite(LinacA, HIGH);
    digitalWrite(LinacB, LOW);
  } else {
    digitalWrite(LinacA, LOW);
    digitalWrite(LinacB, LOW);
  } 

  //Ultrasound Sensors
  //if (UltrasoundButton == 1){
  // digitalWrite(TriggerPin, LOW);
  // //digitalWrite(Ultra2Trig, LOW);
  // delayMicroseconds(2);
  // // Sets the trigPin on HIGH state for 10 micro seconds


  // digitalWrite(TriggerPin, HIGH);
  // //digitalWrite(Ultra2Trig, HIGH);
  // delayMicroseconds(10);

  // digitalWrite(TriggerPin, LOW);
  //digitalWrite(Ultra2Trig, LOW);

  // duration1 = pulseIn(Ultra1Echo, HIGH);
  //duration2 = pulseIn(Ultra2Echo, HIGH);
  // Reads the echoPin, returns the sound wave travel time in microseconds

  // Calculating the distance
  // distance1 = determine_distance(Ultra1Trig, Ultra1Echo); 
  distance2 = determine_distance(Ultra2Trig, Ultra2Echo);
  // Serial.println(distance2);

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
  
  //calculate distances of all 3 sensors
  // distance_array[0] = determine_distance(left_Trig, left_Echo);
  // Serial.println(distance_array[0]);
  // distance_array[1] = determine_distance(right_Trig, right_Echo);
  // distance_array[2] = determine_distance(front_Trig, front_Echo);


  // chosen_index = find_index(distance_array, max(max(distance_array[0], distance_array[1]), distance_array[2]));
  // Serial.println(max(max(distance_array[0], distance_array[1]), distance_array[2]));
  // Serial.println(chosen_index);
  if (digitalRead(HA_Button) == 1) {
    switch(chosen_index) {
      case 0:
        if ((distance_array[0] <= 5)) {
          prox_Left = true;
        }

        break;
      case 1:
      if ((distance_array[1] <= 5) && (digitalRead(UltrasoundButton) == 1)) {
          prox_Right = true;
      }
      break;
      case 2:
        if ((distance_array[2] <= 5) && (digitalRead(UltrasoundButton) == 1)) {
          prox_Front = true;
        }
        break;
      default:
        prox_Left = prox_Right = prox_Front = false;
        break;
    }
  }

  // Serial.println(prox_Right);
  // if (prox_Left) {
  //   Serial.println("Left detect");
  // } else {
  //   Serial.println("No Left");
  // }
  // Serial.println(prox_Left);
  // Serial.println(prox_Front);
    
  if ((distance1 <= 35) && (digitalRead(UltrasoundButton) == 1)) {
    ProxFront = 1;
  } else {
    ProxFront = 0;
  }

  if ((distance2 <= 45) && (digitalRead(UltrasoundButton) == 1)) {
    ProxBack = 1;
  } else {
    ProxBack = 0;
  }



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
}

