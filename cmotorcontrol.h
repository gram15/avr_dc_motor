#ifndef CMOTORCONTROL_H
#define CMOTORCONTROL_H

#include <Arduino.h>


#define SERIAL_BUFF_NUM 64


class cMotorControl
{
public:
    cMotorControl();

    int readSerialCmd();

    int cmdParser();

    enum eCOMMAND {
        eCMD_START,
        eCMD_STOP,
        eCMD_READ,
        eCMD_SETTRG,

        eCMD_NUM
    };



private:
    // circular array for managing serial commands
    char szSerialBuffer[SERIAL_BUFF_NUM];
    char szCmd[4];
    int iCmdSlider;
    int iHead;
    int iTail;
    bool bErrorActive;

    int runCmd();

};


#endif // CMOTORCONTROL_H
