
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* WiFi */
const char* ssid     = "Sergio_LINK";
const char* password = "4F48C9n?t-";
/* WiFi Reconnection */
const int wifiCheckTimeout = 10000;
int lastCheckTime = 0;
/* UDP */
char packetBuffer[255]; //buffer to hold incoming packet
const unsigned int udpPort = 27020;
WiFiUDP Udp;


void espConnectWifi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        //Serial.print(".");
    }

    //Serial.println("");
    //Serial.println("WiFi Connected");
    //Serial.println("IP address: ");
    //Serial.println(WiFi.localIP());
}


void getPacket()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    //Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    //Serial.print(remoteIp);
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    //Serial.println("Contents:");
    //Serial.println(packetBuffer); // -- str dbg output
    Serial.write(packetBuffer, 16);
    

    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.println("");
  espConnectWifi();
  Udp.begin(udpPort);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(millis() - lastCheckTime > wifiCheckTimeout){
     if(WiFi.status() != WL_CONNECTED){
          //Serial.println("Reconnecting...");
          espConnectWifi();
    }
    lastCheckTime = millis();
  }

  delay(5);
  
  getPacket();
  
}
