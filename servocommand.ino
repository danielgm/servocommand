
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

static int v;
static int servoIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {

	// Format: "2:200;4:475;1:250"
	// Servo #2 to 200 PWM, #4 to 475, and servo 1 to 250 PWM.

	while (Serial.available()) {
		char c = Serial.read();
		switch (c) {
			case '0'...'9':
				v = v * 10  +  (c - '0');
				break;

			case ':':
				Serial.print(v);
				Serial.print(':');
				servoIndex = v;
				v = 0;
				break;

			case ';':
				Serial.print(v);
				Serial.print(';');
				pwm.setPWM(servoIndex, 0, v);
				v = 0;
				break;
		}
	}
}

