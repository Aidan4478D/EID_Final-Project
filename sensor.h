#ifndef SENSOR_H
#define SENSOR_H


long          encoder_count(void);
unsigned long ultrasonic_pulse_us(void);

void sensor_setup(void *ptr);
void sensor_task(void *ptr);


#endif /* SENSOR_H */
