#ifndef RADAR_BAUMER_CPP
#define RADAR_BAUMER_CPP

#include <RadarBaumer.hpp>

// Init object static variables
CANSAME5x* RadarBaumer::_instanceCAN = nullptr;
CANSAME5x* RadarBaumer::_instanceCAN_Boat = nullptr;
int RadarBaumer::_radarAdressOnBoatCAN = 0;
RadarStatus RadarBaumer::sensorStatus = NO_ERROR ;
uint8_t RadarBaumer::canopyConfidence = 0;
uint8_t RadarBaumer::groundConfidence = 0;
uint16_t RadarBaumer::canopyDistance = 0;
uint16_t RadarBaumer::groundDistance = 0;
uint32_t RadarBaumer::canIDReceived = 0;
uint8_t RadarBaumer::rawData[8] = {0,0,0,0,0,0,0,0};
bool RadarBaumer::_repeatRadarHeightToBoatCAN = false;

/*  funtion RadarBaumer::attachRadarCanInstance(CANSAME5x* instanceCAN_radar)
 *
 *  Attach the CAN instance where the radar is conncted : makes the library Hardware independent
 */
void RadarBaumer::attachRadarCanInstance(CANSAME5x* instanceCAN_Radar){
    _instanceCAN = instanceCAN_Radar;
}

/*  function RadarBaumer::attachBoatCanInstance(CANSAME5x* instanceCAN_Boat)
 *  
 *  Attach CAN instance to where the rest of the boat is connected
 *  Moreover, allows or not the data read from the radar to be repetead to the Boat CAN line
 */
void RadarBaumer::attachBoatCanInstance(CANSAME5x* instanceCAN_Boat, bool repeatRadarHeightToBoatCAN, int radarAdressOnBoatCAN){
    _instanceCAN_Boat = instanceCAN_Boat;
    _repeatRadarHeightToBoatCAN = repeatRadarHeightToBoatCAN;
    _radarAdressOnBoatCAN = radarAdressOnBoatCAN;
}

/*  function RadarBaumer::readHeight(int packetLength)
 *  
 *  function to be attached to the can message On received callback
 *  Each time a message is received from the radar, decode it, and store the new value
 *  in the radarBaumer object
 */
void RadarBaumer::readHeight(int packetLength){

    // Read the packet ID
    canIDReceived = _instanceCAN->packetId();

    // Check that correct packet ID has been receiver
    if(canIDReceived == 0x18C0FF80){
        // Check that payload has the correct length
        if(packetLength != 8){
            Serial.println("Oupsi, not a correct Baumer frame");
            return;
        }

        // Lecture du message CAN
        uint8_t i = 0;
        while(_instanceCAN->available()){
            rawData[i] = _instanceCAN->read();
            i += 1;
        }
        // Check for valid frame
        if(i != 8){
            Serial.print("Oupsi... Wrong frame received...");
            return;
        }

        // Byte 0, bit 6-7 : Sensor Status
        sensorStatus = RadarStatus(rawData[0] & 0b00000011);

        // Byte 0-1, bit 0-5, 15 : canopy confidence
        canopyConfidence = (rawData[0] >> 2) + ((rawData[1] & 0b00000001) << 6);

        // Byte 1, bit 8-14 : Ground Confidence
        groundConfidence = (rawData[1] >> 1);

        // Byte 2-3 : Canopy Distance
        canopyDistance = (uint16_t(rawData[3]) << 8) + uint16_t(rawData[2]);

        // Byte 4-5 : Ground Distance
        groundDistance = (uint16_t(rawData[5]) << 8) + uint16_t(rawData[4]);

        if(_repeatRadarHeightToBoatCAN){
            sendHeightToCANBoat();
        }
    }else{
        Serial.println("Wrong packet ID...");
    }

}

/*  function RadarBaumer::sendHeightToCANBoat()
 *  
 *  Send the radar measurement to the boat CAN line
 */
void RadarBaumer::sendHeightToCANBoat(){
    _instanceCAN_Boat->beginPacket(_radarAdressOnBoatCAN);
    _instanceCAN_Boat->write(sensorStatus & 0x00FF);
    _instanceCAN_Boat->write(canopyConfidence);
    _instanceCAN_Boat->write(groundConfidence);
    _instanceCAN_Boat->write((canopyDistance & 0x00FF));
    _instanceCAN_Boat->write((canopyDistance & 0xFF00) >> 8);
    _instanceCAN_Boat->write((groundDistance & 0x00FF));
    _instanceCAN_Boat->write((groundDistance & 0xFF00) >> 8);
    _instanceCAN_Boat->endPacket();
}
#endif // RADAR_BAUMER_CPP