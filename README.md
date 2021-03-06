# Smooth
C++ framework for writing applications based on Espressif's ESP-IDF.

## Overview

Smooth provides a set of classes that makes life as a developer easier compared to working directly with ESP-IDF & FreeRTOS APIs.
An application built with Smooth is entirely event driven and thread-safe*. Smooth utilizes the power of FreeRTOS, but hides all the complexities from the application programmer.

Traditionally, embedded systems require a fully static memory footprint after start-up. Smooth takes
a somewhat more pragmatic view on this; it utilizes the standard library (which is not memory static) to provide cleaner code,
at the cost of some extra used bytes of RAM. However, where it is appropriate, such as with the *Queue*, things are designed so
that the result is a memory static instance, i.e. a *smooth::ipc::Queue* will _not_ behave like an *std::vector*.  

Apart from hardware/IDF-specific classes, applications written using Smooth can be compiled and run on POSIX systems (e.g. Linux)
without any special considerations. 

*) To certain limits, of course.

### Provided functionality

#### Core
- Application initialization
- Wifi configuration / control
- Tasks
- Queues with support for proper C++ objects, not just plain data structures
- Timer Events
- Event-driven TCP Sockets
- System events

#### Hardware level

- Output
- Input
- Input with interrupt to event translation
- I2C Master Device class

### Application level

- MQTT Client
- Sensor BME280
- 16 channel I/O expander MCP23017
- RGB LED, i.e. WS2812(B), SK6812, WS2813, (a.k.a NeoPixel). 


## Installation

In your ESP-IDF projects's root folder, type the following to add `smooth` as a submodule.

```Bash
git submodule add https://github.com/PerMalmberg/Smooth.git components/smooth
```

### Sample applications

Please see the [test application in this repository.](https://github.com/PerMalmberg/Smooth-test)