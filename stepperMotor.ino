// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
#define commonAnode true

byte gammatable[256];

#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
#define aTog 2



int Steps = 0;
int x = 0;
int dir = 1;
int countF = 0;
int countB = 0;
int count = 0;
int cSensor = 1;

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(aTog,INPUT);


  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

   // use these three pins to drive an LED
#if defined(ARDUINO_ARCH_ESP32)
  ledcAttachPin(redpin, 1);
  ledcSetup(1, 12000, 8);
  ledcAttachPin(greenpin, 2);
  ledcSetup(2, 12000, 8);
  ledcAttachPin(bluepin, 3);
  ledcSetup(3, 12000, 8);
#else
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
#endif

for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
}
void loop()
{
  float red, green, blue;
  
  tcs.setInterrupt(false);  // turn on LED

  delay(120);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  //tcs.setInterrupt(true);  // turn off LED

  if (int(red)>200) {
    cSensor = 1;
  } else if (int(red) < 80) {
    cSensor = 2;
  } else {
    cSensor = 3;
  }    

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
        
  if (cSensor == 1) {
    lcd.setRGB(255, 0, 0);
    // Print a message to the LCD.
    lcd.println("Go Away You              ");
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print("Fat Fuck");
    delay(500);
  }else if (cSensor == 2) {
    lcd.setRGB(0, 255, 0);
    // Print a message to the LCD.
    lcd.println("Good Kitty!!              ");
    lcd.setCursor(0, 1);
  while (count != 800) {
    if (dir == 1){
    stepper(x);
    x++;
    if (x>3){
      x=0;
    }
  }
  else if (dir == -1){
    stepper (x); 
    x--;
    if (x <0){
      x=3;
    }
  }

  if (countF == 400){
    dir *= -1;
    countF = 0;
  } else if (countB == 400) {
    dir *=1;
  }
  count++;
  countF++;
  if (count >= 400) {
    countB++;
  }

  delay (20);
  } 
count = 0;
countB = 0;
    // print the number of seconds since reset:
    lcd.print("                             ");
  }else if (cSensor == 3) {
    // Print a message to the LCD.
    lcd.println("                        ");
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print("                          ");
  }

#if defined(ARDUINO_ARCH_ESP32)
  ledcWrite(1, gammatable[(int)red]);
  ledcWrite(2, gammatable[(int)green]);
  ledcWrite(3, gammatable[(int)blue]);
#else
  analogWrite(redpin, gammatable[(int)red]);
  analogWrite(greenpin, gammatable[(int)green]);
  analogWrite(bluepin, gammatable[(int)blue]);
#endif
Serial.println("something else");
  
//  float voltage = sensorValue * (5.0 / 1023.0);
//  int turns = floor(voltage * max / 3.5);
//
//  if (turns != prevturns) {
//    for (int i = 0; i < abs(turns - prevturns)/2; i++) {
//      if (turns - prevturns > 0) {
//        steps_left = 4095;
//        Direction = true;
//        while (steps_left > 0) {
//          currentMillis = micros();
//          if (currentMillis - last_time >= 3920) {
//            stepper(1);
//            last_time = micros();
//            steps_left--;
//          }
//        }
//      }
//      else {
//        steps_left = 4095;
//        Direction = false;
//        while (steps_left > 0) {
//          currentMillis = micros();
//          if (currentMillis - last_time >= 3920) {
//            stepper(1);
//            last_time = micros();
//            steps_left--;
//          }
//        }
//      }
//    }
//  }
//  prevturns = turns;
//  delay(3000);
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);

}

void stepper(int Steps) {
  switch (Steps) {
    case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      break;
    case 1:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case 2:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
//    case 4:
//      digitalWrite(IN1, LOW);
//      digitalWrite(IN2, HIGH);
//      digitalWrite(IN3, LOW);
//      digitalWrite(IN4, LOW);
//      break;
//    case 5:
//      digitalWrite(IN1, HIGH);
//      digitalWrite(IN2, HIGH);
//      digitalWrite(IN3, LOW);
//      digitalWrite(IN4, LOW);
//      break;
//    case 6:
//      digitalWrite(IN1, HIGH);
//      digitalWrite(IN2, LOW);
//      digitalWrite(IN3, LOW);
//      digitalWrite(IN4, LOW);
//      break;
//    case 7:
//      digitalWrite(IN1, HIGH);
//      digitalWrite(IN2, LOW);
//      digitalWrite(IN3, LOW);
//      digitalWrite(IN4, HIGH);
//      break;
    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
  }
//  SetDirection();
}
//void SetDirection() {
//  if (Direction == true) {
//    Steps++;
//  }
//  if (Direction == false) {
//    Steps--;
//  }
//  if (Steps > 3) {
//    Steps = 0;
//  }
//  if (Steps < 0) {
//    Steps = 3;
//  }
//}
