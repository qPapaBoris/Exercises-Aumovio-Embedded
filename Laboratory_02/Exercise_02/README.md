## Turn Signal System Simulation

An application will be developed to simulate a car turn signal system using three push buttons and two LEDs.

Pressing and holding the left button will activate the corresponding LED, which will blink intermittently.

The same behavior applies to the right button and its corresponding LED.

Pressing the middle button activates the hazard mode, in which both LEDs will blink simultaneously.

The hazard mode has priority over individual signaling. If a signaling button and the hazard button are pressed at the same time, the LEDs will operate in hazard mode.

The implementation must use a state machine based on switch-case instructions.

A header file must be used.

The solution must be implemented using direct register manipulation only (do not use predefined functions such as `digitalWrite()`).

Hardware configuration:

- LED_RIGHT is connected to pin 2
- LED_LEFT is connected to pin 3
- Push buttons are connected to pins 10, 11, and 12.

Wokwi link: https://wokwi.com/projects/459826213044202497