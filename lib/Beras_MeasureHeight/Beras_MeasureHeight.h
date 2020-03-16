#ifndef BERAS_MEASURE_HEIGHT_h
#define BERAS_MEASURE_HEIGHT_h

#include <Arduino.h>
#include <pcf8574_esp.h>
#include <Adafruit_VL53L0X.h>

class Beras_MeasureHeight
{
public:
    Beras_MeasureHeight(PCF857x *expander, Adafruit_VL53L0X *sensor, unsigned long interval = 60000UL);

    bool measureHeight();

    // Use this function only when (currentSensorNumber >= VL53L0X_DEVICES)
    unsigned int getSumOfBuriedSensors() const;

    // Use this function only when (currentSensorNumber >= VL53L0X_DEVICES)
    void printRangeReadings() const;

    void resetMeasurement();

    inline bool getCurrentMeasuringStatus() const
    {
        return deviceIsMeasuring;
    }

    inline int getCurrentHeightStatus() const
    {
        return heightStatus;
    }

    void setHeightStatus(const int status);

    void setPreviousBuriedSensors(const unsigned int sensors);

    inline unsigned int getPreviousBuriedSensors()
    {
        return previousBuriedSensors;
    }

    inline unsigned long getMeasureInterval()
    {
        return measureInterval;
    }

    void setMeasureInterval(unsigned long interval);

    static const int HEIGHT_IS_SUFFICIENT = 1;
    static const int HEIGHT_IS_LOW = 0;
    static const int HEIGHT_IS_CRITICAL = -1;

    static const unsigned int VL53L0X_DEVICES = 5;
    static const unsigned int VL53L0X_PCF8574_PINS[];

private:
    // Made for lag prevention as other processes can be made to know when a measurement is running via getCurrentMeasuringStatus()
    void measureRangeMillimeterWithStatusChange(const unsigned int sensorNumber);
    void measureRangeMillimeter(const unsigned int sensorNumber);

    unsigned long measureInterval;

    unsigned int previousBuriedSensors = 0;

    int heightStatus = 1;

    bool deviceIsMeasuring = false;

    unsigned int currentSensorNumber = 0;
    unsigned int rangeReadings[VL53L0X_DEVICES]{};

    PCF857x *ioExpander;
    Adafruit_VL53L0X *rangingSensor;
};

#endif