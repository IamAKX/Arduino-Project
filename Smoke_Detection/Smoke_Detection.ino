//SMOKE DETECTION SYSTEM


/*
The MQ-2 smoke sensor is sensitive to smoke and to the following flammable gases:

       - LPG
       - Butane
       - Propane
       - Methane
       - Alcohol
       - Hydrogen
*/


/*Circuit
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* A0 connection of the sensor attached to analog pin A5

        * Buzzer black wire to ground
        * Buzzer red wire to pin 13

*/

#include <SoftwareSerial.h>

SoftwareSerial ser(10, 11); // RX, TX


const int smoke = A5;
const int buzzerPin = 13;

void setup() {
 
  pinMode(smoke, INPUT);
  Serial.begin(9600);

   ser.begin(115200);

  // reset ESP8266
  ser.println("AT+RST");
}

void loop()
{
  //This function will read smoke level
  
  int analogSensor = analogRead(smoke);

  Serial.print("Smoke: ");
  Serial.println(analogSensor);
  if(analogSensor > 220)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(1500);  
    digitalWrite(buzzerPin, LOW);
    delay(500);  
    digitalWrite(buzzerPin, HIGH);
    delay(1000);  
    digitalWrite(buzzerPin, LOW);
   
  }
  sendToThinkSpeak(analogSensor);
}


void sendToThinkSpeak(int smk)
{
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET https://api.thingspeak.com/update?api_key=";
  getStr += "SV6R2WIHSYEXRW9Z";
  getStr +="&field1=";
  getStr += String(smk);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}


