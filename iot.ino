
#include "thingProperties.h" //Cloud változók kezeléséhez
#include <Wire.h> //I2C prtokollhoz

void setup() {
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  Wire.begin(); //I2C indítás, masterként
}

void loop() {
  ArduinoCloud.update();

}


void onButtonUpChange()  {
  Wire.beginTransmission(0); //Adat küldése
  Wire.write(ButtonUp);
  Serial.println(1);
  Wire.endTransmission(); //Küldés zárása
}

void onButtonDownChange()  {
  Wire.beginTransmission(0); //Adat küldése
  Wire.write(ButtonDown+10);
  Serial.println(1);
  Wire.endTransmission(); //Küldés zárása
}

void onSwitchChange()  {
  Wire.beginTransmission(0); //Adat küldése
  Wire.write(Switch+20);
  Serial.println(1);
  Wire.endTransmission(); //Küldés zárása
}

void onColorChange()  {
  Wire.beginTransmission(1); //Adat küldése
  Wire.write("0");
  Wire.endTransmission(); //Küldés zárása
}
