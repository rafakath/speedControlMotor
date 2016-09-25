# speedControlMotor


Project Objective : To control the speed of DC motor

Microcontroller 	: 	16F877A (Microchip)
Language 		:	Embedded C
Compiler		: 	MPLAB 8.33
Simulation Software	:	Proteus 7.1


Hardware Description:

Internal EEROM : To store user set input value for speed controlling of motor.

7-Segment CA : for display purpose multiplexed 7-segment is implemented using Timer0 interrupt to reduce pin count.

IGBT : To supply power to  DC Motor 2Amps,250V and control accordingly depending upon PWM input from microcontroller. 

IGBT Driver : Inorder to drive IGBT driver circuit is build using BJT. 

Description:
To control speed of motor PWM technique is implement . It has five push button switches for user input, one for start, one for stop, one for entry/exit edit mode and two switches for changing value up/down for PWM and value will be display in 3 digit 7-segment.
