/*
Project:	dc_motor
Board:		uno
MCU:		16MHzatmega328
FRENQUENCY:	16000000

Created using QtCreator
*/

#include <Arduino.h>
#include "cmotorcontrol.h"

//#define LED_PIN     13
//#define DELAY_MS    1000

int main()
{
    // Initialize Arduino Librairies
    init();

    //pinMode(LED_PIN, OUTPUT);
    setup();

    while (1)
    {
        //digitalWrite(LED_PIN, HIGH);
        //delay(DELAY_MS);
        //digitalWrite(LED_PIN, LOW);
        //delay(DELAY_MS);
        loop();
        if (serialEventRun) serialEventRun();
    }

}

