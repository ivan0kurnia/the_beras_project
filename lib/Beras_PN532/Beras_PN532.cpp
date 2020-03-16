#include <Arduino.h>
#include <Adafruit_PN532.h>

#include "Beras_PN532.h"

Beras_PN532::Beras_PN532(const unsigned int irq, const unsigned int reset, const unsigned long interval) : Adafruit_PN532(irq, reset), readingInterval{interval}
{
}

void Beras_PN532::readRFID(const unsigned int timeout)
{
    readStatus = readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);
}

void Beras_PN532::setReadingInterval(const unsigned long interval)
{
    readingInterval = interval;
}

void Beras_PN532::resetReadStatus()
{
    readStatus = false;
}

String Beras_PN532::getLastUid() const
{
    String s;

    for (size_t n{0}; n < UID_MAX; ++n)
    {
        if (uid[n] < 0x10)
        {
            s += '0';
            s += String(uid[n], HEX);
        }
        else
        {
            s += String(uid[n], HEX);
        }
    }

    s.toUpperCase();

    return s;
}