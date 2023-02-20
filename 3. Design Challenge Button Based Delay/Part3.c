/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Edward R. Coyle
 *
 *      This code is used to blink an led at a designated speed determined by the length of time a button is pressed.
 *
 */



#include <msp430.h>
#include <stdint.h>


void gpioInit();
void timerInit();
uint16_t speed= 1000;       //Initial amount of time the button will blink
uint16_t timepress= 1000;   //Initial value of timepress
uint16_t count= 1000;       //Initial value of count
char state = 0;
void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();                             // Runs gpioInit
    timerInit();                            // Runs timerInit

    PM5CTL0 &= ~LOCKLPM5;
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings

    __bis_SR_register(GIE);

while(1){
   if (timepress == 00)            // Used for startup. this is so the output will never be 0
{   timepress= 1000;            // sets the timepress to 1000 if it is ever 0

}

    switch (state) {

      case 0:
            {
                break;      // this lets the system work without changing anything
            }
      case 1:
        {
            count++;        //used to count how long one wants the button to be pressed
            break;
            }
      case 2:
        {
            timepress=0;            //this resets timepress to hold a new value
            timepress=count;        // this sets the number held in count to timepress
            state=0;                // sets the state back to 0
            count=0;                // resets count

            break;
            }
    }
}
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings


}


void gpioInit(){

    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P6.6 to output direction

    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low --> High edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled

    P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
    P4REN |= BIT1;                          // P2.3 pull-up register enable
    P4IES &= ~BIT1;                         // P2.3 Low --> High edge
    P4IE |= BIT1;                           // P2.3 interrupt enabled


}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

    // Timer1_B3 setup
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = speed;                          // set initial timer to 50,000
    TB1CTL = TBSSEL_1 | MC_2|ID_2;                 // ACLK, continuous mode, divider value of 4



}


/*
 * INTERRUPT ROUTINES
 */

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    if ( P2IN &  BIT3)       // @TODO Fill in this argument within the If statement to check if the interrupt was triggered off a rising edge.
    {
        state = 2;                          // sets led to 1
        P2IES |= BIT3;                         // P2.3 High --> Low edge
        // @TODO Add code to change which edge the interrupt should be looking for next
    }

    else if (!( P2IN & BIT3 )) // @TODO Fill in this argument within the If statement to check if the interrupt was triggered off a falling edge.
    {
        state=1;                          // sets led to 0
        P2IES &= ~BIT3;                         // P2.3 Low --> High edge
        // @TODO Add code to change which edge the interrupt should be looking for next
    }
    P2IFG &= ~BIT3;                         // Clear P1.3 IFG
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    timepress = 1000;                        // sets timepress to 5000
    count=0;                                 // resets count
    P4IFG &= ~BIT1;                          // Clear P4.1 IFG
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

    P1OUT ^= BIT0;                                // blinks the red led when the timer is up
    TB1CCR0 += timepress;                         // Add Offset to TB1CCR0
}










