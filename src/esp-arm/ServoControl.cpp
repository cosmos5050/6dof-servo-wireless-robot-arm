#include <ServoControl.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// I2C pins for esp32
#define SDA_PIN 21
#define SCL_PIN 22

// Servo PWM frequency
#define SERVO_FREQ 50

// PCA9685 12-bit resolution
#define SERVO_COUNTS 4096

#define SERVO_PERIOD_US (1000000 / SERVO_FREQ)

// Min and max servo pulse width in us out of 20ms
#define SERVO_PW_MIN_US 500
#define SERVO_PW_MAX_US 2500

// Min and max angles
#define SERVO_ANGLE_MIN 0
#define SERVO_ANGLE_MAX 180

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void initServoControl() {
    Wire.begin(SDA_PIN, SCL_PIN);

    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);
}

void setServoAngle(int channel, int angle) {
    // Map angle range to PW
    int pulse_us = map(
    angle,
    SERVO_ANGLE_MIN,
    SERVO_ANGLE_MAX,
    SERVO_PW_MIN_US,
    SERVO_PW_MAX_US
  );

  // Angle to counts conversion
  int pulse_count = (pulse_us * SERVO_COUNTS) / SERVO_PERIOD_US;

  pwm.setPWM(channel, 0, pulse_count);
}