// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	
	
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
//  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
//  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R &= ~0x14;        // 3) disable analog on PF2,4
  GPIO_PORTF_PCTL_R &= ~0x000F0F00;   // 4) PCTL GPIO on PF2,4
  GPIO_PORTF_DIR_R &= ~0x10;          // 5) PF4 in,
	GPIO_PORTF_DIR_R |= 0x04;           // PF2 out
  GPIO_PORTF_AFSEL_R &= ~0x14;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R |= 0x10;          // enable pull-up on  PF4
  GPIO_PORTF_DEN_R = 0x14;          // 7) enable digital I/O on PF2,4
  GPIO_PORTF_DATA_R |= 0x04;         // set pf2 on

  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes 
		Delay100ms(1);
		if(GPIO_PORTF_DATA_R&0x10) GPIO_PORTF_DATA_R |= 0x04;
		if(!(GPIO_PORTF_DATA_R&0x10)) GPIO_PORTF_DATA_R ^= 0x04;
		
  }
}
