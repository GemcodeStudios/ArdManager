/*
 * Author: Sohail Anwar Shah
 * Date: 24/09/2017
 * 
 * Description: This Arduino sketch is designed to work with ArdManager - Available from the Google Play Store.
 * 
 * Hardware Connections:
 * Follow standard setup for Temperature Humidity sensor (SimpleDHT). I use pin digital pin 2.
 * 
 * Connect an LED to pin 13. Annode to pin 13 and cathode to ground. You may want to connect a series resistor if the
 * current is too high.
 * 
 * Bluetooth Shield: Model-HC-05. Connect the TX on shield to RX of Arduino and similarly RX on shield to TX of Arduino. 
 * 
 * Disclaimer:
 * The code below is provided as is and Gemcode studios hold no responibility for loss of any kind.
 * 
 * Gemcode Studios 2017.
 */
#include <SimpleDHT.h>

int pinDHT11 = 2;
SimpleDHT11 dht11;

String inputString;         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() 
{  
  Serial.begin(9600);
  pinMode(13, OUTPUT);  
}

void loop() 
{
  //
  // Handle input from Bluetooth stream data:
  //
  if (stringComplete) 
  {   //
      // Example on how to check for flip switch commands sent from ArdManager Mobile App:
      // See ArdManager help to see what other String header data can be sent.
      //
      if (inputString == "Sw1=1\n")
      {
        digitalWrite(13, HIGH);
        Serial.write("Console:Light ON\n");
        delay(100);
      }
     
      if (inputString.substring(0) == "Sw1=0\n") // Use this method if you need to check for part commands.
      {
        digitalWrite(13, LOW);
        Serial.write("Console:Light Off\n");
        delay(100);
      }

      // Push data back into serial port again:
      // This is done in case your Arduino is connected to a PC via USB.
      // Data passed from Mobile App that the Arduino is not interested in
      // can be processed by the PC app such as Geolocation/Accelerometer/compass.
      // If you are not planning to use a PC app then you may comment out the
      // call!
      Serial.print(inputString);
      delay(100);
      
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
      
  //
  // Data Monitor Stream:
  //
  // Read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Console: Read DHT11 failed"); // Using "Console: " will be picked up the ArdManager App console.
    return;
  }

  // Send Temperature value using format: "M1:SensorValue\n"
  Serial.print("M1:");
  Serial.println((int)temperature);

  // Send Temperature Monitor Name using format: "M1_Name:Monitor Name\n"
  Serial.println("M1_Name:Monitor 1: Temperature");

  // Send Temperature Monitor Sub Details using format: "M1_Sub:Sub Details\n"
  Serial.print("M1_Sub:");
  Serial.println("C");

  Serial.print("M2:");
  Serial.println((int)humidity); 
  Serial.println("M2_Name:Monitor 2: Humidity");
  Serial.print("M2_Sub:");
  Serial.println("%");

  delay(1000);  
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


