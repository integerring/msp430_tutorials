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

int main(void) {
  // Watchdog is turned off by compiling with -mdisable-watchdog

  // Initialize LED states
  P1OUT &= ~(GREEN_LED | RED_LED);

  // Setup LED directions as out
  P1DIR |= (GREEN_LED | RED_LED);

  // Use calibration data to set DCO frequency to 8 MHz
  BCSCTL1 = CALBC1_8MHZ;
  DCOCTL = CALDCO_8MHZ;

  // Set MCLK to use DCO at 8 MHz, set SMCLK to use DCO / 8 at 1 MHz
  BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3;

  // Set ACLK to use LFXT1CLK at low freq, 32768 kHz, 12.5 pF
  BCSCTL1 |= DIVA_0;
  BCSCTL3 |= XCAP_3;

  // Set WDT+ as a timer clocked by 1MHz SMCLK, expires every 32 ms.
  // WDT+ expiry should cause an interrupt.
  WDTCTL = WDT_MDLY_32;
  IE1 |= WDTIE;

  __eint();
  
  // Enter LPM0 between interrupts
  LPM0;

  return 0;  // Never executed
}

// Supported crystal states
enum state_t {XTAL_INIT_FAIL, XTAL_RUNNING, XTAL_FAIL};

ISR(WDT, wdt_handler) {
  // WDT+ interrupt flag is auto-cleared on ISR exit.

  // Due to the fact that the LFXT takes quite a long time to start,
  // we must allow for it to be in fail mode when we start the
  // program.

  // Red LED on:   LFXT1 has not yet been up
  // Green LED on: LFXT1 is up, no fails seen apart from initially
  // Both LEDs on: LFX1 has failed after it has been up

  static enum state_t state = XTAL_INIT_FAIL;
  static unsigned int fail;

  // Sample fail flag
  fail = ((BCSCTL3 & LFXT1OF) == LFXT1OF);
  
  // Change state
  if (state == XTAL_INIT_FAIL) {
    if (fail) {
      P1OUT |= RED_LED;
    }
    else {
      state = XTAL_RUNNING;
      P1OUT &= ~RED_LED;
      P1OUT |= GREEN_LED;
    }
  }
  else if ((state == XTAL_RUNNING) && fail) {
    state = XTAL_FAIL;
    P1OUT |= RED_LED | GREEN_LED;
  }
}
