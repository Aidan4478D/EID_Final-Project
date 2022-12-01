#ifndef SENSOR_PRIVATE_H
#define SENSOR_PRIVATE_H


// notice again how private headers are only intended to be included in
// the associated source file and nowhere else, beware as not doing so
// may result in unforseen consequences
static void enc_chan_a_isr(void);
static void enc_chan_b_isr(void);
static void enc_reset_isr(void);

static unsigned long ultrasonic_pulse(void);


#endif /* SENSOR_PRIVATE_H */
