#include <Arduino.h>
#include <CAN.h>
#include <RadarBaumer.hpp>
#include <Palpeur.hpp>

//--------------------- DEFINE -------------------------------
#define BLINK_PERIOD_MS                             500
#define PalpeurAddressCAN                           442
#define RadarAdressOnBoatCanLine                    443
const int shouldRadarRepeatHeightOnBoatCanLine =  true;
#define PALPEUR_CHIP_SELECT                        PB01 
#define PALPEUR_DATA                               PB03
#define PALPEUR_CLOCK                              PB02
#define PALPEUR_OFFSET                              722
#define F_VALUE_TO_RADIAN                           512  // Convert the feeler value to a radian value
#define FEELER_LENGTH                              1445
#define FIFTY_HERZ                                   19
#define BLINK_LED                                  PB05

//---------------- GLOBAL VARIABLES --------------------------
bool ledState = false;
unsigned long lastBlink = 0;
unsigned long lastPalpeurSend = 0;
RadarBaumer radar;
Palpeur palpeur(PALPEUR_OFFSET, FEELER_LENGTH, F_VALUE_TO_RADIAN, PALPEUR_CLOCK, PALPEUR_DATA, PALPEUR_CHIP_SELECT);

//---------------------- SETUP -------------------------------
void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");
    palpeur.init();

    // Initialise CAN1
    if(!can1.begin(250E3)){
        Serial.println("Starting CAN1 failed");
    }else{
        Serial.println("Starting CAN1 Succed");
        radar.attachRadarCanInstance(&can1);
        can1.onReceive(radar.readHeight); // attach radarHeight to the receive callback
    }

    // Initialise CAN0
    if(!can0.begin(1000E3)){
        Serial.println("Starting CAN0 failed");
    }else{
        Serial.println("Starting CAN0 Succed");
        palpeur.attachCANInstance(&can0, PalpeurAddressCAN);
        radar.attachBoatCanInstance(&can0,shouldRadarRepeatHeightOnBoatCanLine,RadarAdressOnBoatCanLine);
    }

    // Setup the blink LED
    pinMode(BLINK_LED, OUTPUT);
}

//----------------------- LOOP -------------------------------
void loop() {
    //blink and print
    if(millis() > lastBlink + BLINK_PERIOD_MS) {
        lastBlink = millis();
        ledState = !ledState;
        digitalWrite(BLINK_LED, ledState);

        // Print Radar Data
        Serial.print("packet ID : ");
        Serial.println(radar.canIDReceived,HEX);
        Serial.print("Raw data : ");
        for(int i = 0; i<8; i++){
            Serial.print(radar.rawData[i],HEX);
            Serial.print(" ");
        }
        Serial.println(" ");
        Serial.print("Radar Status :");
        Serial.println(radar.sensorStatus);
        Serial.print("Canopy Confidence :");
        Serial.println(radar.canopyConfidence);
        Serial.print("Ground Confidence :");
        Serial.println(radar.groundConfidence);
        Serial.print("Canopy Distance [mm] :");
        Serial.println(radar.canopyDistance);
        Serial.print("Ground Distance [mm] :");
        Serial.println(radar.groundDistance);
        Serial.print("palpeur height : ");
        Serial.println(palpeur.altitude);
        Serial.println("\n");
    }

    // Read and send plapeur
    if(millis() > lastPalpeurSend + FIFTY_HERZ){
        lastPalpeurSend = millis();
        palpeur.makeMeasurement();
        palpeur.sendHeightToCAN();
    }
}