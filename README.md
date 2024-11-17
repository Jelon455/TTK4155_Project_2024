# TTK4155_Project_2024
Project for NTNU's course TTK4155.
This term project aims at creating a **computer controlled electromechanical ping pong game**. The
electronic components, game boards and development tools needed to realize the game will be
handed out in the beginning of the semester. The main challenge will be to assemble the hardware
components and develop software for the microcontrollers, making a fully functional embedded
computer system that will enable you to play a refreshing game of ping pong. The term project
is in essence a rather comprehensive laboratory exercise which requires allocation of a substantial
amount of time for lab work throughout the semester. To make the project easier to complete, it
is divided into a number of smaller weekly exercises which will ensure continuity in the work and
necessary progress in the project.

# Overview
This repository contains the code and documentation for an Embedded Ping Pong Game, developed as part of the TTK4155 course at NTNU. The goal of this project is to design, build, and program an electromechanical ping pong game using embedded systems and custom-built hardware.
The project involves integrating various electronic components, developing microcontroller software, and assembling hardware into a fully functional embedded computer system.


# Project Goals
*Develop precise control over hardware components to simulate ping pong gameplay.
*Implement joystick-based user input.
Communication: Establish communication between microcontrollers using CAN (Controller Area Network).
User Interface: Display game state and scores on an OLED screen.
Motor and Sensor Control: Manage servo motors, IR sensors, and encoders for gameplay mechanics.

# Key Features
Microcontroller Integration: The game is powered by Atmel AVR (ATMega162) Node 1, supporting joystick input, OLED display, and CAN communication.
Motor and servo controll and IR handling is done by Arduino board ATSAM3X8E Node 2.

# Hardware Components:
Joystick for user interaction.
Servo motors for paddle movement.
Infrared (IR) sensors for ball detection.
MCP2515 CAN controller for inter-node communication.
OLED screen for real-time score display.

# Software Architecture:
Driver-level programming for ADC, SPI, CAN, and peripherals.
Modular design with separate drivers for each hardware component.
Real-time updates and performance optimization.

# Repository Structure
/Node1\
├── ADC_driver.c         # Analog-to-Digital Conversion (ADC) logic.\
├── CAN_driver.c         # Handles CAN communication.\
├── CAN_control.c        # Interface for the MCP2515 CAN controller.\
├── OLED_driver.c        # Manages the OLED display and text rendering.\
├── SPI_driver.c         # SPI protocol implementation.\
├── UART_driver.c        # Serial communication support.\
├── Memory_driver.c      # Managment of Atmega162 memory.\
├── main.c               # Entry point for Node 1's functionality.\

/Node2\
├── Motor_driver.c       # Controls motors for movement.\
├── Encoder_driver.c     # Reads encoder values for precision control and implemented PI conroller.\
├── pwm.c                # Pulse Width Modulation (PWM) for servo control.\
├── time.c               # Timing utilities for delays and scheduling.\
├── main.c               # Entry point for Node 2's functionality.\

# Peripheral Control:
ADC for joystick input and PWM for servo and motor control.

# Communication Protocols:
SPI and CAN for inter-node communication. UART for serial communication.

# How to Play
Start the System:\
Power up both nodes. Ensure the CAN bus is correctly terminated and connected.
Control Paddle:\
Use the joystick to move the paddle left or right.
Game Over:\
The game ends when the IR sensor detects a missed ball. The final score is displayed on the OLED screen.

# Acknowledgments
Course Materials: NTNU course TTK4155.
Instructors: Special thanks to the professors and teaching assistants for their guidance.
For additional details, refer to the course documentation.
