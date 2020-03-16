#include <Arduino.h>
#include "Beras_StapleDivider.h"

Beras_StapleDivider::~Beras_StapleDivider()
{
    clearStapleArray();
}

void Beras_StapleDivider::loadStapleQuota(const unsigned int quota, const unsigned int divider)
{
    if (divider == 0)
        return;

    clearStapleArray();

    if (quota % divider == 0)
    {
        stapleArraySize = quota / divider;
    }
    else
    {
        stapleArraySize = (quota / divider) + 1;
    }

    stapleArray = new unsigned int[getStapleArraySize()];

    if (getStapleArraySize() == 1)
    {
        stapleArray[0] = quota;
    }
    else
    {
        if (quota % divider == 0)
        {
            for (size_t stapleIndex = 0; stapleIndex < getStapleArraySize(); ++stapleIndex)
            {
                stapleArray[stapleIndex] = divider;
            }
        }
        else
        {
            for (size_t stapleIndex = 0; stapleIndex < getStapleArraySize(); ++stapleIndex)
            {
                if (stapleIndex) // stapleIndex != 0
                {
                    stapleArray[stapleIndex] = divider;
                }
                else // stapleIndex == 0
                {
                    stapleArray[stapleIndex] = quota % divider;
                }
            }
        }
    }
}

void Beras_StapleDivider::printStapleArray() const
{
    Serial.print("Staple array: ");

    for (size_t stapleIndex = 0; stapleIndex < getStapleArraySize(); ++stapleIndex)
    {
        Serial.print(operator[](stapleIndex));

        if (stapleIndex < getStapleArraySize() - 1)
        {
            Serial.print(' ');
        }
    }

    Serial.println();
}

void Beras_StapleDivider::clearStapleArray()
{
    stapleArraySize = 0;

    delete[] stapleArray;
    stapleArray = nullptr;
}

unsigned int Beras_StapleDivider::operator[](unsigned int index) const
{
    if (stapleArray == nullptr || index >= getStapleArraySize())
    {
        return 0;
    }

    return stapleArray[index];
}