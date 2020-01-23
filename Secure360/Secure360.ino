#include <SoftwareSerial.h>

int pirPin = 4;    //the digital pin connected to the PIR sensor's output
int buzzerPin = 13; //the digital pin connected to the buzzer output
int calibrationTime = 60;      

SoftwareSerial ser(10, 11); // RX, 

void setup() {

    // initialize serial communication:
  Serial.begin(115200);
  // enable software serial
  ser.begin(115200);


  //Initialize pins
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  digitalWrite(buzzerPin, LOW);

  //Caliberate sensor
  for(int i = 0; i < calibrationTime; i++)
  {
    Serial.println((calibrationTime - i)+" secs left...");
    delay(1000);
  }
  delay(50);

  //Alert when sensor get caliberated
  digitalWrite(buzzerPin, HIGH);
  delay(500);  
  digitalWrite(buzzerPin, LOW);
  delay(500);  
  digitalWrite(buzzerPin, HIGH);
  delay(500);  
  digitalWrite(buzzerPin, LOW);


  // reset ESP8266
  reset();
  connectWifi();
    
}

void reset() {
  ser.println("AT+RST");
  delay(1000);
  if(ser.find("OK") ) Serial.println("Module Reset");
}

void connectWifi() {
  String cmd = "AT+CWJAP=\"Akash\",\"one2three\"";
  ser.println(cmd);
  delay(4000);
  if(ser.find("OK")) {
    Serial.println("Connected!");
  }
  else {
    connectWifi();
    Serial.println("Cannot connect to wifi"); 
  }
}



void loop() 
{
  if(digitalRead(pirPin) == HIGH)
  {
    updateStatus("yes");
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(500);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    Serial.println("Motion detected");
   

  }
  else
  {
     digitalWrite(buzzerPin, LOW);
     Serial.println("Motion not detected");
    updateStatus("no");
  }
  delay(5000);
}

void updateStatus(String state)
{
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "secure360.is-best.net"; // Firebase cloud
  cmd += "\",80";
  ser.println(cmd);

  if( ser.find("OK")) {
    Serial.println("TCP connection ready");
  } 
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
  return;
  }
  delay(1000);  
  // prepare GET string
 String getStr = "GET /server.php?status="+state+" HTTP/1.1\r\n";
getStr += "Host: secure360.is-best.net\r\n";
getStr += "Cookie: __test=cf78b93a8cbe42fa0bb42bf9d0047b43; expires=Friday, 1 January 2038 at 05:25:55; path=/";
  
  getStr += "\r\n\r\n";



  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length())+"\r\n";
  
  ser.println(cmd);
  Serial.println(cmd);

  delay(500);
  
  
  if(ser.find(">")){
    Serial.println("Sending.. "+getStr);
    
    ser.print(getStr);
    if( ser.find("SEND OK")) { Serial.println("Packet sent");

while (ser.available()) {

String tmpResp = ser.readString();

Serial.println(tmpResp);

}

  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
}
}
