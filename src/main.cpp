#include <Arduino.h>
#include <CAN.h>

bool ledState = false;

void SerialPrint(int packetLength){
  Serial.print("We received a new packet of size ");
  Serial.println(packetLength);

  Serial.println(can1.read());
  Serial.println(can1.read());
  Serial.print(can1.read());
  //for(int i = 0 ; i < packetLength; i++){
  //  Serial.print(can1.read());
  //}

  Serial.println("");
  Serial.println("");
}

void readBaumerheight(int packetLength){
  if(packetLength != 8){
    Serial.println("Oupsi, not a correct Baumer frame");
    return;
  }

  uint8_t tempByte = 0;
  uint8_t sensorStatus = 0;
  uint8_t targetConfidence = 0;
  uint64_t targetDistanceRaw = 0;
  uint64_t targetSpeedRaw = 0;
  float targetDistance = 0;
  float targetSpeed = 0;

  tempByte = can1.read();
  sensorStatus = tempByte;

  tempByte = can1.read();
  targetConfidence = tempByte;

  tempByte = can1.read();
  targetDistanceRaw = tempByte;
  tempByte = can1.read();
  targetDistanceRaw += tempByte << 8;
  tempByte = can1.read();
  targetDistanceRaw += tempByte << 16;

  Serial.print("Sensor Status :");
  Serial.println(sensorStatus);
  Serial.print("Target Confidence :");
  Serial.println(targetConfidence);
  Serial.println("Target Distance");
  Serial.println(targetDistanceRaw);
  Serial.println();
}

void setup()
{

    delay(5000);
    Serial.begin(9600);
    Serial.println("Setup");

    if(!can1.begin(250E3)){
      Serial.println("Starting CAN1 failed");
    }else{
      Serial.println("Starting CAN1 Succed");
      can1.onReceive(readBaumerheight);
    }
    if(!can0.begin(10000)){
      Serial.println("Starting CAN0 failed");
    }else{
      Serial.println("Starting CAN0 Succed");
    }

    // Setup the general LED
    pinMode(PB04, OUTPUT);
}

void loop() {
    //blink
    if(ledState){
      digitalWrite(PB04, LOW);
      ledState = false;
    }else{
      digitalWrite(PB04, HIGH);
      ledState = true;
    }
    
    /*
    Serial.println("Start Sending");

    can1.beginPacket(0x2);
    can1.write(0x32);
    can1.write(0X03);
    can1.endPacket();
    */

    delay(100);
}