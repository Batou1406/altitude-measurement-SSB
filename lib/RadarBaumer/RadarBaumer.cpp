#ifndef RADAR_BAUMER_CPP
#define RADAR_BAUMER_CPP

#include <RadarBaumer.hpp>

CANSAME5x* RadarBaumer::_instanceCAN = &can1;

RadarStatus RadarBaumer::sensorStatus = NO_ERROR ;
float RadarBaumer::targetConfidence = 0;
float RadarBaumer::targetDistance = 0;
float RadarBaumer::targetSpeed = 0;
uint32_t RadarBaumer::canIDReceived = 0;
uint8_t RadarBaumer::rawData[8] = {0,0,0,0,0,0,0,0};

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

        // Declare temp storage variable
        uint8_t tempByte = 0;
        uint8_t sensorStatusRaw = 0;
        uint8_t targetConfidenceRaw = 0;
        uint32_t targetDistanceRaw = 0;
        int16_t targetSpeedRaw = 0;

        // Byte 0,1 : Target Speed
        tempByte = _instanceCAN->read();
        rawData[0] = tempByte;
        targetSpeedRaw = tempByte;
        
        tempByte = _instanceCAN->read();
        rawData[1] = tempByte;
        targetSpeedRaw += tempByte << 8;

        targetSpeed = targetSpeedRaw*1.f + 32768; // mm/s

        // Byte 7 : Unused
        rawData[7] = _instanceCAN->read();

        // Byte 2,3,4 : Target Distance
        tempByte = _instanceCAN->read();
        rawData[2] = tempByte;
        targetDistanceRaw = tempByte;

        tempByte = _instanceCAN->read();
        rawData[3] = tempByte;
        targetDistanceRaw += tempByte << 8;
        
        tempByte = _instanceCAN->read();
        rawData[4] = tempByte;
        targetDistanceRaw += tempByte << 16;
        
        targetDistance = targetDistanceRaw*0.1f; //mm

        // Byte 5 : Target confidence
        tempByte = _instanceCAN->read();
        rawData[5] = tempByte;
        targetConfidenceRaw = tempByte;

        targetConfidence = targetConfidenceRaw*0.01f;

       // Byte 6 : Sensor Status
        tempByte = _instanceCAN->read();
        rawData[6] = tempByte;
        sensorStatusRaw = tempByte;

        sensorStatus = RadarStatus(sensorStatusRaw);



        // Byte 7 : Unused
        rawData[7] = _instanceCAN->read();
        
        
        /*
        // Byte 0 : Sensor Status
        tempByte = _instanceCAN->read();
        sensorStatusRaw = tempByte;
        sensorStatus = RadarStatus(sensorStatusRaw);

        // Byte 1 : Target confidence
        tempByte = _instanceCAN->read();
        targetConfidenceRaw = tempByte;
        targetConfidence = targetConfidenceRaw*0.01f;

        // Byte 2,3,4 : Target Distance
        tempByte = _instanceCAN->read();
        targetDistanceRaw = tempByte;
        Serial.print(tempByte);
        Serial.print(" ");
        tempByte = _instanceCAN->read();
        targetDistanceRaw += tempByte << 8;
        Serial.print(tempByte);
        Serial.print(" ");
        tempByte = _instanceCAN->read();
        targetDistanceRaw += tempByte << 16;
        Serial.println(tempByte);
        targetDistance = targetConfidenceRaw*0.1f; //mm

        // Byte 5 : Unused
        _instanceCAN->read();

        // Byte 6,7 : Target Speed
        tempByte = _instanceCAN->read();
        targetSpeedRaw = tempByte;
        tempByte = _instanceCAN->read();
        targetSpeedRaw += tempByte << 8;
        targetSpeed = targetSpeedRaw*1.f; // mm/s
        */
    }else{
        Serial.println("Wrong packet ID...");
    }

}

#endif // RADAR_BAUMER_CPP