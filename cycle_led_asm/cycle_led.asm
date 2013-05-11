; Copyright (c) 2013, Mattias Svanstrom
; All rights reserved.
; See the file LICENSE.TXT for the software license. 

#include <msp430g2553.h>

        .equ RED_LED,   BIT0
        .equ GREEN_LED, BIT6
        .equ BUTTON,    BIT3

        ;;
        ;; Main program
        ;;
        .text

main:
        mov     #(WDTPW | WDTHOLD), &WDTCTL ; Disable watchdog
        mov     #__stack, r1                ; Set up stack
        
        ;; Setup LED directions as out, BUTTON as in
        bis.b   #(GREEN_LED | RED_LED), &P1DIR
        bic.b   #BUTTON, &P1DIR
        ;; Initialize LED states
        bic.b   #(GREEN_LED | RED_LED), &P1OUT
        ;; Must enable internal pull-up resistor for button on Launchpad 1.5
        bis.b   #BUTTON, &P1REN
        bis.b   #BUTTON, &P1OUT
        ;; Enable interrupt
        bic.b   #BUTTON, &P1IFG
        bis.b   #BUTTON, &P1IE
        bis.b   #BUTTON, &P1IES
        eint
hang:
        bis     #LPM4, r2       ; Enter LPM4, execution stops

        ;;
        ;; ISR for button press
        ;;
isr_button_press:
        bit.b   #BUTTON, &P1IFG ; Ignore interrupt from other than the button
        jz      isr_end
        bic.b   #BUTTON, &P1IFG ; Clear interrupt flag
        bit.b   #GREEN_LED, &P1OUT ; Is green LED on?
        jz      flip_green
        xor.b   #RED_LED, &P1OUT
flip_green:
        xor.b   #GREEN_LED, &P1OUT
isr_end:
        reti

        ;;
        ;; Interrupt vector table
        ;;
        .section .vectors, "a"

        .word   isr_end                 ; 0xFFE0 Unused
        .word   isr_end                 ; 0xFFE2 Unused
        .word   isr_button_press        ; 0xFFE4 Port 1
        .word   isr_end                 ; 0xFFE6 Port 2
        .word   isr_end                 ; 0xFFE8 Unused
        .word   isr_end                 ; 0xFFEA ADC10
        .word   isr_end                 ; 0xFFEC USCI A0/B0 Transmit
        .word   isr_end                 ; 0xFFEE USCI A0/B0 Receive
        .word   isr_end                 ; 0xFFF0 Timer0_A CC1, TA0
        .word   isr_end                 ; 0xFFF2 Timer0_A CC0
        .word   isr_end                 ; 0xFFF4 Watchdog Timer
        .word   isr_end                 ; 0xFFF6 Comparator A
        .word   isr_end                 ; 0xFFF8 Timer1_A CC1-4, TA1
        .word   isr_end                 ; 0xFFFA Timer1_A CC0
        .word   isr_end                 ; 0xFFFC Non-maskable
        .word   main                    ; 0xFFFE Reset
