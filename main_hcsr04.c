/**
 * @file main1.c
 * @author Marianne Cruz 
 * @brief
 *    The main file that drives the HCSR04 ultrasonic sensor.
 *		Main loop continously sends trigger signal, measures width of echo pulse,
 * 		processes it, and sends corresponding length through UART0
 * @version 0.1
 * @date 2022-01-3 
 */

/* Copyright 2022 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

/** File includes. */
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* Register definitions. */
#include "./inc/tm4c123gh6pm.h"
/* Clocking. */
#include "./inc/PLL.h"
/* Clock delay and interrupt control. */
#include "./inc/CortexM.h"
#include "./inc/UART.h"
/* Add whatever else you need here! */
#include "hcsr04.h"
#include "./inc/Timer1A.h"
#include "./inc/TExaS.h"



/** Entry point. */
int main(void) {
    DisableInterrupts();

	  /* Interrupts currently being used:
	     Timer0A, input capture - Measures pulse width of echo signal 
			 UART0, pri3 - Serial output to Jetson (needs testing) 
	  */
	
    /* PLL Init. */
		PLL_Init(Bus80MHz);

		/* Allow us to talk to the PC via PuTTy! Check device manager to see which
	     COM serial port we are on. The baud rate is 115200 chars/s. */
		UART_Init(3);
		
		hcsr04_Init();
    
		/* Allows any enabled timers to begin running. */
    EnableInterrupts();

		/* Global variables */
		int32_t time_period;
		float distance_in;
		//float distance_cm;

    while (1) {
				send_Trigger();
				time_period = Timer0ACapture_EchoWidth();
			
				/*Accounts for range: 1in - 13ft ( > 13ft = > 15000 period )*/
				if (time_period > 15000){distance_in = 12 * 13;}
				
				else{
				distance_in = (time_period * 1097) / 1000000; /*conversion constant ~ 0.00109669*/
				//distance_cm = distance_in * 2.54;
				UART_OutUDec(distance_in);
				}
    }
		
		return 1;
}


