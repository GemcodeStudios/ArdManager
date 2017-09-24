/*
 * Author: Sohail Anwar Shah
 * Date: 24/09/2017
 * 
 * Description: This Arduino sketch is designed to work with ArdManager Pro - Available from the Google Play Store.
 * 
 * Hardware Connections:
 * Connect a push button to digital pin 8 in a pullup configuration. That is as a voltage divider 
 * with the switch pulling voltage upto to 5volts when pushed.
 * 
 * Connect an LED to pin 12. Annode to pin 12 and cathode to ground. You may want to connect a series resistor if the
 * current is too high.
 * 
 * Bluetooth Shield: Model-HC-05. Connect the TX on shield to RX of Arduino and similarly RX on shield to TX of Arduino. 
 * 
 * Disclaimer:
 * The code below is provided as is and Gemcode studios hold no responibility for loss of any kind.
 * 
 * Gemcode Studios 2017.
 */

String inputString;               // A string to hold incoming data Bluetooth signal
boolean stringComplete = false;   // whether the string is complete
boolean bNurseComing = false;     // Signal caller that nurse is aware of button press.

void setup() 
{  
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(8, INPUT);
}

void loop() 
{
  //
  // Handle input stream data (This will be Bluetooth device):
  //
  if (stringComplete) 
  {
      if (inputString == "Sw1=1\n")
      {
        bNurseComing = true; // Indicate to call we will be comming by blinking led.
      }
     
      if (inputString.substring(0) == "Sw1=0\n")
      {
        bNurseComing = false;
      }

      // clear the string:
      inputString = "";
      stringComplete = false;
    }

  // Blink LED to show Nurse is aware of call:
  if (bNurseComing)
  {
    digitalWrite(12, HIGH);
    delay(100);
    digitalWrite(12, LOW);
    delay(100);
  }
    
  //
  // Read if user presses push button:
  // You may want to put some bounce detection if it is causing 
  // problems.
  //
  int pushButton = digitalRead(8);
  if (pushButton == HIGH)
  {
    Serial.write("Alert=1.0\n"); // Sends to mobile device an audible alert.
    delay(100);
  }
  
  delay(100);  
} 

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


