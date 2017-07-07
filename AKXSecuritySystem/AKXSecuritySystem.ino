// PIR security alarm
// Buzzer in PIN 12(Output)
// PIR in PIN 3(Input)

/*
 * connection

      ARDUINO   GSM  
      ---------------
      GND       GND   
      Pin0(RX)  TX  
      Pin1(TX)  RX  
      
      
      ARDUINO   PIR   
      ----------------
      GND       GND   
      Pin3      Output
      5V        5V    
      
      ARDUINO   BUZZER  
      -------------------
      pin12     red
      pin13     black
 */

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 60;       

//the time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int buzzerPin = 13;



void setup()
{
  Serial.begin(9600);
  
  //Initialize pins
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  
  //Caliberate sensor
  Serial.println("Sensor is caliberating.. Sensor will be ready in 30 secs..");
  for(int i = 0; i < calibrationTime; i++){
      Serial.println((calibrationTime - i)+" secs left...");
      delay(1000);
      }
    Serial.println(" Done");
    Serial.println("SENSOR ACTIVE");
    delay(50);

  //Alert when sensor get caliberated
    digitalWrite(buzzerPin, HIGH);
    delay(500);  
    digitalWrite(buzzerPin, LOW);
    delay(500);  
    digitalWrite(buzzerPin, HIGH);
    delay(500);  
    digitalWrite(buzzerPin, LOW);
    
}


void loop()
{
  if(digitalRead(pirPin) == HIGH)
  {
       // digitalWrite(buzzerPin, HIGH); -------------------------------------------------------- BUZZER DECATIVE  
       makeACall();
       if(lockLow)
       { 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         delay(50);
       }        
         takeLowTime = true;
  }
 
  if(digitalRead(pirPin) == LOW)
  {      
     //digitalWrite(buzzerPin, LOW);   -------------------------------------------------------- BUZZER DECATIVE
 
     if(takeLowTime)
     {
       lowIn = millis();          //save the time of the transition from high to LOW
       takeLowTime = false;       //make sure this is only done at the start of a LOW phase
     }
       //if the sensor is low for more than the given pause,
       //we assume that no more motion is going to happen
     if(!lockLow && millis() - lowIn > pause)
     { 
       //makes sure this block of code is only executed again after
       //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
      }
  }
}

void makeACall()
{
  // print the serial data to GSM
  Serial.println("ATD7992353568;" ); // change here to call a number using SIM900A
  // wait 10 seconds before the next loop
  delay(10000);
 // Serial.print("ATH" ); // hang the call
}
