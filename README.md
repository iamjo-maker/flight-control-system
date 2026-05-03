# flight-control-system
This project explores the development of a basic embedded flight control system. It focuses on signal processing, actuator control, and structured testing workflows.
Prototype implementation for testing core flight control logic; not yet a complete flight controller.

## Objectives
- Read and process signals
- Generate stable outputs for actuators
- Develop a modular control framework
- Validate logic through staged testing

## Architecture
- Microcontroller-based system (ESP32)
- Input: Digital Signal over I2C
- Output: PWM signals(v1)

## Implementation

### Core Control Code
- Reads input signals
- Processes and maps control logic
- Outputs actuator commands

### Testing Framework
A separate testing module was developed to:
- Validate signal handling
- Debug control responses
- Ensure safe operation before integration

## Hardware Context
Designed for integration with an RC platform, but tested independently during development.

## Limitations
- Prototype-level logic
- Only P-loop control
- Cannot be used directly 

## Future Work
- Complete PID-based stabilization
- Signal filtering and noise handling
- Full system integration
