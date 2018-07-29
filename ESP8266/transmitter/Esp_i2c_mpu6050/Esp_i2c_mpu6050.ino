#include <ESP8266WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>


/* I2C MPU6050*/
const int8_t R_CONFIG = 0x1A;
const int8_t R_GYRO_CONFIG = 0x1B;
const int8_t R_ACCEL_CONFIG = 0x1C;

const int MPU_ADDR = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

/* WiFi */
const char* ssid     = "Sergio_LINK";
const char* password = "4F48C9n?t-";
/* WiFi Reconnection */
const int wifiCheckTimeout = 10000;
int lastCheckTime = 0;

/* UDP */
const unsigned int udpPort = 27020;
WiFiUDP Udp;
const IPAddress udpServer(192, 168, 0, 255); // udp server address

void mpuWrite(int8_t addr, int8_t data){
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission(true);
}

void mpuInitConfig(){
    mpuWrite(R_CONFIG, 0b00000011); //write to filter DLP_CFG value 3
    Serial.print(" >> Configured filter.\n");
    mpuWrite(R_GYRO_CONFIG, 0b10000000); // run gyroX self test
    mpuWrite(R_GYRO_CONFIG, 0b01000000); // run gyroY self test
    mpuWrite(R_GYRO_CONFIG, 0b00100000); // run gyroZ self test
    mpuWrite(R_GYRO_CONFIG, 0b00011000); // set gyro scale range to +/- 2000 C/s
    Serial.print(" >> Configured gyroscope.\n");
    mpuWrite(R_ACCEL_CONFIG, 0b10000000); // run accelX self test
    mpuWrite(R_ACCEL_CONFIG, 0b01000000); // run accelY self test
    mpuWrite(R_ACCEL_CONFIG, 0b00100000); // run accelZ self test
    mpuWrite(R_ACCEL_CONFIG, 0b00011000); // set accel scale to +/- 16g
    //mpuWrite(R_ACCEL_CONFIG, 0b00011000); // set accel DHPF(Difital High Pass Filter)
    Serial.print(" >> Configured accelerometer.\n");
}

void espConnectWifi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void uartDebugPrint(){
    Serial.print("AcX = "); Serial.print(AcX);
    Serial.print(" | AcY = "); Serial.print(AcY);
    Serial.print(" | AcZ = "); Serial.print(AcZ);
    Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
    Serial.print(" | GyX = "); Serial.print(GyX);
    Serial.print(" | GyY = "); Serial.print(GyY);
    Serial.print(" | GyZ = "); Serial.println(GyZ);
}

void buildAndSendStringPacket(){
    String packet = "";

    packet += String(AcX) + " ";
    packet += String(AcY) + " ";
    packet += String(AcZ) + " ";
    packet += String(GyX) + " ";
    packet += String(GyY) + " ";
    packet += String(GyZ) + " ";
    packet += String(Tmp / 340.00 + 36.53);

    byte packetBuffer[packet.length()+1];
    packet.getBytes(packetBuffer, packet.length()+1);
    udpWritePacket(packetBuffer, packet.length()+1);
}

void buildAndSendBytePacket(){
  // packet size 14 bytes with temp two bytes
  // +2 bytes for packet end marker(packet delimiter)
  const size_t packetSize = 16;
  byte packet[packetSize] = {0};

  packet[0] = (AcX & 0x00FF);
  packet[1] = ((AcX & 0xFF00) >> 8);

  packet[2] = (AcY & 0x00FF);
  packet[3] = ((AcY & 0xFF00) >> 8);

  packet[4] = (AcZ & 0x00FF);
  packet[5] = ((AcZ & 0xFF00) >> 8);

  packet[6] = (GyX & 0x00FF);
  packet[7] = ((GyX & 0xFF00) >> 8);

  packet[8] = (GyY & 0x00FF);
  packet[9] = ((GyY & 0xFF00) >> 8);

  packet[10] = (GyZ & 0x00FF);
  packet[11] = ((GyZ & 0xFF00) >> 8);

  // Calculate temp by formula Tmp / 340.00 + 36.53 on receive side
  packet[12] = (Tmp & 0x00FF);
  packet[13] = ((Tmp & 0xFF00) >> 8);

  // Two byte packet end marker, chosed values equal to 132 celsium temperature
  // which is unreal for temp in normal environment
  packet[14] = 0x7F;
  packet[15] = 0xFF;

  Serial.write(packet, packetSize); // Debug
  udpWritePacket(packet, packetSize);
}

void udpWritePacket(byte *packetBuffer, unsigned int packetSize){
    Udp.beginPacket(udpServer, udpPort);
    Udp.write(packetBuffer, packetSize);
    Udp.endPacket();
}

void mpuReadData(){
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);  // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void setup() {
    // put your setup code here, to run once:
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    Serial.begin(115200);
    Serial.println("");
    mpuInitConfig();
    Serial.println("Connecting...");
    espConnectWifi();
    delay(2000);
}

void loop() {

    if(millis() - lastCheckTime > wifiCheckTimeout){
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("Reconnecting...");
            espConnectWifi();
        }
        lastCheckTime = millis();
    }


    
    mpuReadData();
    //buildAndSendStringPacket();
    buildAndSendBytePacket();
    //uartDebugPrint();
    
    delay(5);
}
