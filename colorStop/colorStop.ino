#include <Wire.h>

#define S11059_Addr 0x2A
#define RegisterCall 0x00
#define RegisterWrite1 0x89
#define RegisterWrite2 0x09
#define SensorRegister 0x03

#define RIGHT  10
#define LEFT  9
#define PR 5
#define ER 6
#define PL 7
#define EL 8
#define refDelayTime 3

#define DIN_PIN 13

int  Steps2Take;
int stepCounter = 0;
bool dropped = true;

int high_byte, low_byte, red, green, blue, IR;


  
void setup() {
  Serial.begin(9600);
  
  pinMode(RIGHT,INPUT);
  pinMode(LEFT,INPUT);
  pinMode(PR,OUTPUT);
  pinMode(ER,OUTPUT);
  pinMode(PL,OUTPUT);
  pinMode(EL,OUTPUT);
  pinMode(DIN_PIN, INPUT_PULLUP);

    Wire.begin();

  Wire.beginTransmission(S11059_Addr);
  Wire.write(RegisterCall);
  Wire.write(RegisterWrite1);
  Wire.endTransmission();

  Wire.beginTransmission(S11059_Addr);
  Wire.write(RegisterCall);
  Wire.write(RegisterWrite2);
  Wire.endTransmission();
  
}

void loop() {

    colorcensor();

    Serial.print(red);
    Serial.print(",");
    Serial.print(green);
    Serial.print(",");
    Serial.print(blue);
    Serial.print(",");
    Serial.print(IR);
    Serial.println("");

    if((red<20 && green<35 && blue<20) | (red>85 && green>135 && blue>105)){
        Serial.println("black or white");

        if ((digitalRead(RIGHT) == HIGH)&&(digitalRead(LEFT) == HIGH)) {
    
            digitalWrite(PR,HIGH);
            analogWrite(ER,200);
            digitalWrite(PL,HIGH);
            analogWrite(EL,200);
            
            } else if ((digitalRead(RIGHT) == LOW)&&(digitalRead(LEFT) == HIGH)){
            
            digitalWrite(PR,HIGH);
            analogWrite(ER,200);
            digitalWrite(PL,HIGH);
            analogWrite(EL,0);
            
            } else if ((digitalRead(RIGHT) == HIGH)&&(digitalRead(LEFT) == LOW)){
            
            digitalWrite(PR,HIGH);
            analogWrite(ER,0);
            digitalWrite(PL,HIGH);
            analogWrite(EL,200);
            
            } else {
            
            digitalWrite(PR,LOW);
            analogWrite(ER,0);
            digitalWrite(PL,LOW);
            analogWrite(EL,0);
            
        }

  

    }else{
        Serial.println("色あるよ！！");
            digitalWrite(PR,LOW);
            analogWrite(ER,0);
            digitalWrite(PL,LOW);
            analogWrite(EL,0);

    }
}

    



  
    


void colorcensor(){

  delay(500);

  Wire.beginTransmission(S11059_Addr);
  Wire.write(SensorRegister);
  Wire.endTransmission();

  Wire.requestFrom(S11059_Addr,8);
  if (Wire.available()){
    high_byte = Wire.read();
    low_byte = Wire.read();
    red = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    green = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    blue = high_byte << 8 | low_byte;

    high_byte = Wire.read();
    low_byte = Wire.read();
    IR = high_byte << 8 | low_byte;
  }
  Wire.endTransmission();


}
