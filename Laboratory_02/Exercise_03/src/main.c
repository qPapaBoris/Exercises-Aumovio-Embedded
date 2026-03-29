#include "main.h"

button_config_t button_car  = {0,1,1,1,0};
button_config_t button_ped  = {0,1,1,1,0};
button_config_t button_mode = {0,1,1,1,0};

traffic_states_t state = STATE_0_TRAFFIC; 

/* DEFAULT MODE FOR TRAFFIC LIGHTS*/
StateLights_t fsm[]=
{
  [STATE_0_TRAFFIC] = {LED1_GREEN,  LED2_RED,     3000, STATE_1_TRAFFIC},
  [STATE_1_TRAFFIC] = {LED1_YELLOW, LED2_RED,     1000, STATE_2_TRAFFIC},
  [STATE_2_TRAFFIC] = {LED1_RED,    LED2_GREEN,   3000, STATE_3_TRAFFIC},
  [STATE_3_TRAFFIC] = {LED1_RED,    LED2_YELLOW,  1000, STATE_0_TRAFFIC}
};

volatile uint8_t tick = 0;
volatile uint16_t counter = 0;


int main()
{
  init_system();
  while(1)
  {
    if(tick)
    {
      tick = 0;
      counter++;
      
      buttons_update();
      //update_mode();
      //output_fsm();
    }
  }
  return 0;
}

ISR(TIMER1_COMPA_vect)
{
  tick = 1;
}

void configure_timer1()
{
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
  OCR1A = 1999;                         // n-1
  TIMSK1 |= (1 << OCIE1A);
}

void init_system()
{
/* LEDS logic: '1' on/ '0' off */
  DDRD |= ((1 << LED1_RED) | (1 << LED1_YELLOW) | (1 << LED1_GREEN) | (1 << LED2_RED) | (1 << LED2_YELLOW) | (1 << LED2_GREEN));
  PORTD &= ~((1 << LED1_RED) | (1 << LED1_YELLOW) | (1 << LED1_GREEN) | (1 << LED2_RED) | (1 << LED2_YELLOW) | (1 << LED2_GREEN));

/* INTERN PULL UP BUTTONS */
  DDRB &= ~((1 << BUTTON_CAR) | (1 << BUTTON_PED) | (1 << BUTTON_MODE)); 
  PORTB |= ((1 << BUTTON_CAR) | (1 << BUTTON_PED) | (1 << BUTTON_MODE));
  
  configure_timer1();
  sei();
}

void buttons_update()
{
  button_update(&button_car, ((PINB & (1 << BUTTON_CAR)) >> BUTTON_CAR));
  button_update(&button_ped, ((PINB & (1 << BUTTON_PED)) >> BUTTON_PED));
  button_update(&button_mode, ((PINB & (1 << BUTTON_MODE)) >> BUTTON_MODE));
}

void button_update(button_config_t *button, uint8_t pin_value)
{
  button->now_button = pin_value;

  if (button->now_button != button->state)
  {
    button->debounce_counter++;

    if (button->debounce_counter >= DEBOUNCE_TIMER)
    {
      button->prev_button = button->state;
      button->state = button->now_button;
      button->debounce_counter = 0;

      if (button->state == 0 && button->prev_button == 1)
      {
        button->press_event++; /* DONT FORGET TO RESET THE VALUE IN STATE MACHINE */
      }
    }
  }
  else
  {
    button->debounce_counter = 0;
  }
}