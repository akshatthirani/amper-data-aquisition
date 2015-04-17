// Testing basic TCP Server on Node 

// Philip & Akshat

// 14th April, 2015

TCPClient client;
byte server[] = { 104, 236, 24, 179 }; // Node Server on Digitalocean

void setup()
{
  // Make sure your Serial Terminal app is closed before powering your Core
  Serial.begin(9600);
  // Now open your Serial Terminal, and hit any key to continue!
  while(!Serial.available()) SPARK_WLAN_Loop();

  Serial.println("connecting...");

  if (client.connect(server, 3000))
  {
    Serial.println("connected");
    
    client.println("GET /get_spark HTTP/1.0");
    client.println("Host: 104.236.24.179");
    client.println("Content-Length: 0");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;);
  }
}