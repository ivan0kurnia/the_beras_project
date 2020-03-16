#include <Arduino.h>
#include "Beras_DeviceState.h"

// Please edit these concurrently with the #define preprocessing directives in the .h file.
// This C-string array is made to mimic enumeration, except you can print the value out of it.
Beras_DeviceState::device_state_t Beras_DeviceState::deviceStates[] PROGMEM{
    "BERAS_READY",       // 0
    "BERAS_PREPARING",   // 1
    "BERAS_MEASURING",   // 2
    "BERAS_TRANSFERRING" // 3
};

void Beras_DeviceState::setCurrentDeviceState(const unsigned int stateNumber)
{
    if (stateNumber >= getTotalDeviceStates())
    {
        currentDeviceState = 0;
    }
    else
    {
        currentDeviceState = stateNumber;
    }
}

Beras_DeviceState::device_state_t Beras_DeviceState::getDeviceStateName(const unsigned int stateNumber)
{
    static device_state_t ARRAY_OUT_OF_BOUNDS PROGMEM = "ARRAY_OUT_OF_BOUNDS";

    if (stateNumber < sizeof(deviceStates) / sizeof(device_state_t))
    {
        return deviceStates[stateNumber];
    }

    return ARRAY_OUT_OF_BOUNDS;
}

unsigned int Beras_DeviceState::getTotalDeviceStates()
{
    return sizeof(deviceStates) / sizeof(device_state_t);
}

void Beras_DeviceState::setHowManyTimesToTransfer(const unsigned int times)
{
    // Prevents underflow by subtracting the variable only when its not zero
    if (times)
    {
        moreTimesToTransfer = times - 1;
    }
    else
    {
        moreTimesToTransfer = 0;
    }
}

void Beras_DeviceState::tellThisObjectOneTransferIsDone()
{
    // Prevents underflow by decrementing the variable only when its not zero
    if (getHowManyMoreTimesToTransfer())
    {
        --moreTimesToTransfer;
    }
}