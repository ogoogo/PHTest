#define PHOTO A5

void setup(){
    Serial.begin(9600);
    pinMode(PHOTO,INPUT);




}

void loop(){
    // if (digitalRead(PHOTO)==HIGH){
    //     Serial.print("HIGH \n");
    // }else{
    //     Serial.print("LOW \n");
    // }



    Serial.print(analogRead(PHOTO));
    Serial.print("\n");

    // if (analogRead(PHOTO)>900){
    //     Serial.print("黒だよ！\n");
    // }else{
    //     Serial.print("白だよ！\n");
    // }

}