//#include "inc/tm4c123gh6pm.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "myGPIO.h"
#include "myI2C.h"
#include "mySysTick.h"


/*------------------------------------------------------------
			***** 2. Global Declarations Section *****
--------------------------------------------------------------*/
int sCounter = 0;
int sLightOn = 0;


// FUNCTION PROTOTYPES: Each subroutine defined
//void DisableInterrupts(void); // Disable interrupts
//void EnableInterrupts(void);  // Enable interrupts


/*-----------------------------------------------------------
							Main
-------------------------------------------------------------*/


int main(void){

	volatile unsigned long pa5 = 0;
	volatile unsigned long fr = 0;
	volatile unsigned long fr_1 = 0;
	volatile unsigned long sysctrl = 0;
	volatile unsigned long sysctrl_bit = 0;
	volatile unsigned long sw1 = 0;
	unsigned int val = 0;


	//TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
	GPIO_init();
	I2C_init();
	sysTickInit();


	//EnableInterrupts();           // enable interrupts for the grader
	PORTE_DATA |= 0x00000010;
	//PORTF_DATA |= 0x00000002;
	while(1){

		//Switch On = Light On
		pa5 = (PORTA_DATA & 0x00000020)>>5;
		if(pa5==1){
			PORTE_DATA |= 0x00000010;
			fr = UART4_FR;
			fr_1 = (fr & 0x8)>>3;
			if(fr!=1){
				if(val>127) val = 0;
				UART4_DATA |= 0x41;
			}
		}
		else{
			PORTE_DATA &= ~0x00000010;
		}
/*
		//SysTick Test
		sysctrl = ST_CTRL;
		sysctrl_bit = (sysctrl & 0x00010000)>>16;
		if(sysctrl_bit == 1){
			sCounter++;
			if(sCounter>10){
				if(LightOn==1){
						LightOn = 0;
						PORTF_DATA |= 0x00000002;
				}else{
						LightOn = 1;
						PORTF_DATA &= ~0x00000002;
				}

				sCounter = 0;
			}
		}
*/

		//SW1 Test
		/*
		sw1 = PORTF_DATA;
		if(((sw1&0x00000010)>4)==0){
				PORTF_DATA |= 0x00000002;
		}else{
			PORTF_DATA &= ~0x00000002;
		}
		*/
		val = INT_ACT;
  }

}


























/*
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
int main(void)
{
	uint8_t ui8LED = 2;
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	while(1)
	{
		// Turn on the LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		// Delay for a bit
		SysCtlDelay(2000000);
		// Cycle through Red, Green and Blue LEDs
		if (ui8LED == 8) {ui8LED = 2;} else {ui8LED = ui8LED*2;}
	}
}
*/
