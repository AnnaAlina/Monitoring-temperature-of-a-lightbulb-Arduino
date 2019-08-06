#define THERMISTOR A1
#define NUMSAMPLES 5
#define THERMROOM 25
#define ROOMRESIST 10000
#define BETA 3960
#define SERIESR 4700

#include <LiquidCrystal.h>
#include <TimerOne.h>

LiquidCrystal lcd(10,9,5,4,3,2);  //define object called lcd

uint16_t samples[NUMSAMPLES];
int mocPin = 6;
int red = 11;
int yellow = 12;
int green = 13;

unsigned long seconds;
float temperature;

void setup(void) {
  lcd.begin(16,2);  //starts lcd screen
  lcd.clear();      //clears screen
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(mocPin, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(counter);
}

void loop() {
  uint8_t i;
  float number;

  for(i = 0; i < NUMSAMPLES; i++){          //for loop to get 5 analog samples
    samples[i] = analogRead(THERMISTOR);    //assigns value to each position within array
    delay(10);                              //waits
  }

  number = 0;
  for(i=0; i < NUMSAMPLES; i++){
    number += samples[i];
    }
  number /= NUMSAMPLES;

  number = 1023 / number - 1;
  number = SERIESR/number;

  temperature = number / ROOMRESIST;
  temperature = log(temperature);
  temperature /= BETA;
  temperature += 1.0 / (THERMROOM + 273.15);
  temperature = 1.0/temperature;
  temperature -= 273.15;

  int setpoint = 65;
  int err;
  int Gain = 20;
  int dutyCycle;
  int onTime;
  int offTime;

  err = setpoint - temperature;
  dutyCycle = err*Gain;
  dutyCycle = constrain(dutyCycle, 0, 200);
  onTime = map(dutyCycle, 0, 200, 0, 4000);  //changes to delay onTime to 0-4000
  offTime= map(onTime, 0, 4000, 1000, 0);    //changes delay offTime to 1000-0

  digitalWrite(mocPin, HIGH);
  delay(onTime);
  digitalWrite(mocPin, LOW);
  delay(offTime);

  if(temperature >= 68.0){ 
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    }
    else if(temperature <=62.0){
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      }
      else if(temperature > 62.0 && temperature <=68.0){
      digitalWrite(green, HIGH);    
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);
      }
}

void counter(){
  seconds++;
  Serial.print(seconds);
  Serial.print(",");
  Serial.println(temperature);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print("Count = ");
  lcd.print(seconds);
  }
