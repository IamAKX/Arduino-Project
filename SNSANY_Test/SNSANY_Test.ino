
// Plot MQ135 data on thingspeak.com using an Arduino and an ESP8266 WiFi
// module.

#include <SoftwareSerial.h>
#include <stdlib.h>

// LED
int ledPin = 13;
// MQ135 analog input
int mq135Pin = 0;

// replace with your channel's thingspeak API key
String apiKey = "N0QIQDV8XL50PZ3S";

// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX

// this runs once
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);    

  // enable debug serial
  Serial.begin(115200);
  // enable software serial
  ser.begin(115200);

  // reset ESP8266
  ser.println("AT+RST");
}


// the loop
void loop() {

  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);

  // read the value from LM35.
  // read 10 values for averaging.
  int val = analogRead(mq135Pin);   
      delay(500);
  

  // convert to mq135SensedValue:
  
  float mq135SensedValue = val;

  // convert to string
  char buf[16];
  String strmq135SensedValue = dtostrf(mq135SensedValue, 4, 2, buf);

  Serial.println(strmq135SensedValue);

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "31.220.16.16"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET http://akashapplications.hol.es/arduino/smsany/server.php?";
  
  getStr +="num=+919804945122";
  getStr +="&msg=Hello,%20I%20am%20arduino!!";
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
