#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#ifndef HEADER_H
#define HEADER_H

/* LEDS CONFIG*/
#define LED_LEFT PD2
#define LED_RIGHT PD3

/* BUTTONS CONFIG*/
#define BUTTON_LEFT PB2
#define BUTTON_RIGHT PB3
#define BUTTON_ALL PB4

typedef enum
{
  LEFT = 0,
  RIGHT,
  ALL
} buttons_config_t;

volatile uint8_t flag = 0;

uint8_t buttons_setting();
void configure_timer1();
void init_system();

#endif