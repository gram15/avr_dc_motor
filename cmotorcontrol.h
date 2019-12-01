#ifndef CMOTORCONTROL_H
#define CMOTORCONTROL_H

#include <Arduino.h>


#define SERIAL_BUFF_NUM 32


class cMotorControl
{
public:
    cMotorControl();

    int readSerialCmd();

    int cmdParser(char * szCmd);

private:
    // circular array for managing serial commands
    char szSerialBuffer[SERIAL_BUFF_NUM];
    int iHead;
    int iTail;
    int _pushBuffer(char *in, int iNum);
    int _popBuffer(char *out, int iNum);

};


#endif // CMOTORCONTROL_H
