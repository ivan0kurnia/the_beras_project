#ifndef DEVICE_STATE_h
#define DEVICE_STATE_h

//Please edit these concurrently with the deviceStates[] array in the .cpp file
#define BERAS_READY 0
#define BERAS_PREPARING 1
#define BERAS_MEASURING 2
#define BERAS_TRANSFERRING 3

class Beras_DeviceState
{
    typedef const char *const device_state_t;

public:
    inline void resetDeviceState()
    {
        setHowManyTimesToTransfer(0);
        setCurrentDeviceState(0);
    }

    void setCurrentDeviceState(const unsigned int stateNumber);

    inline unsigned int getCurrentDeviceState() const
    {
        return currentDeviceState;
    }

    inline void goToNextState()
    {
        setCurrentDeviceState(getCurrentDeviceState() + 1);
    }

    inline void goToPreviousState()
    {
        setCurrentDeviceState(getCurrentDeviceState() - 1);
    }

    inline device_state_t getCurrentDeviceStateName()
    {
        return getDeviceStateName(getCurrentDeviceState());
    }

    void setHowManyTimesToTransfer(const unsigned int times);

    inline unsigned int getHowManyMoreTimesToTransfer() const
    {
        return moreTimesToTransfer;
    }

    void tellThisObjectOneTransferIsDone();

    static unsigned int getTotalDeviceStates();
    static device_state_t getDeviceStateName(const unsigned int stateNumber);

private:
    unsigned int moreTimesToTransfer = 0;

    static device_state_t deviceStates[];
    unsigned int currentDeviceState = 0;
};

#endif