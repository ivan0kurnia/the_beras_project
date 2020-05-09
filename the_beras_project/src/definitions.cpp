#include <Arduino.h>

#define USE_BLYNK 1

typedef const unsigned int beras_input_t;
typedef const unsigned int beras_output_t;

// Input Pins
beras_input_t COVER_SWITCH = 36U;
beras_input_t PUSH_BUTTON = 39U;
beras_input_t MECHANISM_SWITCH_1 = 34U;
beras_input_t MECHANISM_SWITCH_2 = 35U;
beras_input_t CAPACITIVE_BUTTON = 2U;
beras_input_t RFID_INTERRUPT = 4U;

// Array of Input Pins
beras_input_t inputPins[]{
    COVER_SWITCH,
    PUSH_BUTTON,
    MECHANISM_SWITCH_1,
    MECHANISM_SWITCH_2,
    CAPACITIVE_BUTTON,
    RFID_INTERRUPT};

// Output Pins
beras_output_t VIBRATION_MOTOR = 25U;
beras_output_t MECHANISM_MOTOR_IN1 = 26U;
beras_output_t MECHANISM_MOTOR_IN2 = 27U;
beras_output_t SERVO_MOTOR = 13U;
beras_output_t ERROR_LED = 15U;

#if USE_BLYNK
#define SIREN_LED 23
#else
beras_output_t SIREN_LED = 23U;
#endif

// Array of Output Pins
beras_output_t outputPins[]{
    VIBRATION_MOTOR,
    MECHANISM_MOTOR_IN1,
    MECHANISM_MOTOR_IN2,
    SERVO_MOTOR,
    ERROR_LED,
    SIREN_LED};

// Load cell definitions
const unsigned int LOAD_CELL_DOUT = 19U;
const unsigned int LOAD_CELL_CLCK = 18U;
const float LOAD_CELL_SCALE = 368.34F;

// Staple constrains in grams
const unsigned int BERAS_MAXIMUM_WEIGHT = 4000U;
// const unsigned int BERAS_EXTRA_FALLING_STAPLE = 0U; // Offset to small to care about this

// Servo motor definitions
const unsigned int SERVO_CHANNEL = 0U;
const unsigned int SERVO_FREQUENCY = 50U;
const unsigned int SERVO_RESOLUTION = 14U;
const unsigned int SERVO_CLOSED_DUTY = 544U;
const unsigned int SERVO_OPENED_DUTY = 1415U;

// Mechanism motor definitions
const unsigned int MECHANISM_MOTOR_CHANNEL1 = 2U;
const unsigned int MECHANISM_MOTOR_CHANNEL2 = 4U;
const unsigned int MECHANISM_MOTOR_FREQUENCY = 8192U;
const unsigned int MECHANISM_MOTOR_RESOLUTION = 12U;
const unsigned int MECHANISM_MOTOR_MAX_DUTY = pow(2, MECHANISM_MOTOR_RESOLUTION) - 1;
const unsigned int MECHANISM_MOTOR_DUTY = MECHANISM_MOTOR_MAX_DUTY;

// Siren LED definitions
const unsigned int SIREN_LED_CHANNEL = 6U;
const unsigned int SIREN_LED_FREQUENCY = 1000U;
const unsigned int SIREN_LED_RESOLUTION = 12U;
const unsigned int SIREN_LED_MAX_DUTY = pow(2, SIREN_LED_RESOLUTION) - 1;

// Wire1 pin definitions
const unsigned int SDA1 = 16U;
const unsigned int SCL1 = 17U;

// Speeds and clocks
const unsigned int MONITOR_SPEED = 115200U;
const unsigned int WIRE_CLOCK = 400000U;
const unsigned int WIRE1_CLOCK = 400000U;

// I2C addresses
const unsigned int PCF8574_ADDRESS = 0x20;
const unsigned int LCD_ADDRESS = 0x27;

#if USE_BLYNK
#define VL53L0X_ADDRESS 0x29
#else
const unsigned int VL53L0X_ADDRESS = 0x29;
#endif

// Beras height percentages defined in an array of C-string
const char *const BERAS_HEIGHT_PERCENTAGES[]{"0", "15", "20", "40", "60", "80"};

// Beras measure height interval
const unsigned long HEIGHT_INTERVAL = 2000UL;