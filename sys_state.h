#ifndef SYS_STATE_H
#define SYS_STATE_H


// here's a user-defined type that we can export to other files by
// including this header
enum sys_state_e {
	IDLE,
	DBW_ACTIVE,
	ESTOP,
	HWERR,
};


sys_state_e dbw_active(void);
sys_state_e estop(void);
sys_state_e hwerr(void);
sys_state_e sys_state(void);

void sys_state_setup(void *ptr);
void sys_state_task(void *ptr);


#endif /* SYS_STATE_H */
