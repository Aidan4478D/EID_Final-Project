#include "sensor.h"

#include <Arduino.h>

#include "sys_state.h"


#define ENC_CHAN_A      18
#define ENC_CHAN_B      19
#define ENC_RESET       20
#define ULTRASONIC_TRIG 4
#define ULTRASONIC_ECHO 5

#define ULTRASONIC_PULSE_DELAY_US 50000
#define ULTRASONIC_PULSE_US       10
#define ULTRASONIC_TIMEOUT_US     5000


// notice how anything that doesn't need to be exported to the global
// symbol table is static
static volatile long encoder_cnt;
static unsigned long prv_ultrasonic_pulse_us;
static unsigned long ultrasonic_us;


static void enc_chan_a_isr(void)
{
	byte chan_a = digitalRead(ENC_CHAN_A);
	byte chan_b = digitalRead(ENC_CHAN_B);

	if (chan_a)
		if (chan_b)
			--encoder_cnt;
		else
			++encoder_cnt;
	else
		if (chan_b)
			++encoder_cnt;
		else
			--encoder_cnt;
}

static void enc_chan_b_isr(void)
{
	byte chan_a = digitalRead(ENC_CHAN_A);
	byte chan_b = digitalRead(ENC_CHAN_B);

	if (chan_a)
		if (chan_b)
			++encoder_cnt;
		else
			--encoder_cnt;
	else
		if (chan_b)
			--encoder_cnt;
		else
			++encoder_cnt;
}

static void enc_reset_isr(void)
{
	encoder_cnt = 0;
}

static unsigned long ultrasonic_pulse(void)
{
	digitalWrite(ULTRASONIC_TRIG, HIGH);
	delayMicroseconds(ULTRASONIC_PULSE_US);
	digitalWrite(ULTRASONIC_TRIG, LOW);

	return pulseInLong(ULTRASONIC_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);
}


long encoder_count(void)
{
	return encoder_cnt;
}

unsigned long ultrasonic_pulse_us(void)
{
	return ultrasonic_us;
}

void sensor_setup(void *ptr)
{
	pinMode(ENC_CHAN_A, INPUT);
	pinMode(ENC_CHAN_B, INPUT);
	pinMode(ENC_RESET,  INPUT);

	pinMode(ULTRASONIC_TRIG, INPUT);
	pinMode(ULTRASONIC_ECHO, OUTPUT);

	digitalWrite(ULTRASONIC_ECHO, LOW);

	attachInterrupt(digitalPinToInterrupt(ENC_CHAN_A), enc_chan_a_isr, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC_CHAN_B), enc_chan_b_isr, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC_RESET),  enc_reset_isr,  HIGH);
}

void sensor_task(void *ptr)
{
	// if we're in the ESTOP state we might not want to update any
	// sensor values for either safety and/or debugging purposes
	if (sys_state() == ESTOP) return;

	unsigned long cur;

	if ((cur = micros()) - prv_ultrasonic_pulse_us >= ULTRASONIC_PULSE_DELAY_US) {
		prv_ultrasonic_pulse_us = cur;
		ultrasonic_us           = ultrasonic_pulse();
	}
}
