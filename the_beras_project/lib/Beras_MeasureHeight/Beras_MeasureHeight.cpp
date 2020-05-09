#include <Arduino.h>
#include <pcf8574_esp.h>
#include <Adafruit_VL53L0X.h>

#include "Beras_MeasureHeight.h"

const unsigned int Beras_MeasureHeight::VL53L0X_PCF8574_PINS[VL53L0X_DEVICES]{1, 2, 3, 4, 5};

Beras_MeasureHeight::Beras_MeasureHeight(PCF857x *expander, Adafruit_VL53L0X *sensor, unsigned long interval) : measureInterval{interval}, ioExpander{expander}, rangingSensor{sensor}
{
}

bool Beras_MeasureHeight::measureHeight()
{
    // Will forever return true unless resetMeasurement() is called
    if (currentSensorNumber >= VL53L0X_DEVICES)
    {
        // printRangeReadings();
        return true;
    }

    measureRangeMillimeterWithStatusChange(currentSensorNumber);
    ++currentSensorNumber;

    return false;
}

unsigned int Beras_MeasureHeight::getSumOfBuriedSensors() const
{
    unsigned int sum = 0;

    for (size_t sensorNumber = 0; sensorNumber < VL53L0X_DEVICES; ++sensorNumber)
    {
        // 80 millimeters is twice the number of the sensor's minimum range
        if (rangeReadings[sensorNumber] < 350)
        {
            ++sum;
        }
    }

    return sum;
}

void Beras_MeasureHeight::printRangeReadings() const
{
    Serial.print(F("[M] VL53L0X range readings: "));

    for (size_t sensorNumber = 0; sensorNumber < VL53L0X_DEVICES; ++sensorNumber)
    {
        Serial.print(rangeReadings[sensorNumber]);

        if (sensorNumber < VL53L0X_DEVICES - 1)
        {
            Serial.print(' ');
        }
    }

    Serial.println();
}

void Beras_MeasureHeight::resetMeasurement()
{
    currentSensorNumber = 0;

    for (size_t sensorNumber = 0; sensorNumber < VL53L0X_DEVICES; ++sensorNumber)
    {
        rangeReadings[sensorNumber] = 0;
    }
}

void Beras_MeasureHeight::measureRangeMillimeterWithStatusChange(const unsigned int sensorNumber)
{
    deviceIsMeasuring = true;

    measureRangeMillimeter(sensorNumber);

    deviceIsMeasuring = false;
}

void Beras_MeasureHeight::measureRangeMillimeter(const unsigned int sensorNumber)
{
    if (sensorNumber >= VL53L0X_DEVICES)
        return;

    for (size_t pin = 0; pin < VL53L0X_DEVICES; ++pin)
    {
        ioExpander->write(VL53L0X_PCF8574_PINS[pin], (sensorNumber == pin));
    }

    if (!rangingSensor->begin())
    {
        Serial.print(F("[M][E] Sensor number "));
        Serial.print(sensorNumber);
        Serial.print(F(" (counted from 0) failed to initiate."));
        Serial.println();

        rangeReadings[sensorNumber] = 0;
    }
    else
    {
        // This struct is 200 bytes long, better to make it static for performance reasons
        static VL53L0X_RangingMeasurementData_t rangingData;
        rangingSensor->rangingTest(&rangingData);

        // Serial.print(F("[M] Sensor number "));
        // Serial.print(sensorNumber);
        // Serial.print(F(" range result: "));
        // Serial.print(rangingData.RangeMilliMeter);
        // Serial.println();

        rangeReadings[sensorNumber] = rangingData.RangeMilliMeter;

        // rangingData = {}; // Reset, not needed in the current implementation
    }
}

void Beras_MeasureHeight::setHeightStatus(const int status)
{
    if (status > 0)
    {
        heightStatus = 1;
    }
    else if (status < 0)
    {
        heightStatus = -1;
    }
    else
    {
        heightStatus = 0;
    }
}

void Beras_MeasureHeight::setPreviousBuriedSensors(const unsigned int sensors)
{
    previousBuriedSensors = sensors;
}

void Beras_MeasureHeight::setMeasureInterval(unsigned long interval)
{
    measureInterval = interval;
}