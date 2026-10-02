# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## l2-task1

This blinky application was modified to utilize the three main leds on the nucleo-H7S3L8
(green, orange, red)

LOG_INF logs a concatnated state of the leds 
0x5 -> 0b101 -> Green & Red LED on
0x2 -> 0b010 -> Orange LED on

## l5-task1

**Note** the led example from the previous tasks can be built and ran along with the 
built-in zephyr `hello_world` example from the zephyr root. 

as such `hello_world` is not added to the repo and the led example from the previous tasks remain

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).

