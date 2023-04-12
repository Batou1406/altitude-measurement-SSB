/*  Written by Baptiste Savioz
 *  april 2023
 *  
 *  Library made for the radar Baumer R600V.DAE0
 */

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
    void attachCanInstance(CANSAME5x* instanceCAN);
    static void readHeight(int packetLength);
    static RadarStatus sensorStatus;
    static uint8_t canopyConfidence;
    static uint8_t groundConfidence;
    static uint16_t canopyDistance;
    static uint16_t groudDistance;
    static uint32_t canIDReceived;
    static uint8_t rawData[8];
 
    private:
    static CANSAME5x* _instanceCAN;
};

#endif // RADAR_BAUMER_HPP