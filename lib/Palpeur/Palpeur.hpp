/*  Written by Baptiste Savioz
 *  april 2023
 *  
 *  Library made for the encoder EMS22A50-B28-LS6
 *  datasheet : https://pdf.utmel.com/r/datasheets/bournsinc-ems22a50b28ls6-datasheets-5164.pdf
 */

#ifndef PALPEUR_HPP
#define PALPEUR_HPP

#include <CAN.h>

class Palpeur
{
    public:
    Palpeur(int offset, int feelerLength, int fValueToRad, int clockPin, int dataPin, int chipSelectPin); // constructeur
    void init();
    void makeMeasurement();
    void attachCANInstance(CANSAME5x* canInstance, int palpeurCAN_ID);
    void sendHeightToCAN();
    int16_t altitude = 0;
 
    private:
    void delayOneMicroseconds();
    int _clockPin = 0;
    int _dataPin = 0;
    int _chipSelectPin = 0;
    int _offset = 0;
    int _feelerLength = 0;
    int _fValueToRad = 0;
    volatile uint16_t lastValue = 0;
    CANSAME5x* _canInstance = nullptr;
    int _palpeurCAN_ID = 0;
};

#endif // PALPEUR_HPP