#ifndef BERAS_PAYLOAD_HANDLER_h
#define BERAS_PAYLOAD_HANDLER_h

#include <Arduino.h>

class Beras_PayloadHandler
{
public:
    explicit Beras_PayloadHandler(const char separator);
    ~Beras_PayloadHandler();

    // To prevent exceptions from accessing random memory, use this function first before using any other function
    void loadPayload(const String *const payload);

    inline unsigned int getPayloadSize() const
    {
        return payloadSize;
    }

    void loadPayloadSize(const String *const payload);

    unsigned int countSeparator(const String *const payload) const;

    inline char getPayloadSeparator() const
    {
        return payloadSeparator;
    }

    void unloadPayload();

    const String &operator[](unsigned int index) const;

private:
    const char payloadSeparator;

    unsigned int payloadSize = 0;
    String *payloadArray = nullptr;
};

#endif