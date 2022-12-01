// we need to include any headers for functions we'll use
#include "motor-ctrl.h"
#include "sensor.h"
#include "sys_state.h"


// these are all the setup tasks we need to run to initialize anything
// necessary for the appropriate task functions
static void (*setups[])(void *ptr) = {
	motor_ctrl_setup,
	sensor_setup,
	sys_state_setup,
};

// these are all the task functions that we want to execute, make sure
// to reduce as many blocking operations so that they get called as
// often as possible
static void (*tasks[])(void *ptr) = {
	motor_ctrl_task,
	sensor_task,
	sys_state_task,
};


void setup(void)
{
	// loop through all the setup tasks
	for (size_t i = 0; i < (sizeof(setups) / sizeof(setups[0])); i++)
		setups[i](NULL);
}

void loop(void)
{
	// loop through all the tasks
	for (size_t i = 0; i < (sizeof(tasks) / sizeof(tasks[0])); i++)
		tasks[i](NULL);
}
