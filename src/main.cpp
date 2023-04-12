#include <Arduino.h>
#include <CAN.h>
#include <RadarBaumer.hpp>

//--------------------- DEFINE -------------------------------
#define BLINK_PERIOD_MS       500

//---------------- GLOBAL VARIABLES --------------------------
bool ledState = false;
unsigned long lastTime = 0;
RadarBaumer radar;

//---------------------- SETUP -------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");

  // Initialise CAN1
  if(!can1.begin(250E3)){
    Serial.println("Starting CAN1 failed");
  }else{
    Serial.println("Starting CAN1 Succed");
    radar.attachCanInstance(&can1);
    can1.onReceive(radar.readHeight); // attach radarHeight to the receive callback
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

//----------------------- Loop ---------------------------
void loop() {
    //blink
    if(millis() > lastTime + BLINK_PERIOD_MS) {
      lastTime = millis();
      ledState = !ledState;
      digitalWrite(PB04, ledState);
    
    // Print Radar Data
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
    Serial.print("Canopy Confidence :");
    Serial.println(radar.canopyConfidence);
    Serial.print("Ground Confidence :");
    Serial.println(radar.groundConfidence);
    Serial.print("Canopy Distance [mm] :");
    Serial.println(radar.canopyDistance);
    Serial.print("Ground Distance [mm] :");
    Serial.println(radar.groudDistance);
    Serial.println(" ");
    }
}