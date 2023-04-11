#ifndef RADAR_BAUMER_HPP
#define RADAR_BAUMER_HPP

#include <CAN.h>

enum RadarStatus{ //Check the datasheet for the specific meaning of the errors
    NO_ERROR,
    REVERSIBLE_ERROR,
    IRREVERSIBLE_ERROR
};

class RadarBaumer
{
    public:
    static void readHeight(int packetLength);
    static RadarStatus sensorStatus;
    static float targetConfidence;
    static float targetDistance;
    static float targetSpeed;
    static uint32_t canIDReceived;
    static uint8_t rawData[8];
 
    private:
    static CANSAME5x* _instanceCAN;
};

#endif // RADAR_BAUMER_HPP