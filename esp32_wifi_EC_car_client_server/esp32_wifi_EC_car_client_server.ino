/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
int led1 = 23;
int led2 = 22;

int EN_A = 19;      //Enable pin for first motor
int IN1 = 25;       //control pin for first motor
int IN2 = 33;       //control pin for first motor

int IN3 = 26;        //control pin for second motor
int IN4 = 27;        //control pin for second motor
int EN_B = 18;      //Enable pin for second motor

int motor_speed = 1;
int motor_speed1 = 1;
  
  const char* ssid     = "HOTBOX-RDW";
  const char* password = "d1a9v8i9d";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);
  
    pinMode(led1, OUTPUT);      // set the LED pin mode
    pinMode(led2, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H1\">here</a> to turn the LED1 on pin 5 on.<br>");
            client.print("Click <a href=\"/L1\">here</a> to turn the LED1 on pin 5 off.<br>");

            client.print("Click <a href=\"/H2\">here</a> to turn the LED2 on pin 5 on.<br>");
            client.print("Click <a href=\"/L2\">here</a> to turn the LED2 on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H1")) {
          digitalWrite(led1, HIGH);               // GET /H turns the LED on
          Forward();
          delay(500);
          StopAllEngins();
        }
        if (currentLine.endsWith("GET /L1")) {
          digitalWrite(led1, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /H2")) {
          digitalWrite(led2, HIGH);               // GET /H turns the LED on
          Backword();
          delay(500);
          StopAllEngins();
          
        }
        if (currentLine.endsWith("GET /L2")) {
          digitalWrite(led2, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /H3")) {
//          digitalWrite(led2, HIGH);               // GET /H turns the LED on
          TurnRight();
          delay(500);
          StopAllEngins();
          
        }
        if (currentLine.endsWith("GET /L3")) {
//          digitalWrite(led2, LOW);                // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /H4")) {
//          digitalWrite(led2, HIGH);               // GET /H turns the LED on
          TurnLeft();
          delay(500);
          StopAllEngins();
          
        }
        if (currentLine.endsWith("GET /L4")) {
//          digitalWrite(led2, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void StopAllEngins(){
     Serial.println("Stop All ...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(EN_A,0);
  digitalWrite(EN_B,0);
}

void Forward(){
     Serial.println("Forward ...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(EN_A,100);
  digitalWrite(EN_B,100);
}

void Backword(){
     Serial.println("Backward ...");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(EN_A,50);
  digitalWrite(EN_B,50);
}

void TurnRight(){
     Serial.println("Right ...");
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(EN_A,100);
  digitalWrite(EN_B,100);
}

void TurnLeft(){
     Serial.println("Left ...");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(EN_A,100);
  digitalWrite(EN_B,100);
}
//void TurnMotorB(){      
//          
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//  digitalWrite(EN_B,50);
//  delay(500);
//}
//void TurnOFFB(){
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);
//}
