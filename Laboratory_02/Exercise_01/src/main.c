#include "header.h"

ISR(TIMER1_COMPA_vect)
{
  flag = 1;
}

void configure_timer1()
{
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= ((1 << WGM12) | (1 << CS11)); // CTC mode, prescaler 8
  OCR1A = 1999;                           // n-1
  TIMSK1 |= (1 << OCIE1A);
}

void init_system()
{
  DDRD |= ((1 << LED_LEFT) | (1 << LED_RIGHT)); // LEDS OUTPUT
  PORTD &= ~((1 << LED_LEFT) | (1 << LED_RIGHT));

  DDRB &= ~((1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_ALL)); // BUTTONS INPUTS
  PORTB |= ((1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_ALL)); // PULL-UP buttons

  configure_timer1();
  sei();
}

int main()
{
  init_system();

  while (1)
  {
    if (flag == 1)
    {
      flag = 0;

      uint8_t buttons = buttons_setting();

      if (buttons & (1 << ALL)) 
        PORTD |= ((1 << LED_LEFT) | (1 << LED_RIGHT));
      else
      {
        if (buttons & (1 << LEFT))
          PORTD |= (1 << LED_LEFT);
        else
          PORTD &= ~(1 << LED_LEFT);

        if (buttons & (1 << RIGHT))
          PORTD |= (1 << LED_RIGHT);
        else
          PORTD &= ~(1 << LED_RIGHT);
      }
    }
  }
  return 0;
}

uint8_t buttons_setting()
{
  uint8_t status = 0;

  if (!(PINB & (1 << BUTTON_LEFT)))
    status |= (1 << LEFT);
  if (!(PINB & (1 << BUTTON_RIGHT)))
    status |= (1 << RIGHT);
  if (!(PINB & (1 << BUTTON_ALL)))
    status |= (1 << ALL);

  return status;
}