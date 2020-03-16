#include <Arduino.h>
#include "Beras_PayloadHandler.h"

Beras_PayloadHandler::Beras_PayloadHandler(const char separator) : payloadSeparator{separator}
{
}

Beras_PayloadHandler::~Beras_PayloadHandler()
{
    unloadPayload();
}

void Beras_PayloadHandler::loadPayload(const String *const payload)
{
    unloadPayload();

    loadPayloadSize(payload);

    payloadArray = new String[getPayloadSize()];

    size_t payloadArrayIndex = 0;
    for (size_t payloadIndex = 0; payloadIndex < payload->length(); ++payloadIndex)
    {
        char payloadIndexedCharacter = payload->operator[](payloadIndex);

        if (payloadIndexedCharacter == getPayloadSeparator())
        {
            ++payloadArrayIndex;
        }
        else
        {
            payloadArray[payloadArrayIndex] += payloadIndexedCharacter;
        }
    }
}

void Beras_PayloadHandler::loadPayloadSize(const String *const payload)
{
    payloadSize = countSeparator(payload) + 1;
}

unsigned int Beras_PayloadHandler::countSeparator(const String *const payload) const
{
    unsigned int separatorCount = 0;

    for (size_t payloadIndex = 0; payloadIndex < payload->length(); ++payloadIndex)
    {
        if (payload->operator[](payloadIndex) == getPayloadSeparator())
        {
            ++separatorCount;
        }
    }

    return separatorCount;
}

void Beras_PayloadHandler::unloadPayload()
{
    payloadSize = 0;

    delete[] payloadArray;
    payloadArray = nullptr;
}

const String &Beras_PayloadHandler::operator[](unsigned int index) const
{
    static const String PAYLOAD_NOT_LOADED PROGMEM{"PAYLOAD_NOT_LOADED"};
    static const String ARRAY_OUT_OF_BOUNDS PROGMEM{"ARRAY_OUT_OF_BOUNDS"};

    if (payloadArray == nullptr)
    {
        return PAYLOAD_NOT_LOADED;
    }

    if (index >= getPayloadSize())
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    return payloadArray[index];
}