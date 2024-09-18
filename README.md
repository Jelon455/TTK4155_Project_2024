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

# Components used in project:
- **ATmega162**: Microcontroller
- **MC7805**: Voltage regulator
- **IDT_7164SL**: SRAM external memory
- **SN74ALS573C**: Latch
- **SN74HC00**: NAND Gates
- **MAX233** : RS232 tranceiver
- **MAX156*** : ADC with a parallel interfance

# Check point reached:
- **Initial assembly of microcontroller and RS-232**: implementation for UART comuntcation, basic connection for ATmega162 including power supply, reset button, clock signal 
- **Address decoding and external RAM** : implementation of connection external memory SRAM with an microcontroler using latch to provide 16bit data, make memory mapping by using NAND gates

# To do list for project
- **Silder position** : improve an function for reading slider position, and calibrate position of slider
- **Reading ADC**: improve time for reading data
- **Connecting silder with low pass filter**: make connection between filter and slider
- **Voltage vs joystick**: make an expression explaining relationship between joystic angle and voltage
