#include "sys_state.h"

#include <Arduino.h>


#define IDLE_LED_DELAY_US       500000
#define DBW_ACTIVE_LED_DELAY_US 250000
#define ESTOP_LED_DELAY_US      125000
#define HWERR_LED_DELAY_US      62500

#define DBW_ACTIVE_TIMEOUT_US 10000


static sys_state_e   state;
static byte          led_state;
static unsigned long led_delay_us;
static unsigned long prv_blink_us;
static unsigned long prv_dbw_active_us;


sys_state_e dbw_active(void)
{
	// this is called a guard clause, check an inverted condition
	// and return early as opposed to hard to read if-else blocks
	if ((state != IDLE) && (state != DBW_ACTIVE)) return state;

	prv_dbw_active_us = micros();
	return state = DBW_ACTIVE;
}

sys_state_e estop(void)
{
	return state = ESTOP;
}

sys_state_e hwerr(void)
{
	if (state == ESTOP) return state;

	return state = HWERR;
}

sys_state_e sys_state(void)
{
	return state;
}

void sys_state_setup(void *ptr)
{
	pinMode(LED_BUILTIN, OUTPUT);

	state        = IDLE;
	led_state    = LOW;
	led_delay_us = IDLE_LED_DELAY_US;

	digitalWrite(LED_BUILTIN, led_state);
}

void sys_state_task(void *ptr)
{
	unsigned long cur;

rerun:
	switch (state) {
		case IDLE:
			led_delay_us = IDLE_LED_DELAY_US;
			break;

		case DBW_ACTIVE:
			// note the clever goto label use here, this
			// allows for us to really cut down on how many
			// times we need to repeat ourselves here
			if ((cur = micros()) - prv_dbw_active_us >= DBW_ACTIVE_TIMEOUT_US) {
				state = IDLE;
				goto rerun;
			}

			led_delay_us = DBW_ACTIVE_LED_DELAY_US;
			break;

		case ESTOP:
			led_delay_us = ESTOP_LED_DELAY_US;
			break;

		case HWERR:
			led_delay_us = HWERR_LED_DELAY_US;
			break;

		default:
			led_delay_us = ESTOP_LED_DELAY_US;
			state = ESTOP;
	}

	if ((cur = micros()) - prv_blink_us >= led_delay_us) {
		prv_blink_us = cur;

		led_state = !led_state;

		digitalWrite(LED_BUILTIN, led_state);
	}
}
