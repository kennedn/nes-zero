#define F_CPU 16000000UL
#define TIMER_TARGET 92 // 92 * 16.32ms = 1.5s
#define IN_PIN_1 PB1
#define IN_PIN_2 PB2
#define OUT_PIN PB3

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int timer_count=0;

// Timer0 interrupt vector, called every 16.32ms -  1 / (F_CPU / 1024) * 255
// Sets OUT_PIN after TIMER_TARGET is reached
ISR (TIMER0_OVF_vect) { 
  if (timer_count == TIMER_TARGET) { 
    timer_count = 0; 
    TIMSK &= ~(_BV(TOIE0)); // Disable timer0 interrupt
    PORTB = (PORTB & ~_BV(OUT_PIN)) | // OUT_PIN = 0 if both buttons pressed, else 1 
            (((PINB & (_BV(IN_PIN_1) | _BV(IN_PIN_2))) != 0) << OUT_PIN);
  }
  else {timer_count++;}
}

// Pin-change interrupt vector, called when either IN_PIN is pressed
// Starts Timer0 if both IN_PINs are HIGH, else clears timer and resets OUT_PIN
ISR(PCINT0_vect) {
  // button_state = 0 if in_pins both pressed, else 1
  uint8_t button_state = (PINB & (_BV(IN_PIN_1) | _BV(IN_PIN_2))) == 0;
  if (button_state) {
    TCNT0 = 0; // reset counter to 0
    TIMSK |= _BV(TOIE0); // enable timer0 interrupt
  } else {
    TIMSK &= ~(_BV(TOIE0)); // disable timer0 interrupt
    // OUT_PIN = 0 if both buttons pressed, else 1
    PORTB = (PORTB & ~_BV(OUT_PIN)) | (!button_state << OUT_PIN);
  }
}

int main(void) {
  DDRB |= ~(_BV(IN_PIN_1) | _BV(IN_PIN_2)); // Set button pins as input
  DDRB |= _BV(OUT_PIN); // Set LED pin as output
  PORTB |= _BV(OUT_PIN); // Set LED HIGH to start

  sei(); // Enable interrupts
  GIMSK |= _BV(PCIE); // Enable pin-change interrupt
  PCMSK |= _BV(PCINT1) | _BV(PCINT2); // Set the pin-change interrupt pins

  TCCR0A = 0;   // Normal mode
  TCCR0B = 0;
  TCCR0B |= _BV(CS00) | _BV(CS02);   // Prescale timer (Divide clock) by 1024

  while(1){}

  return 0;
}

