/*
  Author: Papa Boris
  Timer1, 1ms, prescaler 8
*/

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define LED_RED PB0
#define LED_YELLOW PB1
#define LED_GREEN PB2
#define BUTTON PD2

typedef enum
{
  ST_IDLE,
  ST_RED,
  ST_YELLOW1,
  ST_GREEN,
  ST_YELLOW2
} State_t;

State_t state = ST_IDLE;

typedef struct
{
  uint8_t led;
  uint16_t time;
  State_t next;
} StateLights_t;

StateLights_t fsm[] =
{
  [ST_IDLE]    = {      0,        0,  ST_IDLE     },
  [ST_RED]     = { LED_RED,    5000,  ST_YELLOW1  },
  [ST_YELLOW1] = { LED_YELLOW, 3000,  ST_GREEN    },
  [ST_GREEN]   = { LED_GREEN,  4000,  ST_YELLOW2  },
  [ST_YELLOW2] = { LED_YELLOW, 3000,  ST_RED      }
};

volatile uint16_t tick;

ISR(TIMER1_COMPA_vect)
{
  tick++;
}

void configureTimer1()
{
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
  OCR1A = 1999;                         // n-1
  TIMSK1 |= (1 << OCIE1A);
}

void initSystem()
{
  DDRB |= (1 << LED_GREEN) | (1 << LED_RED) | (1 << LED_YELLOW);
  PORTB = 0;

  DDRD &= ~(1 << BUTTON);
  PORTD |= (1 << BUTTON);

  configureTimer1();
  sei();
}

int main()
{
  initSystem();
  static uint8_t prev_button_state = 1, now_button_state;
  static uint16_t counter = 0;

  while (1)
  {
    if (tick)
    {
      tick--;

      now_button_state = (PIND & (1 << BUTTON)) >> BUTTON;

      if (state == ST_IDLE)
      {
        if (prev_button_state && !now_button_state)
        {
          state = ST_RED;
          counter = 0;
        }
      }
      else
      {
        counter++;

        if (counter >= fsm[state].time)
        {
          counter = 0;
          state = fsm[state].next;
        }
        PORTB &= ~((1 << LED_RED) | (1 << LED_YELLOW) | (1 << LED_GREEN));
        PORTB |= (1 << fsm[state].led);
      }

      prev_button_state = now_button_state;
    }
  }
  return 0;
}