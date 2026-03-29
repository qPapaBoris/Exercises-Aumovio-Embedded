#include "main.h"

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

  DDRB &= ~((1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_HAZARD)); // BUTTONS INPUTS
  PORTB |= ((1 << BUTTON_LEFT) | (1 << BUTTON_RIGHT) | (1 << BUTTON_HAZARD)); // PULL-UP buttons

  configure_timer1();
  sei();
}

int main()
{
    init_system();

    while(1)
    {
        if(flag)
        {
            flag = 0;
            blink_update();
            priority_blinkers();
            blinkers_output();
        }
    }
    return 0;
}

void blink_update()
{
    counter++;

    if (counter >= TOGGLE_LEDS) 
    {
        counter = 0;
        blink ^= 1;
    }
}

void priority_blinkers()
{
    if (!(PINB & (1 << BUTTON_HAZARD)))         state = HAZARD;
    else if (!(PINB & (1 << BUTTON_RIGHT)))     state = RIGHT;
        else if (!(PINB & (1 << BUTTON_LEFT)))  state = LEFT;
            else                                state = IDLE;
}

void blinkers_output()
{
    switch(state)
    {
        case HAZARD:
            if(blink)
                PORTD |= ((1 << LED_RIGHT) | (1 << LED_LEFT));
            else
                PORTD &= ~((1 << LED_RIGHT) | (1 << LED_LEFT));  
            break;
        case RIGHT:
            PORTD &= ~(1 << LED_LEFT);

            if(blink)
                PORTD |= (1 << LED_RIGHT);
            else
                PORTD &= ~(1 << LED_RIGHT); 
            break;
        case LEFT:
            PORTD &= ~(1 << LED_RIGHT);

            if(blink)
                PORTD |= (1 << LED_LEFT);
            else
                PORTD &= ~(1 << LED_LEFT); 
            break;
        case IDLE:
            PORTD &= ~((1 << LED_RIGHT) | (1 << LED_LEFT));
            break;
    }
}