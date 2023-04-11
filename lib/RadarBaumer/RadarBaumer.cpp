#ifndef RADAR_BAUMER_CPP
#define RADAR_BAUMER_CPP

#include <RadarBaumer.hpp>

CANSAME5x* RadarBaumer::_instanceCAN = &can1;

RadarStatus RadarBaumer::sensorStatus = NO_ERROR ;
float RadarBaumer::targetConfidence = 0;
float RadarBaumer::targetDistance = 0;
float RadarBaumer::targetSpeed = 0;

void RadarBaumer::readHeight(int packetLength){
    if(packetLength != 8){
        Serial.println("Oupsi, not a correct Baumer frame");
        return;
    }

    uint8_t tempByte = 0;
    uint8_t sensorStatusRaw = 0;
    uint8_t targetConfidenceRaw = 0;
    uint32_t targetDistanceRaw = 0;
    uint16_t targetSpeedRaw = 0;

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
    tempByte = _instanceCAN->read();
    targetDistanceRaw += tempByte << 8;
    tempByte = _instanceCAN->read();
    targetDistanceRaw += tempByte << 16;
    targetDistance = targetConfidenceRaw*0.1f; //mm

    // Byte 5 : Unused
    _instanceCAN->read();

    // Byte 6,7 : Target Speed
    tempByte = _instanceCAN->read();
    targetSpeedRaw = tempByte;
    tempByte = _instanceCAN->read();
    targetSpeedRaw += tempByte << 8;
    targetSpeed = targetSpeedRaw*1.f; // mm/s
}

#endif // RADAR_BAUMER_CPP