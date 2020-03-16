#ifndef BERAS_STAPLE_DIVIDER_h
#define BERAS_STAPLE_DIVIDER_h

#include <Arduino.h>

class Beras_StapleDivider
{
public:
    ~Beras_StapleDivider();

    inline unsigned int getStapleArraySize() const
    {
        return stapleArraySize;
    }

    void loadStapleQuota(const unsigned int quota, const unsigned int divider);

    void printStapleArray() const;

    void clearStapleArray();

    unsigned int operator[](unsigned int index) const;

private:
    unsigned int stapleArraySize = 0;
    unsigned int *stapleArray = nullptr;
};

#endif