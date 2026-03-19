/*
  Author: Papa Boris

  Timer 1ms, 16MHz
  T = (n*k*p)/ f_cpu_clk - >
  0,001 = (n*k*p)/ 16000000 ->
  -> n = 2000, with k = 1, p(prescaler) = 8, TIMER1
*/

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define LED1    PB0
#define LED2    PB2
#define BUTTON1 PB1
#define BUTTON2 PB3

typedef enum
{
  FAST_DELAY = 50,
  SLOW_DELAY = 300
}BlinkDelay;

volatile uint32_t current_ms;

void init()
{
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
  OCR1A = 1999;                         // n-1
  TIMSK1 = (1 << OCIE1A);

  DDRB |= (1 << LED1) | (1 << LED2);
  
  PORTB = (1 << BUTTON1) | (1 << BUTTON2);  // Pull-up button 1 - off, 0 - on / LED: 1 - on, 0 - off
}

ISR(TIMER1_COMPA_vect)
{
  current_ms++;
}

int main()
{
  init();
  sei();
  
  while (1)
  {
    static uint32_t last_check_LED1 = 0;
    static uint32_t last_check_LED2 = 0;

    if (!(PINB & (1 << BUTTON1)))
    {
      if (current_ms - last_check_LED1 >= FAST_DELAY)
      {
        last_check_LED1 = current_ms;
        PORTB ^= (1 << LED1);
      }
    }

    if (!(PINB & (1 << BUTTON2)))
    {
      if (current_ms - last_check_LED2 >= SLOW_DELAY)
      {
        last_check_LED2 = current_ms;
        PORTB ^= (1 << LED2);
      }
    }
  }
  return 0;
}