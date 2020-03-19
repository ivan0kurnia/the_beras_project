#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

#include <BlynkSimpleEsp32.h>

#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_VL53L0X.h>
#include <pcf8574_esp.h>
#include <HX711.h>

#include <Beras_PN532.h>
#include <Beras_UserData.h>
#include <Beras_DeviceState.h>
#include <Beras_StapleDivider.h>
#include <Beras_MeasureHeight.h>
#include <Beras_POSTHandler.h>
#include <Beras_PayloadHandler.h>
#include "definitions.cpp"

const char SERVER_URL[] PROGMEM = "http://mejakalori.xyz/Beras/BerasDevice/";
const char SERVER_KEY[] PROGMEM = "64bf9ec7e43d43c544c059380d171162";

const char BLYNK_KEY[] PROGMEM = "bvBSu-fWm4KHCXxfARaZbQau68nH40cJ";

WiFiClient wifiClient;
HTTPClient httpClient;

PCF857x expander{PCF8574_ADDRESS, &Wire};
Beras_PN532 rfid{Beras_PN532::INTERRUPT_PIN, Beras_PN532::RESET_PIN};

Adafruit_VL53L0X vl53l0x;
VL53L0X_RangingMeasurementData_t measurementData;

HX711 scale;

LiquidCrystal_PCF8574 lcd{LCD_ADDRESS};

Beras_UserData user;
Beras_DeviceState beras;
Beras_StapleDivider stapleDivider;
Beras_MeasureHeight height{&expander, &vl53l0x};

Beras_POSTHandler post{&httpClient, SERVER_URL, SERVER_KEY};
Beras_PayloadHandler berasPayload{'_'};
Beras_PayloadHandler heightPayload{'_'};

TaskHandle_t RFID_TaskHandle;
TaskHandle_t LED_TaskHandle;
TaskHandle_t Blynk_TaskHandle;

namespace Beras_LCD
{
bool lcdIsPrintedBackToDefault = false;
unsigned long lcdLastPrintTimestamp = 0;

void printLcdBackToDefault();
}; // namespace Beras_LCD

namespace Beras_Siren
{
const unsigned int dutySaveAddress = 0;

unsigned short ledDuty;
unsigned short previousLedDuty;

unsigned short savedLedDuty;
}; // namespace Beras_Siren

namespace Beras_Notification
{
const unsigned long criticalInterval = 3600000UL;
const unsigned long lowInterval = 7200000UL;

unsigned long criticalTimestamp = UINT32_MAX - criticalInterval;
unsigned long lowTimestamp = UINT32_MAX - lowInterval;

bool sufficiencyIsNotified = true;
}; // namespace Beras_Notification

void RFID_TaskFunction(void *);
void LED_TaskFunction(void *);
void Blynk_TaskFunction(void *);

void printWifiStatus();
void printDeviceState();
void printDeviceState(bool *);

