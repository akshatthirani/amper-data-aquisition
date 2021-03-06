/*
  thingspeak_v2.cpp

  Author: Akshat Thirani
  Date: April 8th, 2015

  Purpose: This program connects the Spark Core to a ThingSpeak
  channel, and sends analog (ADC) values from the A0 pin.

  The program is being developed to send more data by exploiting the
  buffer and TCP connection.

*/
#include "application.h"
#include "math.h"

// Thinkspeak channel information
String writeAPIKey = "CIQJGMRZXJQSTG5W";
String channelID = "29116";

// TCP socket initialize
TCPClient client;

void ThingSpeakUpdate(String tsData);

float x;
float y;
int analogvalue;
int APIN = A0;

/*--------------------------------------------------------
Setup
--------------------------------------------------------*/
void setup()
{
    Serial.begin(9600);
    delay(10000);
    Serial.println("===Starting===");

    pinMode(APIN, INPUT);
}

/*--------------------------------------------------------
Main loop
--------------------------------------------------------*/
void loop()
{
    if(Spark.connected())
    {

        for (float z = 4.712; z < 10.995; z = z + .15)      // "z" sets the sin wave to the first zero crossing 4.712 and ends it on the next 10.995.
        {
          x = sin(z) * 127.5 + 127.5;                       // Making the sin wave all positive numbers and setting it to a scale of 0-255 (makes it easy to PWM an LED)
          y = 255 - (sin(z) * 127.5 + 127.5);               // This inverts the sin wave so we have two streams.

        analogvalue = analogRead(APIN);
          // Must convert data to Strings, make sure you use capital "S" in Strings
          ThingSpeakUpdate("field1="+String(x)+"&field2="+String(y)+"&field3="+String(analogvalue));

          // I put this delay in place so we don't flood Thingspeak but you should really use a timer, delays screw with the sparkcloud connection some times.
          delay(1000);
        }
    }
}

/*------------------------------------------------
Sends sensor data to Thingspeak
Inputs: String, data to be entered for each field
Returns:
------------------------------------------------*/
void ThingSpeakUpdate(String tsData)
{
    Serial.println("Date string: " + tsData);

    Serial.println("...Connecting to Thingspeak");

    // Connecting and sending data to Thingspeak
    if(client.connect("api.thingspeak.com", 80))
    {
        Serial.println("...Connection succesful, updating datastreams");

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(tsData.length());
        client.print("\n\n");

        client.println(tsData); //the ""ln" is important here.

        // This delay is pivitol without it the TCP client will often close before the data is fully sent
        delay(200);

        Serial.println("Thingspeak update sent.");
    }
    else{
        // Failed to connect to Thingspeak
        Serial.println("Unable to connect to Thingspeak.");
    }

    if(!client.connected()){
        client.stop();
    }
    client.flush();
    client.stop();
}
