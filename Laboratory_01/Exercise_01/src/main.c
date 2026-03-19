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
} BlinkDelay;

volatile uint8_t flag_tick;

void init()
{
  TCCR1A = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
  OCR1A = 1999;                         // n-1
  TIMSK1 |= (1 << OCIE1A);

  DDRB |= (1 << LED1) | (1 << LED2);

  PORTB |= (1 << BUTTON1) | (1 << BUTTON2);  // Pull-up button 1 - off, 0 - on / LED: 1 - on, 0 - off
}

ISR(TIMER1_COMPA_vect)
{
  flag_tick++;
}

int main()
{
  init();
  sei();

  static uint32_t current_ms_led1 = 0;
  static uint32_t current_ms_led2 = 0;

  while (1)
  {
    if (flag_tick)
    {
      flag_tick--;

      if (!(PINB & (1 << BUTTON1)))
      {
        current_ms_led1++;

        if (current_ms_led1 >= FAST_DELAY)
        {
          current_ms_led1 = 0;
          PORTB ^= (1 << LED1);
        }
      }
      else
      {
        current_ms_led1 = 0;
        PORTB &= ~(1 << LED1);
      }

      if (!(PINB & (1 << BUTTON2)))
      {
        current_ms_led2++;

        if (current_ms_led2 >= SLOW_DELAY)
        {
          current_ms_led2 = 0;
          PORTB ^= (1 << LED2);
        }
      }
      else
      {
        current_ms_led2 = 0;
        PORTB &= ~(1 << LED2);
      }
    }
  }
}