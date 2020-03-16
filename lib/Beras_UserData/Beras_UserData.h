#ifndef BERAS_USER_DATA_h
#define BERAS_USER_DATA_h

#include <Arduino.h>

class Beras_UserData
{
public:
    ~Beras_UserData();

    inline void setUserData(const String nm, const unsigned int qt)
    {
        setName(nm);
        setStapleQuota(qt);
    }

    const String &getName() const;

    inline unsigned int getStapleQuota() const
    {
        return quota;
    }

    inline void clearUserData()
    {
        clearName();
        resetQuota();
    }

    void setName(const String nm);
    void clearName();

    void setStapleQuota(const unsigned int qt);
    void resetQuota();

private:
    const String *name = nullptr;
    unsigned int quota = 0;
};

#endif