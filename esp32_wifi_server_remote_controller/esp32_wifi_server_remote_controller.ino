/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/









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
#include <HTTPClient.h>

const int SW_pin = 33; // digital pin connected to switch output
const int X_pin = 25; // analog pin connected to X output
const int Y_pin = 26; // analog pin connected to Y output

 int y_pos; // analog pin connected to Y output
 int x_pos; // analog pin connected to Y output

  // constants won't change. They're used here to set pin numbers:
const int buttonPin = 23;     // the number of the pushbutton pin
const int buttonPin2 = 22;     // the number of the pushbutton pin
const int buttonPin3 = 18;     // the number of the pushbutton pin
const int buttonPin4 = 19;     // the number of the pushbutton pin

const int ledPin =  26;      // the number of the LED pin
const int ledPin2 =  25;  
const int ledPin3 =  33;  

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int buttonState3 = 0;         // variable for reading the pushbutton status
int buttonState4 = 0;         // variable for reading the pushbutton status

  const char* ssid     = "SSID";
  const char* password = "password";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(23, OUTPUT);      // set the LED pin mode

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
    
  pinMode(SW_pin, INPUT);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
    server.begin();

     // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);

}

int value = 0;
int value2 = 0;
int value3 = 0;
int value4 = 0;

void loop(){
  StartServer();

   // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if(buttonState == HIGH){
    SendMove("1");
    Serial.println("CLICKED 1");
    }
    if(buttonState2 == HIGH){
      SendMove("2");
      Serial.println("CLICKED 2");
    }
    if(buttonState3 == HIGH){
      SendMove("3");
      Serial.println("CLICKED 3");
    }
    if(buttonState4 == HIGH){
      Serial.println("CLICKED 4");
      SendMove("4");
    }
    
}

void SendMove(String moveToMake ){
   
      TurnLedOn();
      HTTPClient http;   
 
   http.begin("http://192.168.1.11/H"+ moveToMake);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
 
   int httpResponseCode = http.GET();   //Send the actual GET request
 
   if(httpResponseCode>0){
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
//    Serial.println(response);           //Print request answer
 
   }else{
 
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   
   
   
    // turn LED off:
    
   http.begin("http://192.168.1.11/L"+ moveToMake);  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
 
   int httpResponseCode2 = http.GET();   //Send the actual GET request
       TurnLedOff();

            
       if(httpResponseCode2 == 200){
        TurnOKLedOn();
        } else{
          TurnERRLedOn();
          }
       
        http.end();  //Free resources
        TurnOKLedOff();
        TurnERRLedOff();
  
  }

  
  void TurnLedOff(){
  digitalWrite(ledPin, LOW);
  }

  void TurnLedOn(){
    digitalWrite(ledPin, HIGH);
  }
  void TurnERRLedOff(){
  digitalWrite(ledPin3, LOW);
  }

  void TurnERRLedOn(){
    digitalWrite(ledPin3, HIGH);
  }
  void TurnOKLedOff(){
  digitalWrite(ledPin2, LOW);
  }

  void TurnOKLedOn(){
    digitalWrite(ledPin2, HIGH);
    }

    void StartServer(){
        
 WiFiClient client = server.available();   // listen for incoming client

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
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

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
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
      }
