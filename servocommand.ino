
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int MODE_PIN = 0;
const int MODE_SERVO = 1;

static int v;
static int outIndex;
static int readMode;

void setup() {
  Serial.begin(38400);
  Serial.println("Receiving servo commands!");

  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates

  outIndex = 0;
  readMode = MODE_SERVO;

  for (int i = 0; i < 13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {

  // Format: "2:200;4:475;1:250"
  // Servo #2 to 200 PWM, #4 to 475, and servo 1 to 250 PWM.

  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'p':
        Serial.print('p');
        readMode = MODE_PIN;
        break;

      case '0'...'9':
        v = v * 10  +  (c - '0');
        break;

      case ':':
        Serial.print(v);
        Serial.print(':');
        outIndex = v;
        v = 0;
        break;

      case ';':
        Serial.print(v);
        Serial.print(';');
        switch (readMode) {
          case MODE_PIN:
            digitalWrite(outIndex, v > 0 ? HIGH : LOW);
            break;
          case MODE_SERVO:
            pwm.setPWM(outIndex, 0, v);
            break;
        }

        v = 0;
        readMode = MODE_SERVO;
        break;
    }
  }
}

