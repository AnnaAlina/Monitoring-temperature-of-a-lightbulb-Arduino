#define THERMISTOR A1           //defining pin A1 to read thermistor value
#define NUMSAMPLES 5             //defining sample size as 5
#define THERMROOM 25           //defining room temperature as 25
#define ROOMRESIST 10000        //defining resistance at room temperature
#define BETA 3960               //defining Beta value from datasheet of thermistor
#define SERIESR 4700           //defining series resistor value

#include <LiquidCrystal.h>      //including LCD library
#include <TimerOne.h>        //include TimerOne library

LiquidCrystal lcd(10,9,5,4,3,2);      //define object called lcd

uint16_t samples[NUMSAMPLES];           //declaring unsigned 16 bit integer array
int mocPin = 6;            //declaring digital pin 6 for mocPin
int red = 11;                       //declaring red pin  11
int yellow = 12;                    //declaring yellow pin 12
int green = 13;                     //declaring green pin 13

unsigned long seconds;              //declaring seconds variable
float temperature;                  //declaring float temperature variable

void setup(void) {               //initial setup 
  lcd.begin(16,2);             //initializing lcd with 16 columns and 2 rows
  lcd.clear();                 //clears screen
  Serial.begin(9600);            //initialize serial monitor with baud rate of 9600
  analogReference(EXTERNAL);    //3.3v to AREF for a less noisy signal for thermistor readings
  pinMode(mocPin, OUTPUT);        //declare mocPin as output
  pinMode(red, OUTPUT);             //declaring red, yellow and green LEDs as output pins
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  Timer1.initialize(1000000);       //initialized the timer in microseconds
  Timer1.attachInterrupt(counter);  //every 1000000us it interrupts program and runs counter
}

void loop() {                               //code to run repeatedly                   
  uint8_t i;                                //unsigned 8-bit integer i
  float number;
for(i = 0; i < NUMSAMPLES; i++){          //for loop to get 5 analog samples
  samples[i] = analogRead(THERMISTOR);    //assigns value to each position within array
  delay(10);                              //waits
  }

 number = 0;                //sets number to 0;
 for(i=0; i < NUMSAMPLES; i++){    //for loop to run through sample size
   number += samples[i];        //adds all sample values and assigns to number variable
  }
 number /= NUMSAMPLES;    //divides total by sample size and assigns to number variable

 number = 1023 / number - 1;        //changes 10-bit value to real value
 number = SERIESR/number;        //4.7k/real value
                                    //calculations for converting averaged sample value to                                                 
                                    //a temperature reading
temperature = number / ROOMRESIST;  //temp=avg sample value/r at room temp of thermistor
temperature = log(temperature);    //setting temperature to the log of temperature
temperature /= BETA;            //divide temperature by beta and set it to temperature
temperature += 1.0 / (THERMROOM + 273.15);    //temperature +1 /25 + 273.15
temperature = 1.0/temperature;    //inverts temperature
temperature -= 273.15;        //converts to celsius

  int setpoint = 65;                //declare temperature setpoint to 65 degrees
  int err;                          //declare error variable
  int Gain = 15;                    //declare gain for calculation
  int dutyCycle;                    //declare variable dutycycle
  int onTime;                       //declare on and offtime variables
  int offTime;

  err = setpoint - temperature;                //error equals 65 - current temp
  dutyCycle = err*Gain;                        //set duty cycle to error times the gain
  dutyCycle = constrain(dutyCycle, 0, 200);       //constraints answer from 0-200
  onTime = map(dutyCycle, 0, 200, 0, 4000);    //changes to delay onTime to 0-4000
  offTime= map(onTime, 0, 4000, 1000, 0);    //changes delay offTime to 1000-0

  digitalWrite(mocPin, HIGH);                //turn on mocPin
  delay(onTime);                             //keep on for ontime
  digitalWrite(mocPin, LOW);                 //turn off mocpin
  delay(offTime);                            //keep off for offtime

  if(temperature >= 68.0){            //if the temperature is greater than or equal to 68
    digitalWrite(red, HIGH);          //turn on red led
    digitalWrite(yellow, LOW);        //turn off yellow and green leds
    digitalWrite(green, LOW);
    }
    else if(temperature <=62.0){      //if temperature is less than or equal to 62
      digitalWrite(yellow, HIGH);     //turn on yellow led
      digitalWrite(red, LOW);         //turn off green and red leds
      digitalWrite(green, LOW);
      }
      else if(temperature > 62.0 && temperature <=68.0){ 
                                                         //if temp is 3 of 65 degrees
      digitalWrite(green, HIGH);                         //turn on green led
      digitalWrite(yellow, LOW);                         //turn off yellow and red leds
      digitalWrite(red, LOW);
      }
}

void counter(){                     //function called counter
  Seconds++;                        //add one to seconds
  Serial.print(seconds);        //prints seconds
  Serial.print(",");            //prints comma
  Serial.println(temperature);    //prints temperature
  lcd.clear();                   //clears lcd
  lcd.setCursor(0,0);               //sets cursor to top left
  lcd.print("Temp = ");           //prints to lcd
  lcd.print(temperature);        //prints temperature value
  lcd.setCursor(0,1);               //sets cursor to next row
  lcd.print("Count: ");        //prints to lcd
  lcd.print(seconds);            //prints seconds value
  }
