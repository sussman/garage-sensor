/* 
 Detection of state-change in a Reed switch.

    -- originally based on 'Debounce' sample code from David A. Mellis, Limor Fried
    -- also based on 'TelnetClient' sample code, by Tom Igoe

*/

#include <SPI.h>
#include <Ethernet.h>


const int buttonPin = 7;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 50;     // the debounce time; increase if the output flickers
int lastPrinted = 0;         // for debouncing the state-change
int printed = 0;             // ditto

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // made-up MAC
IPAddress clientIP(10,0,1,240);
IPAddress serverIP(10,0,1,144); 
int serverPort = 51729;
EthernetClient client;


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Ethernet.begin(mac, clientIP);
  Serial.begin(9600);
  delay(1000);
  Serial.println("Connecting to server...");

  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to TCP server.");
  } 
  else {
    Serial.println("Connection to TCP server FAILED.");
  }
}


void loop() {
  int reading = digitalRead(buttonPin);
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
  }
  // set the LED using the state of the button:
  digitalWrite(ledPin, buttonState);
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  if (lastButtonState != buttonState) {
    printed = buttonState;
    // debounce the state-printing too!
    if (printed != lastPrinted) {
      notifyStateChange(buttonState);
    }
    lastPrinted = printed;
  }
  lastButtonState = reading;
}


// Alert that the state of the switch has changed
void notifyStateChange(int buttonState) {

  // if the TCP server's disconnected, retry the connection.
  if (! client.connected()) {
    Serial.println("Warning: TCP not connected.");
    client.stop();
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Re-connected to TCP server.");
    } 
    else {
      // if you didn't get a connection to the server:
      Serial.println("Connection lost, failed to reconnect.");
    }
  }

  delay(1000);  // more debounce-insurance -- state can't "officially" change more than once per second.
  if (buttonState == 0) {
    Serial.println("Switch is OPEN");
    if (client.connected()) {
      client.print("Switch is OPEN");
    }
  } else {
    Serial.println("Switch is CLOSED");
    if (client.connected()) {
      client.print("Switch is CLOSED");
    }
  }
}
