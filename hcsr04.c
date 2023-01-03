/**
 * @file hcsr04.c
 * @author Marianne Cruz 
 * @note 
 * 		Output Pins:
 * 			PB3 - Trigger signal (1ms)
 *			PA1 - UART0 Tx 
 *		Input Pin:
 *			PB6 - Echo Signal 
 * @version 0.1
 * @date 2022-12-23
 */

#include "hcsr04.h"

void static DelayMs(void);

void send_Trigger(void){
	int8_t echo_high = 0;
	
	GPIO_PORTB_DATA_R |= (1<<3);
	DelayMs();
	GPIO_PORTB_DATA_R &= ~(1<<3);
	
	if(GPIO_PORTB_DATA_R & (1<<6)){echo_high = 1;}
	
}

void hcsr04_Init(void){
	//Initialize PB3 as digital out to send trigger
	SYSCTL_RCGCPWM_R      |= 0x01;            // activate PWM0
  SYSCTL_RCGCGPIO_R     |= 0x02;            // activate port B
  while((SYSCTL_PRGPIO_R & 0x02) == 0){};   // Wait
	GPIO_PORTB_PCTL_R     &= ~0x0000F000;     // GPIO Config for pin 3
  GPIO_PORTB_DIR_R      |=  0x08;           // make PB3 out
  GPIO_PORTB_AFSEL_R    &= ~0x08;           // regular port function
  GPIO_PORTB_DEN_R      |=  0x08;           // enable digital I/O on PB3
  GPIO_PORTB_AMSEL_R    &= ~0x08;           // disable analog functionality on PB3-0
	GPIO_PORTB_ODR_R |= (1<<3);         			// (5V L)PB3 Open Drain Enable
	GPIO_PORTB_DR4R_R |= (1<<3); 							// (5V L) PB3 4mA drive enable, since 5/2E3 = 2.5 mA > 2 mA default drive
	GPIO_PORTB_DATA_R &= ~(1<<3);							// Set PB3 low
		
	//Initialize PB6 as digital in to measure pulse width in time
	GPIO_PORTB_DIR_R &= ~(1<<6);        //make PB6 in
  GPIO_PORTB_DEN_R |= (1<<6);         //enable digital I/O on PB6 
  GPIO_PORTB_AFSEL_R |= (1<<6);       //use PB6 alternate function 
  GPIO_PORTB_PCTL_R &= ~0x0F000000;  //configure PB6 for T0CCP0 
  GPIO_PORTB_PCTL_R |= 0x07000000;
  
	//Initialize Timer0A for input-edge time mode
	SYSCTL_RCGCTIMER_R |= 1;  //enable clock to Timer Block 0
  TIMER0_CTL_R &= ~(1<<0);          //disable timer0A during setup 
  TIMER0_CFG_R = 4;            //Timer0A in 16-bit timer mode 
  TIMER0_TAMR_R = 0x17;        //Timer0A enable up-count and capture mode 
  TIMER0_CTL_R |= (1<<3)|(1<<2);   //capture rising and falling edges on PB6 pin 
  TIMER0_CTL_R |= (1<<0);           //enable Timer0A 

}

int Timer0ACapture_EchoWidth(void)
{
    int risingEdge, fallingEdge;
	while(1)
	{
    TIMER0_ICR_R = 4;            /* clear timer0A capture flag */
    while((TIMER0_RIS_R & (1<<2)) == 0) ;    /* wait till captured */
	  if(GPIO_PORTB_DATA_R & (1<<6)) /*check if rising edge occurs */
		{
    risingEdge = TIMER0_TAR_R;     /* save the timestamp */
    TIMER0_ICR_R = 4;            /* clear timer0A capture flag */
    /* detect falling edge */
    while((TIMER0_RIS_R & (1<<2)) == 0) ;    /* wait till captured */
    fallingEdge = TIMER0_TAR_R;     /* save the timestamp */
    return (fallingEdge - risingEdge) & 0x00FFFFFF; /* return the time difference */
		}
	}
}

void static DelayMs(void){
  uint32_t n=1;
	uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}




