#include <EEPROM.h>

void setup(){
    Serial.begin(9600);
    
}

void loop(){

    // for (int i=0 ; i<10 ;i++){
    //     EEPROM.write(i,i);
    //     delay(10000);
    // }
    // Serial.print("Finished!");

    for (int i=0 ; i<10 ;i++){
        Serial.print(EEPROM.read(i));
    }
    delay(100000000);
}
