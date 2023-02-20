/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>
#include <stdint.h>


void gpioInit();
void timerInit();
uint16_t speed= 10000;
char state = 0;
void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE);

while(1){

    }

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings


}


void gpioInit(){

    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled



}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

    // Timer1_B3 setup
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = speed;                          // set initial timer to 50,000
    TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode



}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.

    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.
    switch (state) {

    case 0:  // Fast
          {
              speed=5000;
              break;
          }
    case 1: // Mid
      {
          speed=10000;
          break;
          }


    case 2: //slow
    {
            speed=20000;
            break;
    }



      case 3:
         {
             state = 0;                   // Resets to case 0
             break;

         }

    }

    {
        if (state >= 3)
            state = 0;
        else
            state ++;                               // Used to increase the alert state by one level
        //TB1CCR0 = speed;                         // Add Offset to TB1CCR0

       P2IFG &= ~BIT3;                          // Clear P4.1 IFG

    }

}


// Timer B1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

    P1OUT ^= BIT0;
    TB1CCR0 += speed;                         // Add Offset to TB1CCR0
}