void setup()
{
    Serial.begin(MONITOR_SPEED);

    for (size_t input = 0; input < sizeof(inputPins) / sizeof(beras_input_t); ++input)
    {
        pinMode(inputPins[input], INPUT);
    }

    for (size_t output = 0; output < sizeof(outputPins) / sizeof(beras_output_t); ++output)
    {
        pinMode(outputPins[output], OUTPUT);
    }

    digitalWrite(ERROR_LED, HIGH); // WiFi has yet to connect

    ledcSetup(SERVO_CHANNEL, SERVO_FREQUENCY, SERVO_RESOLUTION);
    ledcAttachPin(SERVO_MOTOR, SERVO_CHANNEL);
    ledcWrite(SERVO_CHANNEL, SERVO_CLOSED_DUTY);

    Wire.begin(-1, -1, WIRE_CLOCK);
    Wire1.begin(SDA1, SCL1, WIRE1_CLOCK);

    lcd.begin(16, 2);
    lcd.setBacklight(255);

    lcd.setCursor(0, 0);
    lcd.print("SSID: Beras_WiFi");
    lcd.setCursor(0, 1);
    lcd.print("Manager (Open)  ");

    WiFiManager wifiManager;
    wifiManager.autoConnect("Beras_WiFiManager");

    digitalWrite(ERROR_LED, LOW); // WiFi is now connected

    Blynk.config(BLYNK_KEY);
    Blynk.connect();

    lcd.setCursor(0, 0);
    lcd.print("Beras Project   ");
    lcd.setCursor(0, 1);
    lcd.print("  The Beras Team");

    Beras_LCD::lcdLastPrintTimestamp = millis();

    expander.begin();
    expander.write(Beras_PN532::RESET_PIN, LOW);
    expander.write(Beras_PN532::RESET_PIN, HIGH);

    rfid.SAMConfig();

    scale.begin(LOAD_CELL_DOUT, LOAD_CELL_CLCK);
    scale.set_scale(LOAD_CELL_SCALE);

    ledcSetup(MECHANISM_MOTOR_CHANNEL1, MECHANISM_MOTOR_FREQUENCY, MECHANISM_MOTOR_RESOLUTION);
    ledcAttachPin(MECHANISM_MOTOR_IN1, MECHANISM_MOTOR_CHANNEL1);

    ledcSetup(MECHANISM_MOTOR_CHANNEL2, MECHANISM_MOTOR_FREQUENCY, MECHANISM_MOTOR_RESOLUTION);
    ledcAttachPin(MECHANISM_MOTOR_IN2, MECHANISM_MOTOR_CHANNEL2);

    ledcSetup(SIREN_LED_CHANNEL, SIREN_LED_FREQUENCY, SIREN_LED_RESOLUTION);
    ledcAttachPin(SIREN_LED, SIREN_LED_CHANNEL);

    // Try increasing the stack depth if the stack overflows
    xTaskCreatePinnedToCore(RFID_TaskFunction, "RFID_Task", 2048U, NULL, tskIDLE_PRIORITY + 1U, &RFID_TaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Blynk_TaskFunction, "Blynk_Task", 4096U, NULL, tskIDLE_PRIORITY + 1U, &Blynk_TaskHandle, CONFIG_ARDUINO_RUNNING_CORE);

    EEPROM.begin(sizeof(Beras_Siren::ledDuty));
    Beras_Siren::ledDuty = EEPROM.readUShort(Beras_Siren::dutySaveAddress);
    Beras_Siren::savedLedDuty = Beras_Siren::previousLedDuty = Beras_Siren::ledDuty;

    Blynk.virtualWrite(SIREN_LED, Beras_Siren::ledDuty);

    // Initial height check --------------------------------------------------------------------------------------------------

    int httpCode;
    const String payload = post.getStringPayload("get_height.php", &httpCode);

    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
        for (size_t percentage = 0; percentage < sizeof(BERAS_HEIGHT_PERCENTAGES) / sizeof(const char *const); ++percentage)
        {
            if (payload == BERAS_HEIGHT_PERCENTAGES[percentage])
            {
                height.setPreviousBuriedSensors(percentage);
                Blynk.virtualWrite(VL53L0X_ADDRESS, String(BERAS_HEIGHT_PERCENTAGES[percentage]).toInt());
                break;
            }
        }

        if (height.getPreviousBuriedSensors() < 1)
        {
            height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_CRITICAL);
        }
        else if (height.getPreviousBuriedSensors() < 2)
        {
            height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_LOW);
        }
        else
        {
            height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_SUFFICIENT);
        }

        Serial.print("[M] Initial height: ");
        Serial.print(BERAS_HEIGHT_PERCENTAGES[height.getPreviousBuriedSensors()]);
        Serial.print('%');
        Serial.println();
    }
    else
    {
        height.setPreviousBuriedSensors(-1);
        Blynk.virtualWrite(VL53L0X_ADDRESS, 0);

        Serial.print("[M] WiFi status: ");
        Serial.print(WiFi.status());
        Serial.println();

        Serial.print("[M][E] HTTP response code: ");
        Serial.print(httpCode);
        Serial.println();

        Serial.println("[M][E] Initial height check failed due to network or HTTP failure.");
    }

    // Try increasing the stack depth if the stack overflows
    xTaskCreatePinnedToCore(LED_TaskFunction, "LED_Task", 2048U, NULL, tskIDLE_PRIORITY + 1U, &LED_TaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

void loop()
{
    if (rfid.getReadStatus() && !height.getCurrentMeasuringStatus() /*&& height.getCurrentHeightStatus() != Beras_MeasureHeight::HEIGHT_IS_CRITICAL*/)
    {
        if (beras.getCurrentDeviceState() == BERAS_READY)
        {
            printDeviceState();

            Serial.print("[P] Verifying card, UID: ");
            Serial.print(rfid.getLastUid());
            Serial.println();

            post.addRequestData("card_uid", rfid.getLastUid().c_str());

            int httpCode;
            const String payload = post.getStringPayload("confirm_uid.php", &httpCode);

            if (httpCode == t_http_codes::HTTP_CODE_OK)
            {
                if (payload[0] == 'A') // Authorized
                {
                    berasPayload.loadPayload(&payload);

                    Serial.print("[M] Card accepted.");
                    Serial.println();
                    Serial.print("[M] User full name: ");
                    Serial.print(berasPayload[1]);
                    Serial.println();
                    Serial.print("[M] Staple quota: ");
                    Serial.print(berasPayload[2]);
                    Serial.println();

                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print(berasPayload[1]);
                    lcd.setCursor(0, 1);
                    lcd.print("Kuota: ");
                    lcd.print(static_cast<float>(berasPayload[2].toInt()) / 1000.0F, 1);
                    lcd.print(" kg");

                    user.setUserData(berasPayload[1], berasPayload[2].toInt());
                    stapleDivider.loadStapleQuota(user.getStapleQuota(), BERAS_MAXIMUM_WEIGHT);
                    beras.setHowManyTimesToTransfer(stapleDivider.getStapleArraySize());

                    Serial.print("[M] Beras will be transferred for ");
                    Serial.print(stapleDivider.getStapleArraySize());
                    Serial.print(" time(s).");
                    Serial.println();

                    berasPayload.unloadPayload();

                    beras.goToNextState();
                }
                else if (payload[0] == 'U') // Unauthorized
                {
                    berasPayload.loadPayload(&payload);

                    Serial.print("[M] Card not accepted.");
                    Serial.println();
                    Serial.print("[M] Server message: ");
                    Serial.print(berasPayload[1]);
                    Serial.println();

                    if (berasPayload[1] == "STAPLE ALREADY TAKEN THIS WEEK")
                    {
                        lcd.setCursor(0, 0);
                        lcd.print("Beras sudah di- ");
                        lcd.setCursor(0, 1);
                        lcd.print("ambil minggu ini");

                        Beras_LCD::lcdIsPrintedBackToDefault = false;
                        Beras_LCD::lcdLastPrintTimestamp = millis();
                    }
                    else if (berasPayload[1] == "USER DOES NOT EXIST")
                    {
                        lcd.setCursor(0, 0);
                        lcd.print("Kartu ini tidak ");
                        lcd.setCursor(0, 1);
                        lcd.print("  dikenal sistem");

                        Beras_LCD::lcdIsPrintedBackToDefault = false;
                        Beras_LCD::lcdLastPrintTimestamp = millis();
                    }

                    berasPayload.unloadPayload();

                    rfid.resetReadStatus();
                }
                else
                {
                    Serial.println("[M][E] Payload is undefined.");

                    lcd.setCursor(0, 0);
                    lcd.print("Error tidak     ");
                    lcd.setCursor(0, 1);
                    lcd.print("dapat terdeteksi");

                    Beras_LCD::lcdIsPrintedBackToDefault = false;
                    Beras_LCD::lcdLastPrintTimestamp = millis();

                    rfid.resetReadStatus();
                }
            }
            else
            {
                Serial.print("[M] WiFi status: ");
                Serial.print(WiFi.status());
                Serial.println();

                Serial.print("[M][E] HTTP response code: ");
                Serial.print(httpCode);
                Serial.println();

                Serial.println("[M][E] Card verification failed due to network or HTTP failure.");

                lcd.setCursor(0, 0);
                lcd.print("Error terdeteksi");
                lcd.setCursor(0, 1);
                lcd.print("   pada jaringan");

                Beras_LCD::lcdIsPrintedBackToDefault = false;
                Beras_LCD::lcdLastPrintTimestamp = millis();

                rfid.resetReadStatus();
            }
        }
        else if (beras.getCurrentDeviceState() == BERAS_PREPARING)
        {
            printDeviceState();

            Serial.println("[P] Calibrating the load cell scale.");
            scale.tare();

            Serial.println("[P] Opening the staple valve.");
            ledcWrite(SERVO_CHANNEL, SERVO_OPENED_DUTY);

            if (beras.getHowManyMoreTimesToTransfer() < stapleDivider.getStapleArraySize() - 1)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Pengambilan yang");
                lcd.setCursor(0, 1);
                lcd.print("ke-");
                lcd.print(stapleDivider.getStapleArraySize() - beras.getHowManyMoreTimesToTransfer());
                lcd.print(" dari ");
                lcd.print(stapleDivider.getStapleArraySize());
            }

            beras.goToNextState();
        }
        else if (beras.getCurrentDeviceState() == BERAS_MEASURING)
        {
            static bool deviceStatePrinted = false;
            printDeviceState(&deviceStatePrinted);

            static bool stapleQuotaReached = false;
            if (!stapleQuotaReached)
            {
                long units = static_cast<long>(scale.get_units());

                static unsigned long previousTimestamp{0};
                if (millis() - previousTimestamp >= 200UL) // Print staple mass every interval
                {
                    Serial.print("[P] Staple mass: ");
                    Serial.print(units);
                    Serial.print(" grams.");
                    Serial.println();

                    previousTimestamp = millis();
                }

                // Unsigned int casted to long to prevent false logic returned by the comparison operator (>=)
                if (units >= static_cast<long>(stapleDivider[beras.getHowManyMoreTimesToTransfer()]))
                {
                    Serial.println("[P] Staple has reached its quota.");
                    stapleQuotaReached = true;

                    Serial.print("[M] Final staple mass: ");
                    Serial.print(units);
                    Serial.print(" grams.");
                    Serial.println();

                    Serial.println("[P] Closing the staple valve.");
                    ledcWrite(SERVO_CHANNEL, SERVO_CLOSED_DUTY);
                }
            }
            else
            {
                static bool instructionPrinted = false;
                if (!instructionPrinted)
                {
                    Serial.println("[P] Staple ready to transfer.");

                    Serial.print("[P] ");
                    Serial.print(beras.getHowManyMoreTimesToTransfer());
                    Serial.print(" more times to transfer the staple.");
                    Serial.println();

                    lcd.setCursor(0, 0);
                    lcd.print("Sentuh tombol   ");
                    lcd.setCursor(0, 1);
                    lcd.print(" untuk mengambil");

                    Serial.println("[M] Please press the button to proceed.");
                    instructionPrinted = true;
                }

                if (digitalRead(CAPACITIVE_BUTTON))
                {
                    Serial.println("[P] Transferring staple.");

                    lcd.setCursor(0, 0);
                    lcd.print("Menuang beras...");
                    lcd.setCursor(0, 1);
                    lcd.print("                ");

                    deviceStatePrinted = false;
                    stapleQuotaReached = false;
                    instructionPrinted = false;

                    beras.goToNextState();
                }
            }
        }
        else if (beras.getCurrentDeviceState() == BERAS_TRANSFERRING)
        {
            static bool deviceStatePrinted = false;
            printDeviceState(&deviceStatePrinted);

            static bool stapleTransferred = false;
            if (!stapleTransferred)
            {
                static bool motorDirection = false;
                if (!motorDirection) // Forward
                {
                    ledcWrite(MECHANISM_MOTOR_CHANNEL1, MECHANISM_MOTOR_DUTY);
                    ledcWrite(MECHANISM_MOTOR_CHANNEL2, 0);

                    if (digitalRead(MECHANISM_SWITCH_1))
                    {
                        // Brake for 1 ms
                        ledcWrite(MECHANISM_MOTOR_CHANNEL1, MECHANISM_MOTOR_MAX_DUTY);
                        ledcWrite(MECHANISM_MOTOR_CHANNEL2, MECHANISM_MOTOR_MAX_DUTY);
                        delay(1);

                        ledcWrite(MECHANISM_MOTOR_CHANNEL1, 0);
                        ledcWrite(MECHANISM_MOTOR_CHANNEL2, 0);

                        motorDirection = true;

                        Serial.println("[P] Staple transfer is done.");
                        Serial.println("[P] Reversing motor.");

                        lcd.setCursor(0, 0);
                        lcd.print("Penuangan beras ");
                        lcd.setCursor(0, 1);
                        lcd.print("  telah berhasil");
                    }
                }
                else // Backward
                {
                    ledcWrite(MECHANISM_MOTOR_CHANNEL1, 0);
                    ledcWrite(MECHANISM_MOTOR_CHANNEL2, MECHANISM_MOTOR_DUTY);

                    if (digitalRead(MECHANISM_SWITCH_2))
                    {
                        // Brake for 1 ms
                        ledcWrite(MECHANISM_MOTOR_CHANNEL1, MECHANISM_MOTOR_MAX_DUTY);
                        ledcWrite(MECHANISM_MOTOR_CHANNEL2, MECHANISM_MOTOR_MAX_DUTY);
                        delay(1);

                        ledcWrite(MECHANISM_MOTOR_CHANNEL1, 0);
                        ledcWrite(MECHANISM_MOTOR_CHANNEL2, 0);

                        motorDirection = false;
                        stapleTransferred = true;

                        Serial.println("[P] Mechanism is back to its place.");
                    }
                }
            }
            else
            {
                deviceStatePrinted = false;
                stapleTransferred = false;

                if (beras.getHowManyMoreTimesToTransfer())
                {
                    Serial.println("[P] Preparing for the next transfer.");

                    beras.tellThisObjectOneTransferIsDone();
                    beras.setCurrentDeviceState(BERAS_PREPARING);
                }
                else
                {
                    Serial.println("[M] Everything was done beautifully!");

                    Serial.println("\r\n[M] Thank you for using our device.");
                    Serial.println("\t- The Beras Team\r\n");

                    lcd.setCursor(0, 0);
                    lcd.print(" Proses selesai ");
                    lcd.setCursor(0, 1);
                    lcd.print("- Terima Kasih -");

                    Beras_LCD::lcdIsPrintedBackToDefault = false;
                    Beras_LCD::lcdLastPrintTimestamp = millis();

                    user.clearUserData();
                    stapleDivider.clearStapleArray();

                    rfid.resetReadStatus();
                    beras.resetDeviceState();
                }
            }
        }
    }

    // ------------------------------------------------------------------------------------------

    if (!rfid.getReadStatus())
    {
        static unsigned long previousTimestamp = UINT32_MAX - height.getMeasureInterval();
        if (millis() - previousTimestamp >= height.getMeasureInterval())
        {
            if (height.measureHeight())
            {
                unsigned int berasHeight = height.getSumOfBuriedSensors();
                height.resetMeasurement();

                if (berasHeight != height.getPreviousBuriedSensors())
                {
                    Blynk.virtualWrite(VL53L0X_ADDRESS, String(BERAS_HEIGHT_PERCENTAGES[berasHeight]).toInt());

                    if (berasHeight < 1)
                    {
                        height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_CRITICAL);
                        Beras_Notification::criticalTimestamp -= Beras_Notification::criticalInterval;
                    }
                    else if (berasHeight < 2)
                    {
                        height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_LOW);
                        Beras_Notification::lowTimestamp -= Beras_Notification::lowInterval;
                    }
                    else
                    {
                        height.setHeightStatus(Beras_MeasureHeight::HEIGHT_IS_SUFFICIENT);
                        Beras_Notification::sufficiencyIsNotified = false;
                    }

                    post.addRequestData("height_percentage", BERAS_HEIGHT_PERCENTAGES[berasHeight]);

                    int httpCode;
                    const String payload = post.getStringPayload("update_height.php", &httpCode);

                    if (httpCode == t_http_codes::HTTP_CODE_OK)
                    {
                        if (payload[0] == 'U') // Updated
                        {
                            heightPayload.loadPayload(&payload);

                            Serial.print("[M] Height percentage updated to ");
                            Serial.print(heightPayload[1]);
                            Serial.print("%");
                            Serial.println();

                            heightPayload.unloadPayload();

                            height.setPreviousBuriedSensors(berasHeight);
                        }
                        else if (payload[0] == 'N') // Not updated
                        {
                            heightPayload.loadPayload(&payload);

                            Serial.println("[M] Height percentage is not updated.");
                            Serial.print("[M] Server message: ");
                            Serial.print(heightPayload[1]);
                            Serial.println();

                            heightPayload.unloadPayload();
                        }
                        else
                        {
                            Serial.println("[M][E] Payload is undefined.");
                        }
                    }
                    else
                    {
                        Serial.print("[M] WiFi status: ");
                        Serial.print(WiFi.status());
                        Serial.println();

                        Serial.print("[M][E] HTTP response code: ");
                        Serial.print(httpCode);
                        Serial.println();

                        Serial.println("[M][E] Height update failed due to network or HTTP failure.");
                    }
                }
                // else // Comment this entire 'else' for less serial printing
                // {
                //     Serial.println("[M] Height update is not needed as it is the same as it was previously.");
                // }

                previousTimestamp = millis();
            }
        }
    }
    else
    {
        height.resetMeasurement();
    }

    printWifiStatus();
    Beras_LCD::printLcdBackToDefault();
}

