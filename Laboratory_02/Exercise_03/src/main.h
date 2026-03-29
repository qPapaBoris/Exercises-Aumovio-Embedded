#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define DEBOUNCE_TIMER 20 /* ms for button debounce*/

/* LEDS HW FIRST TRAFFIC LIGHT */
#define LED1_RED    PD2
#define LED1_YELLOW PD3
#define LED1_GREEN  PD4

/* LEDS HW SECOND TRAFFIC LIGHT */
#define LED2_RED    PD5
#define LED2_YELLOW PD6
#define LED2_GREEN  PD7

/* BUTTONS HW */
#define BUTTON_CAR  PB0
#define BUTTON_PED  PB1
#define BUTTON_MODE PB2

/* BUTTONS SYSTEM */
typedef struct
{
    uint16_t debounce_counter;
    uint8_t now_button;
    uint8_t state;
    uint8_t prev_button;
    uint8_t press_event;
} button_config_t;

extern button_config_t button_car;
extern button_config_t button_ped;
extern button_config_t button_mode;

typedef enum
{
    MODE_DEFAULT = 0,
    MODE_DAY,
    MODE_NIGHT,
    MODE_TRAFFIC_LOW,
    MODE_TRAFFIC_HIGH,
    MODE_BLINK
} system_mode_t;

/* TRAFFIC LIGHT SYSTEM */
typedef enum
{
    STATE_0_TRAFFIC = 0,
    STATE_1_TRAFFIC,
    STATE_2_TRAFFIC,
    STATE_3_TRAFFIC
} traffic_states_t;

typedef struct
{
    uint8_t traffic_light_1;
    uint8_t traffic_light_2;
    uint16_t time;
    traffic_states_t next_state;
} StateLights_t;

extern volatile uint8_t tick;
extern volatile uint16_t counter;

void configure_timer1();
void init_system();
void buttons_update();
void button_update(button_config_t *button, uint8_t pin_value);
//void update_mode();
//void output_fsm();

#endif