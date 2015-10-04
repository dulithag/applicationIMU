#ifndef __MYGPIO__
#define __MYGPIO__


#define PORT_CLK *((volatile unsigned long*) 0x400FE608)
#define RCGC_UART *((volatile unsigned long*) 0x400FE618)
	

#define PORTA_DATA *((volatile unsigned long*) 0x400043FC)
#define PORTA_DEN *((volatile unsigned long*) 0x4000451C)
#define PORTA_DIR *((volatile unsigned long*) 0x40004400)
#define PORTA_AFSEL *((volatile unsigned long*) 0x40004420)	
#define PORTA_PCTL *((volatile unsigned long*) 0x4000452C)	
#define PORTA_AMSEL      (*((volatile unsigned long *)0x40004528))

#define PORTB_DATA *((volatile unsigned long*) 	0x400053FC)
#define PORTB_DEN *((volatile unsigned long*)  	0x4000551C)
#define PORTB_DIR *((volatile unsigned long*)  	0x40005400)
#define PORTB_AFSEL *((volatile unsigned long*) 0x40005420)	
#define PORTB_PUR *((volatile unsigned long*) 	0x40005510)	
#define PORTB_ODR *((volatile unsigned long*) 	0x4000550C)	

#define PORTC_DATA *((volatile unsigned long*) 0x400063FC)
#define PORTC_DEN *((volatile unsigned long*) 0x4000651C)
#define PORTC_DIR *((volatile unsigned long*) 0x40006400)
#define PORTC_AFSEL *((volatile unsigned long*) 0x40006420)	
#define PORTC_PCTL *((volatile unsigned long*) 0x4000652C)	
#define PORTC_AMSEL      (*((volatile unsigned long *)0x40006528))


#define PORTE_DATA *((volatile unsigned long*) 0x400243FC)
#define PORTE_DEN *((volatile unsigned long*)  0x4002451C)
#define PORTE_DIR *((volatile unsigned long*)  0x40024400)
#define PORTE_AFSEL *((volatile unsigned long*) 0x40024420)	
	
#define PORTF_DATA *((volatile unsigned long*) 0x400253FC)
#define PORTF_DEN *((volatile unsigned long*)  0x4002551C)
#define PORTF_DIR *((volatile unsigned long*)  0x40025400)
#define PORTF_AFSEL *((volatile unsigned long*) 0x40025420)	
#define PORTF_PUR *((volatile unsigned long*) 0x40025510)	
#define PORTF_IS *((volatile unsigned long*) 0x40025404)		
#define PORTF_EV *((volatile unsigned long*) 0x4002540C)		
#define PORTF_BE *((volatile unsigned long*) 0x40025408)		
#define PORTF_IM *((volatile unsigned long*) 0x40025410)		
#define PORTF_CR *((volatile unsigned long*) 0x4002541C)		



#define INT_EN *((volatile unsigned long*) 0xE000E100)	
#define INT_PRI *((volatile unsigned long*) 0xE000E41C)	
#define INT_ACT *((volatile unsigned long*) 0xE000E300)		
#define CFGCTRL *((volatile unsigned long*) 0xE000ED14)	

void GPIO_init(void);
void GPIOPortF_Handler(void);

#endif
