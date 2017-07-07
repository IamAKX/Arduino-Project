void setup() {
// initialize serial communications at 9600 bps:
Serial.begin(9600);

delay(2000);
// print the serial data to GSM
Serial.println("ATD8981305046;" ); // change here to call a number using SIM900A
// wait 10 seconds before the next loop
delay(10000);
Serial.print("ATH" ); // hold the call
}

void loop() {
}
