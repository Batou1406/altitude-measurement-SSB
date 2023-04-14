# Altitude measurement - radar Baumer and Palpeur

This repo is dedicated to log measurement made with the altitude radar Baumer, to make height measurement with the palpeur and its rotary encoder and to send all these data to the boat CAN line. It use the custom MCU V2.2 from SSB.

The CAN line from the Baumer works at 250kbit/s with the protocol CAN J1939  
datasheet : https://www.baumer.com/medias/__secure__/en_BA_R600V.DAE0_manual_1.6.pdf?mediaPK=8990537383966

The CAN line from the boat works at 1Mbit/s with the protocol CANopen

The rotary encoder is the EMS22A50-B28-LS6  
datasheet : https://pdf.utmel.com/r/datasheets/bournsinc-ems22a50b28ls6-datasheets-5164.pdf
