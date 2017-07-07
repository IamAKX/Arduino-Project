//SMART GARBAGE COLLECTION

/*Circuit
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 2
	* ECHO connection of the sensor attached to digital pin 4

*/

#include <SoftwareSerial.h>

SoftwareSerial ser(10, 11); // RX, TX

const int trigPin = 2;
const int echoPin = 4;
String apiKey = "SKO3IO70X29E8KHW";

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  // enable software serial
  ser.begin(115200);

  // reset ESP8266
  ser.println("AT+RST");
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inchesLength, cmLength;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inchesLength = microsecondsToInches(duration);
  cmLength = microsecondsToCentimeters(duration);
  
  if(cmLength <= 15)
    {
  
      sendEmail();
      //sendTextMsg();  
    }
  
  Serial.print(inchesLength);
  Serial.print("in, ");
  Serial.print(cmLength);
  Serial.print("cm");
  Serial.println();

  sendToThinkSpeak(cmLength);
    
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


void sendToThinkSpeak(long cmLength)
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
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(cmLength);
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

void sendTextMsg()
{
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
  String getStr = "GET http://akashapplications.hol.es/arduino/smsany/server.php?num=+919804945122&msg=Your%20garbage%20level%20is%20critical.%20Please%20empty%20is%20immediately.";

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
     
  
}


void sendEmail()
{
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
  String getStr = "GET http://akashapplications.hol.es/Akash/sendemail.php?to=akx.sonu%40gmail.com&msg=The+level+of+the+garbage+is+critically+high.+Empty+is+immediately.";

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
  sendTextMsg(); 
}

