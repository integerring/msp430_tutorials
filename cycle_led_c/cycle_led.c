/*
  Copyright (c) 2013, Mattias Svanstrom
  All rights reserved.
  See the file LICENSE.TXT for the software license.
*/

#include <msp430.h>
#include <isr_compat.h>

// Connected to port P1
#define RED_LED   BIT0
#define GREEN_LED BIT6
#define BUTTON    BIT3

int main(void) {
  // Stop watchdog before it expires (in 32768 cycles from reset).
  WDTCTL = WDTPW | WDTHOLD;  // Must use "password"

  // Setup LED directions as out, BUTTON as in
  P1DIR |= (GREEN_LED | RED_LED);
  P1DIR &= ~BUTTON;

  // Initialize LED states
  P1OUT &= ~(GREEN_LED | RED_LED);

  // Must enable internal resistor on Launchpad 1.5
  P1REN |= BUTTON;
  P1OUT |= BUTTON;  // Pull up

  // Setup interrupt for BUTTON
  P1IFG &= ~(BUTTON);
  P1IE  |= BUTTON;
  P1IES |= BUTTON;  // React on button press, not release
  __eint();

  // Enter LPM4 between interrupts
  LPM4;

  return 0;  // Never executed
}

ISR(PORT1, button_press) {
  if (P1IFG & BUTTON) {
    P1IFG &= ~(BUTTON);

    P1OUT ^= (P1OUT & GREEN_LED) ? RED_LED : 0;
    P1OUT ^= GREEN_LED;
  }
}
