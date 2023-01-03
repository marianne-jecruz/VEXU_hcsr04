/**
 * @file hcsr04.h
 * @author Marianne Cruz 
 * @brief
 *    HCSR04 driver functions
 * 		Output Pins:
 * 			PB3 - Trigger signal (1ms)
 *			PA1 - UART0 Tx 
 *		Input Pin:
 *			PB6 - Echo Signal
 * @version 0.1
 * @date 2022-12-23
 */
 
#include <stdint.h>
#include "./inc/Timer1A.h"
#include "./inc/tm4c123gh6pm.h"
 
 void hcsr04_Init(void);
 
 int Timer0ACapture_EchoWidth(void);
 
 void send_Trigger(void);
 
void static DelayMs(void);
 
 
 
 