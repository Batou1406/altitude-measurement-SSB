#include <Palpeur.hpp>

Palpeur::Palpeur(int offset, int feelerLength, int fValueToRad, int clockPin, int dataPin, int chipSelectPin){
    _offset = offset;
    _feelerLength = feelerLength;
    _fValueToRad = fValueToRad;
    _clockPin = clockPin;
    _dataPin = dataPin;
    _chipSelectPin = chipSelectPin;
}

void Palpeur::init(){
    // Setup correct Pin Mode
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, INPUT);
    pinMode(_chipSelectPin, OUTPUT);
}

void Palpeur::makeMeasurement(){
    uint16_t absoluteValue = 0;
    bool parity;
    bool msgParity;

    //chip select
    digitalWrite(_chipSelectPin, LOW);
    delayOneMicroseconds();

    for (int i = 0; i < 15; i++) {
        msgParity = 1;

        //clock tick
        digitalWrite(_clockPin, LOW);
        delayOneMicroseconds();
        digitalWrite(_clockPin, HIGH);
        delayOneMicroseconds();

        bool data_1 = digitalRead(_dataPin);
        //Serial.print(data_1);

        // Parity calculation
        if (data_1) {
            msgParity = !msgParity;
        }
        if (i < 10) {
            absoluteValue += (data_1 << (9 - i));
        }
        else {
            switch (i) {
                case 15:
                    parity = data_1;
                    break;
            }
        }
    }
    //chip unselect
    delayOneMicroseconds();
    digitalWrite(_chipSelectPin, HIGH);

    if ((parity == msgParity)) {
        lastValue = 0xFFFF;
        Serial.println("Err : Incorrect parity.");
    }
    else {
        lastValue = absoluteValue;
    }

    float angle;
    angle = lastValue - _offset;
    angle    = angle * PI / _fValueToRad;

    altitude = _feelerLength * sin(angle) - 100;
}

void Palpeur::delayOneMicroseconds()
{
    uint32_t n = 20;

    for(;n;n--){
        __asm__ __volatile("");
    }
}

void Palpeur::attachCANInstance(CANSAME5x* canInstance, int palpeurCAN_ID){
    _canInstance = canInstance;
    _palpeurCAN_ID = palpeurCAN_ID;
}

void Palpeur::sendHeightToCAN(){
    _canInstance->beginPacket(_palpeurCAN_ID);
    _canInstance->write(altitude & 0x00FF);
    _canInstance->write((altitude & 0xFF00) >> 8);
    _canInstance->endPacket();
}