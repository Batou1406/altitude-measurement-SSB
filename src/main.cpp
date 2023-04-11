#include <Arduino.h>
#include <CAN.h>
#include <RadarBaumer.hpp>

//---------------- GLOBAL VARIABLES --------------------------
bool ledState = false;
RadarBaumer radar;

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");

  // Initialise CAN1
  if(!can1.begin(250E3)){
    Serial.println("Starting CAN1 failed");
  }else{
    Serial.println("Starting CAN1 Succed");
    //can1.filterExtended(0x18C0FF80, 0);
    can1.onReceive(radar.readHeight);
  }

  // Initialise CAN0
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

    delay(500);
    
    Serial.print("packet ID : ");
    Serial.println(radar.canIDReceived,HEX);
    Serial.print("Raw data : ");
    for(int i = 0; i<8; i++){
      Serial.print(radar.rawData[i],HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
    Serial.print("Sensor Status :");
    Serial.println(radar.sensorStatus);
    Serial.print("Target Confidence :");
    Serial.println(radar.targetConfidence);
    Serial.print("Target Distance [mm]: ");
    Serial.print(radar.targetDistance);
    Serial.println();
    Serial.print("Target speed [mm/s] :");
    Serial.println(radar.targetSpeed);
    Serial.println();
}