void Beras_LCD::printLcdBackToDefault()
{
    if (!lcdIsPrintedBackToDefault && millis() - lcdLastPrintTimestamp >= 2500UL)
    {
        lcd.setCursor(0, 0);
        lcd.print("Tap kartu untuk ");
        lcd.setCursor(0, 1);
        lcd.print(" mengambil beras");

        lcdIsPrintedBackToDefault = true;
    }
}

void printWifiStatus()
{
    static unsigned int previousWifiStatus = WiFi.status();

    unsigned int wifiStatus = WiFi.status();
    if (previousWifiStatus != wifiStatus)
    {
        Serial.print("[P] WiFi status: ");
        Serial.print(wifiStatus);
        Serial.println();

        previousWifiStatus = wifiStatus;
    }
}

void printDeviceState()
{
    Serial.print("[S] Current device state: ");
    Serial.print(beras.getCurrentDeviceState());
    Serial.print(" | ");
    Serial.print(beras.getCurrentDeviceStateName());
    Serial.println();
}

void printDeviceState(bool *staticPrintingState)
{
    if (!staticPrintingState)
    {
        printDeviceState();
        *staticPrintingState = true;
    }
}

void RFID_TaskFunction(void *)
{
    for (;;)
    {
        static unsigned long previousTimestamp = millis();

        if (!rfid.getReadStatus() && millis() - previousTimestamp >= rfid.getReadingInterval())
        {
            rfid.readRFID();

            previousTimestamp = millis();
        }
    }
}

