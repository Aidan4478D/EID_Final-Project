#include "motor-ctrl.h"

#include <Arduino.h>

// a good convention for include order is as follows:
//   * associated source header
//   * system headers
//   * source tree headers
#include "sensor.h"
#include "sys_state.h"


#define MOTOR 2


static void motor_ctrl(byte pwm)
{
	// although this is a dummy wrapper function, you can imagine
	// that if we were controlling a motor using an h-bridge we
	// could encapsulate the logic here
	analogWrite(MOTOR, pwm);
}


void motor_ctrl_setup(void *ptr)
{
	pinMode(MOTOR, OUTPUT);

	digitalWrite(MOTOR, LOW);
}

void motor_ctrl_task(void *ptr)
{
	// we want to turn off motors the moment we're not in DBW_ACTIVE
	if (sys_state() != DBW_ACTIVE) {
		motor_ctrl(0);
		return;
	}

	motor_ctrl(128);
}
