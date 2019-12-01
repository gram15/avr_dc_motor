#include "cmotorcontrol.h"

#define LED_PIN     13
#define DELAY_MS    1000

#define MOTOR_FW    10
#define MOTOR_BW    9
#define MOTOR_SENSE_A A0
#define MOTOR_SENSE_B A1

#define syscall

int iSlopeStatus = 0;
bool bLedStatus = HIGH;
int iVoltageA = 0;
int iVoltageB = 0;
char TCNT2init= 130;

cMotorControl::cMotorControl() :
    iHead(0)
  , iTail(0)
  , iCmdSlider(0)
  , bErrorActive(false)
{
    // init the serial buffer
    memset(&szSerialBuffer, 0, SERIAL_BUFF_NUM);
    memset(&szCmd, 0, 4);
}

int cMotorControl::readSerialCmd()
{
    unsigned char iAvailableBytes = Serial.available();
    if(iAvailableBytes > 0)
    {
        for(; iAvailableBytes; iAvailableBytes--)
        {
            szSerialBuffer[iHead++] = Serial.read();
            iHead %= SERIAL_BUFF_NUM;
        }
    }
    return 0;
}

int cMotorControl::cmdParser()
{
    // available bytes in buffer: (SERIAL_BUFF_NUM + head - tail )%SERIAL_BUFF_NUM
    unsigned char iBufferContent = (SERIAL_BUFF_NUM + iHead - iTail) % SERIAL_BUFF_NUM;
    if( iBufferContent > 0 )
    {
        char bReadByte = szSerialBuffer[iTail++];
        switch(bReadByte)
        {
        case '#':
            memset(&szCmd, 0, 4);
            iCmdSlider = 0;
            bErrorActive = false;
            break;

        case '@':
            if(!bErrorActive) runCmd();
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            if(iCmdSlider < 2)
            {
                szCmd[iCmdSlider++] = bReadByte;
            }
            else
            {
                bErrorActive = true;
            }
            break;

        default:
            // not allowed char, do nothing or raise error?
            bErrorActive = true;
            break;
        }
        iTail %= SERIAL_BUFF_NUM;
    }
    return 0;
}




//************************************//

void motor_command(int iSpeed)
{
    int iDirection = iSpeed/abs(iSpeed);
    if(iDirection == 1)
    {
        analogWrite(MOTOR_FW, abs(iSpeed));
        analogWrite(MOTOR_BW, 0);
    }
    else
    {
        analogWrite(MOTOR_FW, 0);
        analogWrite(MOTOR_BW, abs(iSpeed));

    }
}

//change the 0 to 1 for timer1 and 2 for timer2
ISR(TIMER2_OVF_vect)
{
    iVoltageA = analogRead(MOTOR_SENSE_A);
    iVoltageB = analogRead(MOTOR_SENSE_B);
    Serial.println(iVoltageA-iVoltageB);
}


void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(MOTOR_FW, OUTPUT);
    pinMode(MOTOR_BW, OUTPUT);
    pinMode(MOTOR_SENSE_A, INPUT);
    pinMode(MOTOR_SENSE_B, INPUT);
    iSlopeStatus = -255;

    // TIMER2_OVF_vect (Timer2 overflow) is fired with freq:
    // Freq_OVF = 16000000/(scale*(255-TCNT2init)) Hz
    // Square wave( _-_-_ ) on pin OVF_Pin has:
    // Freq_PIN = FreqOVF/2
    cli();
    TCCR2B = 0x00; // No clock source (Timer/Counter stopped)

    TCNT2 = TCNT2init; // Register : the Timer/Counter (TCNT2) and Output Compare Register (OCR2A and OCR2B) are 8-bit
                       // Reset Timer Count

    TCCR2A = 0x00; // TCCR2A - Timer/Counter Control Register A
                   // All bits to zero -> Normal operation

    TCCR2B |= (1<<CS22)|(1<<CS20); // Prescale 128 (Timer/Counter started)
    TCCR2B &= ~(1<<CS21);          // CS22=1 CS21=0 CS20=1 -> prescale = 128

    TIMSK2 |= (1<<TOIE2); // TIMSK2 - Timer/Counter2 Interrupt Mask Register
    // Bit 0 - TOIE2: Timer/Counter2 Overflow Interrupt Enable
    sei();

    Serial.begin(115200);
}

void loop()
{
    digitalWrite(LED_PIN, bLedStatus);
    motor_command(iSlopeStatus);
    delay(DELAY_MS);

    //digitalWrite(LED_PIN, LOW);
    //delay(DELAY_MS);

    bLedStatus = !bLedStatus;

    //    if(bLedStatus == HIGH)
    //    {
    //        bLedStatus = LOW;
    //    }
    //    else
    //    {
    //        bLedStatus = HIGH;
    //    }

    iSlopeStatus += 32;
    if(iSlopeStatus >= 255)
    {
        iSlopeStatus = -255;
    }
}
