int const SL = 2;
int const CLK = 3;
int const SER = 4;

int incomingData;
int incomingDataOld;

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
  pinMode(CLK,OUTPUT);
  pinMode(SL,OUTPUT);
  pinMode(SER,INPUT);

  digitalWrite(SL,HIGH);
  digitalWrite(CLK,LOW);

  Serial.begin(9600);
}

void loop() {
  incomingData = myShiftIn(SER,CLK,SL);

  if (incomingData != incomingDataOld){
    Serial.println(incomingData,BIN);
  }
  incomingDataOld = incomingData;

}
