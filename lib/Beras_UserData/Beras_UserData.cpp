#include <Arduino.h>
#include "Beras_UserData.h"

Beras_UserData::~Beras_UserData()
{
    clearUserData();
}

const String &Beras_UserData::getName() const
{
    static const String NAME_UNSET PROGMEM{"NAME_UNSET"};

    if (name == nullptr)
    {
        return NAME_UNSET;
    }

    return *name;
}

void Beras_UserData::setName(const String nm)
{
    clearName();
    
    name = new const String(nm);
}

void Beras_UserData::setStapleQuota(const unsigned int qt)
{
    resetQuota();
    
    quota = qt;
}

void Beras_UserData::clearName()
{
    delete name;
    name = nullptr;
}

void Beras_UserData::resetQuota()
{
    quota = 0;
}