void Blynk_TaskFunction(void *)
{
    for (;;)
    {
        if (Blynk.connected())
        {
            Blynk.run();
        }
        else
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                Blynk.connect();
            }
        }

        static unsigned long previousTimestamp = millis();
        if (Beras_Siren::ledDuty != Beras_Siren::savedLedDuty)
        {
            if (Beras_Siren::ledDuty != Beras_Siren::previousLedDuty)
            {
                Beras_Siren::previousLedDuty = Beras_Siren::ledDuty;
                previousTimestamp = millis();
            }
            else
            {
                if (millis() - previousTimestamp >= 10000UL)
                {
                    EEPROM.writeUShort(Beras_Siren::dutySaveAddress, Beras_Siren::ledDuty);
                    EEPROM.commit();

                    Beras_Siren::savedLedDuty = Beras_Siren::previousLedDuty = Beras_Siren::ledDuty;

                    Serial.print("[M] New siren brightness setting (");
                    Serial.print(Beras_Siren::savedLedDuty);
                    Serial.print(" out of ");
                    Serial.print(SIREN_LED_MAX_DUTY);
                    Serial.print(") is saved to the EEPROM.");
                    Serial.println();

                    previousTimestamp = millis();
                }
            }
        }
        else
        {
            previousTimestamp = millis();
        }

        if (height.getCurrentHeightStatus() == Beras_MeasureHeight::HEIGHT_IS_CRITICAL)
        {
            if (millis() - Beras_Notification::criticalTimestamp >= Beras_Notification::criticalInterval)
            {
                Blynk.notify("Persediaan beras bagi keluarga prasejahtera HAMPIR HABIS. Mohon bantuannya mengisi ATM Beras. Tuhan memberkati. Amin.");
                Beras_Notification::criticalTimestamp = millis();
            }
        }
        else if (height.getCurrentHeightStatus() == Beras_MeasureHeight::HEIGHT_IS_LOW)
        {
            if (millis() - Beras_Notification::lowTimestamp >= Beras_Notification::lowInterval)
            {
                Blynk.notify("Persediaan beras bagi keluarga prasejahtera telah MENIPIS. Mohon bantuannya mengisi ATM Beras. Tuhan memberkati. Amin.");
                Beras_Notification::lowTimestamp = millis();
            }
        }
        else // (height.getCurrentHeightStatus() == Beras_MeasureHeight::HEIGHT_IS_SUFFICIENT)
        {
            if (!Beras_Notification::sufficiencyIsNotified)
            {
                Blynk.notify("Persediaan beras bagi keluarga prasejahtera telah cukup kembali. Tuhan memberkati. Amin.");
                Beras_Notification::sufficiencyIsNotified = true;
            }
        }
    }
}

void LED_TaskFunction(void *)
{
    for (;;)
    {
        // Red LED in the PCB
        if (WiFi.status() == WL_CONNECTED)
        {
            digitalWrite(ERROR_LED, LOW);
        }
        else
        {
            digitalWrite(ERROR_LED, HIGH);
        }

        // Siren LED
        if (height.getCurrentHeightStatus() == Beras_MeasureHeight::HEIGHT_IS_SUFFICIENT)
        {
            ledcWrite(SIREN_LED_CHANNEL, 0);
        }
        else
        {
            static unsigned long previousTimestamp = millis();
            static bool ledState = true;

            if (millis() - previousTimestamp >= 500UL)
            {
                ledState = !ledState;
                previousTimestamp = millis();
            }

            ledcWrite(SIREN_LED_CHANNEL, ledState ? Beras_Siren::ledDuty : 0);
        }
    }
}

BLYNK_WRITE(SIREN_LED)
{
    Beras_Siren::ledDuty = param.asInt();
}