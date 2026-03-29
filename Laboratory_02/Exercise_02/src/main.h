#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#ifndef MAIN_H
#define MAIN_H

#define TOGGLE_LEDS 500 // TIMER FOR ON/OFF LEDS

/* LEDS HW CONFIG */
#define LED_RIGHT PD2
#define LED_LEFT PD3

/* BUTTONS HW CONFIG */
#define BUTTON_RIGHT PB2
#define BUTTON_HAZARD PB3
#define BUTTON_LEFT PB4

typedef enum
{
    HAZARD = 0,
    RIGHT,
    LEFT,
    IDLE
} config_t;

volatile uint8_t flag = 0;
volatile uint16_t counter = 0;
static uint8_t state = IDLE;
static uint8_t blink = 0;

void blink_update();
void priority_blinkers();
void blinkers_output();
void configure_timer1();
void init_system();

#endif