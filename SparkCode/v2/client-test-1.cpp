/*

client-test-1.cpp

Date: 16th April, 2015
Author: Akshat Thirani

Purpose:

This program is to create a connection from
the Spark to a node server that is listening
for TCP connections.

Every second the analog value from pin A0
is logged, and is transmitted every 15
seconds using the TCP connections.

A string is being created as the message
which would look like:

	"UNIXDate.0:val0;1:val1;...;14:val14"

*/

#DEFINE APIN A0

#include "application.h"
#include "math.h"

TCPClient client;

byte server[] = { 74, 125, 224, 72 }; // Google
int port = 80;

void transmitData(String dataString);

int analogvalue = 0;
String makeString;

void setup()
{
    Serial.begin(9600);		// Data rate
    delay(10000);

    pinMode(APIN, INPUT);

    Serial.println("Hello, World! Input PIN assigned.");

    Serial.println("Connecting to TCP Server...");

    if (client.connect(server, port))
      {
        Serial.println("connected");

				client.println("GET /search?q=unicorn HTTP/1.0");
        client.println("Host: www.google.com");
        client.println("Content-Length: 0");
        client.println();

        // To be changed to POST

        // Server should respond with a confirmation that is printed to
        // the serial monitor.

      }
      else
      {
        Serial.println("connection failed");
      }

}


void loop()
{
	int itt = 0;
	char numstr1[21]; // enough to hold all numbers up to 64-bits
	char numstr[21]; // enough to hold all numbers up to 64-bits

	int flag1 = 1;

	int tempTime = Time.now();
		sprintf(numstr2, "%d", age);		// Casting int as String

	makeString = numstr1;

		Serial.println("Time (UNIX) = ");
		Serial.print(makeString);

	makeString = makeString + ".";

		Serial.println("Time with added . = ");
		Serial.print(makeString);

	while (itt < 15)
	{
		analogvalue = analogRead(APIN);

			Serial.println("Value at ");
			Serial.print(itt);
			Serial.println(" = ");
			Serial.print(analogvalue);

		sprintf(numstr1, "%d", itt);		// Casting int as String
		sprintf(numstr2, "%d", analogvalue);		// Casting int as String

		makeString = makeString + itt + ":" + numstr2;

		if (itt != 14)
			makeString = makeString + ";";

		delay(1000);	// Delay of 1 sec.
	}

	// The string to be sent should be ready now as makeString

		Serial.println("Final string = ");
		Serial.print(makeString);

		Serial.println("---- Entering transmitData function...");
	transmitData(makeString);
		Serial.println("---- Returned to main loop function...");
}

void transmitData(String dataString)
{
	Serial.println("Connecting...");

	if(client.connect(server, port)){

		Serial.println("...Connection succesful. ");

		client.print("POST /update HTTP/1.1\n");
        client.print("Host: 104.236.24.179\n");
        client.print("Connection: close\n");
        client.print("\n\n");

        // Now the real data

        client.println(dataString);

        Serial.println("Transmission succesful.");
	}
	else{
		Serial.println("Unable to connect.");
	}

	if(!client.connected()){
        client.stop();
    }

	client.flush();
    client.stop();
}
