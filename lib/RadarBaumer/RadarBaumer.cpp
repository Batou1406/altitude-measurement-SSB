#ifndef RADAR_BAUMER_CPP
#define RADAR_BAUMER_CPP

#include <RadarBaumer.hpp>

CANSAME5x* RadarBaumer::_instanceCAN = nullptr;

RadarStatus RadarBaumer::sensorStatus = NO_ERROR ;
uint8_t RadarBaumer::canopyConfidence = 0;
uint8_t RadarBaumer::groundConfidence = 0;
uint16_t RadarBaumer::canopyDistance = 0;
uint16_t RadarBaumer::groudDistance = 0;
uint32_t RadarBaumer::canIDReceived = 0;
uint8_t RadarBaumer::rawData[8] = {0,0,0,0,0,0,0,0};

void RadarBaumer::attachCanInstance(CANSAME5x* instanceCAN){
    _instanceCAN = instanceCAN;
}

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

        // Byte 4-5 : Ground Distane
        groudDistance = (uint16_t(rawData[5]) << 8) + uint16_t(rawData[4]);

    }else{
        Serial.println("Wrong packet ID...");
    }

}

#endif // RADAR_BAUMER_CPP