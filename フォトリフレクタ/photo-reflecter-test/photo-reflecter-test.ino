#define LEFT 9
#define RIGHT 10

void setup(){
    Serial.begin(9600);
    pinMode(LEFT,INPUT);
    pinMode(RIGHT, INPUT);




}

void loop(){
    // if (digitalRead(PHOTO)==HIGH){
    //     Serial.print("HIGH \n");
    // }else{
    //     Serial.print("LOW \n");
    // }



    Serial.print(digitalRead(LEFT));
    Serial.println(digitalRead(RIGHT));
    

    // if (analogRead(PHOTO)>900){
    //     Serial.print("黒だよ！\n");
    // }else{
    //     Serial.print("白だよ！\n");
    // }

}
