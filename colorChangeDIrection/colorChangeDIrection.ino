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

int const SL = 2;
int const CLK = 3;
int const SER = 4;

int incomingData;
int incomingDataOld;

bool front=false;
bool right=false;
bool back=false;
bool left=false;

/* シフトレジスタから8ビットのデータを受け取るための関数 */
byte myShiftIn(int dataPin, int clockPin, int loadPin){
  byte data;

  digitalWrite(loadPin,LOW); //データを格納
  digitalWrite(loadPin,HIGH); //データを確定

  data = digitalRead(dataPin); //H読み取り

  for (int i=1; i<8; i++){
    digitalWrite(clockPin,HIGH); //クロック立ち上がり
    data = data << 1 | (digitalRead(dataPin)); //GからAを順に
    digitalWrite(clockPin,LOW); //クロック立ち下がり
  }

  return data;
}


  
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

  pinMode(CLK,OUTPUT);
  pinMode(SL,OUTPUT);
  pinMode(SER,INPUT);

  digitalWrite(SL,HIGH);
  digitalWrite(CLK,LOW);
  
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
      //mess:一つ目が黒の条件、二つ目が白の条件(チューニングでここ変える)
        Serial.println("black or white");

        if ((digitalRead(RIGHT) == HIGH)&&(digitalRead(LEFT) == HIGH)) {
    
            digitalWrite(PR,HIGH);
            analogWrite(ER,240);
            digitalWrite(PL,HIGH);
            analogWrite(EL,180);
            
            } else if ((digitalRead(RIGHT) == LOW)&&(digitalRead(LEFT) == HIGH)){
            
            digitalWrite(PR,HIGH);
            analogWrite(ER,240);
            digitalWrite(PL,HIGH);
            analogWrite(EL,0);
            
            } else if ((digitalRead(RIGHT) == HIGH)&&(digitalRead(LEFT) == LOW)){
            
            digitalWrite(PR,HIGH);
            analogWrite(ER,0);
            digitalWrite(PL,HIGH);
            analogWrite(EL,180);
            
            } else {
            
            digitalWrite(PR,HIGH);
            analogWrite(ER,0);
            digitalWrite(PL,HIGH);
            analogWrite(EL,0);
            
        }

      }else{
        Serial.println("色あるよ！！");
        digitalWrite(PR,LOW);
        analogWrite(ER,0);
        digitalWrite(PL,LOW);
        analogWrite(EL,0);

        Serial.println("中心まで進むよ");
        goStraightALittle(1000);
        //mess:ここの秒数調節必要

        stop();

        delay(10000);
        //mess:これは中心までの秒数を調節するためにわかりやすくするための10秒

        findWay();



    }
}

void findWay(){
  incomingData = myShiftIn(SER,CLK,SL);

  if (incomingData != incomingDataOld){
    Serial.println(incomingData,BIN);
  }
  incomingDataOld = incomingData;

  int first = incomingData%2;
  int second = (incomingData/2)%2;
  int third = (incomingData/4)%2;
  int fourth = (incomingData/8)%2;

  Serial.println("後ろのタイヤのところまで前進するよ");
  goStraightALittle(3000);
  //mess:秒数調整

  
//mess:下の3つも秒数調整(曲がってから分岐抜け出すための直進)
  if (fourth==1){
      turn(1);

      goStraightALittle(1000);
  }else if(first==1){
      goStraightALittle(1000);
  
      
  }else if(second==1){
      turn(3);
      goStraightALittle(1000);
  }else{
  
    uTurn();

  }

}

void turn(int u)
{
    // 1だったら右に、3だったら左に回転
    if (u == 1)
    {
        Serial.println("右に曲がるよ");
        digitalWrite(PR, LOW);
        digitalWrite(ER, 200);
        digitalWrite(PL, HIGH);
        digitalWrite(EL, 200);
        delay(5600);
        
    }
    else if (u == 3)
    {
        Serial.println("左に曲がるよ");
        digitalWrite(PR, HIGH);
        digitalWrite(ER, 200);
        digitalWrite(PL, LOW);
        digitalWrite(EL, 200);
        delay(2800);
        
    }
}

void uTurn()
{
    // 180度回転
    Serial.println("回れ右するよ");
        digitalWrite(PR, LOW);
        digitalWrite(ER, 200);
        digitalWrite(PL, HIGH);
        digitalWrite(EL, 200);
        delay(5600);
        //mess:秒数調整
        
}
void goStraightALittle(int s)
//mess:秒数引数に入れれるようにしてます
{
    //少し進めるだけ
    Serial.println("少し進むよ");
        digitalWrite(PR, HIGH);
        digitalWrite(ER, 200);
        digitalWrite(PL, HIGH);
        digitalWrite(EL, 200);
        delay(s);
}
void stop(){
    digitalWrite(PR, HIGH);
    digitalWrite(ER, 0);
    digitalWrite(PL, HIGH);
    digitalWrite(EL, 0);
  
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
