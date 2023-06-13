#include <IRremote.h> //Könyvtár az infra kezeléséhez
#include <Wire.h> //I2C protokoll használatához

int BUTTON_UP=9; //Felfelé nyomógomb állapot érzékelése
int BUTTON_DOWN=8; //Lefelé nyomógomb állapot érzékelése
int UP=5; //Felfelé utasítás
int DOWN=12; //Lefelé utasítás
int MAX=10; //Max pozíció elérés figyelése
int MIN=11; //Min pozíció elérés figyelése
int SWITCH=7; //Üzemmód váltás
int INFRA = 6; //Infra jelek fogadása
int LIGHT=A0; //Analóg láb a fotorezisztív szenzorhoz
int mode_change=0; //Aktuális üzemmód

IRrecv irrecv(INFRA);
decode_results results;

void setup() {
  pinMode(BUTTON_UP, INPUT); //Felfelé nyomógomb állapot érzékelése
  pinMode(BUTTON_DOWN, INPUT); //Lefelé nyomógomb állapot érzékelése
  pinMode(UP, OUTPUT); //Felfelé utasítás
  pinMode(DOWN, OUTPUT); //Lefelé utasítás
  pinMode(MAX, INPUT); //Max pozíció elérés figyelése
  pinMode(MIN, INPUT); //Min pozíció elérés figyelése
  pinMode(SWITCH,INPUT); //Üzemmód váltás
  pinMode(LIGHT,INPUT); //Analóg láb a fotorezisztív szenzorhoz
  pinMode(INFRA,INPUT); //Infra jelek fogadása
  int pressed_up=0; //Felfelé nyomógomb állapot érzékelése
  int pressed_down=0; //Lefelé nyomógomb állapot érzékelése
  int max_position=0; //Max pozíció elérés figyelése
  int min_position=0; //Min pozíció elérés figyelése
  irrecv.enableIRIn();
  irrecv.blink13(true);
  Wire.begin(0);  // I2C indítás, slave 0 néven

}

void loop() {
  int pressed_up=digitalRead(BUTTON_UP); //Felfelé utasítás olvasása
  int pressed_down=digitalRead(BUTTON_DOWN); //Lefelé utasítás olvasása
  int max_position=digitalRead(MAX); //Max pozíció elérve olvasása
  int min_position=digitalRead(MIN); //Min pozíció elérve olvasása
  int mode=digitalRead(SWITCH); //Üzemmód váltás olvasása
  float light=analogRead(LIGHT); //Fénymennyiség olvasása
  Wire.onReceive(dataArrived); //Adat érkezésre adott függvényt hívja
  if(mode_change==0) //Egyes üzemmód, kézi működtetés
  {
        if (((pressed_up == LOW) || (pressed_down == LOW)) && !((pressed_up == LOW) && (pressed_down == LOW))) //XOR kettő kimenet bekapcsolásának kizárásához
        {
          if(pressed_up && !max_position) digitalWrite(UP, HIGH);
          if(pressed_down && !min_position) digitalWrite(DOWN, HIGH);
        }
        else
        {
          digitalWrite(UP, LOW);
          digitalWrite(DOWN, LOW); 
         }
  }
  else if(mode_change==1) //Kettes üzemmód, automatikus működtetés
  {
    if(light>500 && !min_position) //Érték felett megpróbálja csökkenteni a fénymennyiséget
    {
      digitalWrite(UP, HIGH);
    }
    else digitalWrite(UP, LOW);
    if(light<100 && !max_position) //Érték alatt megpróbálja növelni a fénymennyiséget
    {
      digitalWrite(DOWN, HIGH);
    }
    else digitalWrite(DOWN, LOW); 
  }
  else //Hármas üzemmód, infrás működtetés
  {
      digitalWrite(UP, LOW);
      digitalWrite(DOWN, LOW); 
      if (irrecv.decode(&results)){
        Serial.println(results.value);
        if(results.value==1095653798 && !max_position) //Felfelé mozgatás
        {
          digitalWrite(UP, HIGH);
          digitalWrite(DOWN, LOW); 
        }
        else if(results.value==1095651758 && !min_position) //Lefelé mozgatás
        {
          digitalWrite(DOWN, HIGH);
          digitalWrite(UP, LOW);
        }
        else //Más gombnál kikapcsolás
        {
          digitalWrite(UP, LOW);
          digitalWrite(DOWN, LOW); 
         }
        irrecv.resume();
      }
  }
}
