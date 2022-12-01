#ifndef MOTOR_CTRL_PRIVATE_H
#define MOTOR_CTRL_PRIVATE_H


// notice how we have to include Arduino.h if we use a type defined by
// the header in our own header
#include <Arduino.h>


static void motor_ctrl(byte pwm);


#endif /* MOTOR_CTRL_PRIVATE_H */
