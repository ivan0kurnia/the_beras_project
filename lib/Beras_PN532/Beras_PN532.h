#ifndef BERAS_PN532_h
#define BERAS_PN532_h

#include <Arduino.h>
#include <Adafruit_PN532.h>

class Beras_PN532 : public Adafruit_PN532
{
public:
    Beras_PN532(const unsigned int irq, const unsigned int reset, const unsigned long interval = 3000U);

    void readRFID(const unsigned int timeout = 0U);

    void setReadingInterval(const unsigned long interval);

    inline unsigned long getReadingInterval() const
    {
        return readingInterval;
    }

    inline bool getReadStatus() const
    {
        return readStatus;
    }

    void resetReadStatus();

    inline unsigned int getLastUidLength() const
    {
        return uidLength;
    }

    // Always returns a String object of 14 hex digits of UID due to the existence of cards with 7 bytes of UID
    String getLastUid() const;

    static const unsigned int INTERRUPT_PIN = 4;
    static const unsigned int RESET_PIN = 0;

    static const unsigned int UID_MAX = 7;

private:
    unsigned long readingInterval;

    bool readStatus;

    uint8_t uid[UID_MAX]{};
    uint8_t uidLength{};
};

#endif