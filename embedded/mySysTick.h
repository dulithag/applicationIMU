#ifndef __MYSYSTICK__
#define __MYSYSTICK__


#define ST_CTRL   *((volatile unsigned long*)  0xE000E010)
#define ST_RELOAD *((volatile unsigned long*)  0xE000E014)
#define ST_CURRENT *((volatile unsigned long*) 0xE000E018)

void SysTick_Handler(void);
void sysTickInit(void);

#endif 
