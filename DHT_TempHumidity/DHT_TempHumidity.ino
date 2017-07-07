#include <SimpleDHT.h>

#include <SoftwareSerial.h>

SoftwareSerial ser(10, 11); // RX, TX

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;
const String apiKey = "Z0MZSA4ZTJAUVX8J";
void setup() {
  Serial.begin(115200);
  ser.begin(115200);

  // reset ESP8266
  ser.println("AT+RST");
}

void loop() 
{
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) 
  {
    Serial.print("Read DHT11 failed.");
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %");
  
  // DHT11 sampling rate is 1HZ.
  
  sendThingSpeak((int)temperature, (int)humidity);

}

void sendThingSpeak(int t, int h)
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET https://api.thingspeak.com/update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(t);
  getStr +="&field2=";
  getStr += String(h);
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

