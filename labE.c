#include "labE.h"

static int velRight=0, velLeft=0; // motor speed

// ****************************************************************************
// initPIC32()
//
void initPIC32(void)
{
// Disable JTAG
	DDPCON = 3;

// Config Timer2, Timer3, OC1, OC2 and OC5
	T2CONbits.TCKPS = 5;	// 1:16 prescaler (i.e. fin = 625 KHz)
	PR2 = 6249;				// Fout = 20M / (32 * (6249 + 1)) = 100 Hz
	TMR2 = 0;				// Reset timer T2 count register
	T2CONbits.TON = 1;		// Enable timer T2 (must be the last command of 
							// the timer configuration sequence)
							//
	T3CONbits.TCKPS = 4;	// 1:32 prescaler (i.e. fin = 1.25 MHz)
	PR3 = 63;				// Fout = 20M / (16 * (63 + 1)) = 20000 Hz
	TMR3 = 0;				// Reset timer T2 count register
	T3CONbits.TON = 1;		// Enable timer T2 (must be the last command of 
							// the timer configuration sequence)
// Motor1 PWM

	OC1CONbits.OCM = 6; 	// PWM mode on OCx; fault pin disabled
	OC1CONbits.OCTSEL =1;	// Use timer T3 as the time base for PWM generation
	OC1RS = 0;				// 
	OC1CONbits.ON = 1;		// Enable OC1 module

// Motor2 PWM
	OC2CONbits.OCM = 6; 	// PWM mode on OCx; fault pin disabled
	OC2CONbits.OCTSEL =1;	// Use timer T3 as the time base for PWM generation
	OC2RS = 0;				// 
	OC2CONbits.ON = 1;		// Enable OC2 module

// Servo PWM
	OC5CONbits.OCM = 6; 	// PWM mode on OCx; fault pin disabled
	OC5CONbits.OCTSEL =0;	// Use timer T2 as the time base for PWM generation
	OC5RS = 0;				// 
	OC5CONbits.ON = 1;		// Enable OC5 module


	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 1;
	IEC0bits.T2IE = 1;		// Enable Timer 2 interrupts
	IEC0bits.T2IE = 1;		

	EnableInterrupts();

// ****************************************************************************
// IO Config
//
//  1-Bridge control
	STDBY = 1;				// Half-Bridge ON

	M1_IN1 = M1_IN2 = 0;	// STOP
	M2_IN1 = M2_IN2 = 0;	// STOP

	TRISCbits.TRISC14 = OUT;	// STDBY
	TRISBbits.TRISB5 = OUT;  	// M1_IN1
	TRISCbits.TRISC13 = OUT; 	// M1_IN2

	TRISBbits.TRISB13 = OUT;  	// M2_IN1
	TRISFbits.TRISF3 = OUT;  	// M2_IN2

//  2-Leds
	LATECLR = 0x000F;		// Leds 4-1 OFF
	LATBCLR = 0x8000;		// Led 5 OFF
	TRISECLR = 0x000F;		// RE3-0 as output
	TRISBCLR = 0x8000;		// RB15 as output

//  3-Sensors
	LATBbits.LATB10 = 0;		// Disable Obstacle sensors output
	TRISBbits.TRISB10 = OUT;	// EN_OBST_SENS as output
	TRISBbits.TRISB9 = IN;		// IV BEACON as input

    LATECLR = 0x0020;			// Disable line sensor
	TRISEbits.TRISE5 = OUT;		// EN_GND_SENS as output

	LATD = LATD | 0x00EC;		// Line sensor: output latches must be set
	TRISD = TRISD & ~(0x00EC);	// Line sensor: 5 bits as output

//  4- start/stop buttons 
	CNPUE = CNPUE | 0x60;		// Activate weak pull-ups in input ports RB3 and RB4

// ADC Config
	AD1PCFGbits.PCFG0 = 0;		// RB0 configured as analog input (AN0)
	AD1PCFGbits.PCFG1 = 0;		// RB1 configured as analog input (AN1)
	AD1PCFGbits.PCFG2 = 0;		// RB2 configured as analog input (AN2)
	AD1PCFGbits.PCFG6 = 0;		// RB6 configured as analog input (AN6)
	AD1PCFGbits.PCFG7 = 0;		// RB7 configured as analog input (AN7)
	AD1PCFGbits.PCFG11 = 0;		// RB11 configured as analog input (AN11)

	AD1CON1bits.SSRC = 7;		// Conversion trigger: internal counter ends
								// sampling and starts conversion
	AD1CON1bits.CLRASAM = 1;	// Stop conversions when the 1st A/D converter
								// interrupt is generated. At the same time,
								// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16;		// Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 2 - 1;	// Interrupt is generated after 2 samples
	AD1CON1bits.ON = 1;			// Enable A/D converter
}

void delay_ms(unsigned int ms)
{
/*
 * Function:	Delay function. Stops program execution for "ms" miliseconds
 * input: 		Number of miliseconds to wait
 * output: 		None
 */
	ms = ms > 50000 ? 50000 : ms;

	resetCoreTimer();
	while(readCoreTimer() <= (20000 * ms));
}

void delay_us(unsigned int us)
{
/*
 * Function:	Delay function. Stops program execution for "us" microseconds
 * input: 		Number of microseconds to wait
 * output: 		None
 */
	us = us > 1000 ? 1000 : us; //if greather that 1000 consider using delay_ms

	resetCoreTimer();
	while(readCoreTimer() <= (20 * us));
}

void led(int LEDn, int LEDst)
{
/*
 * Function:	Manage LED[1-4] state
 * input: 	LEDn  - Led Number, 1 2 3 or 4
 * 			LEDst - Led State, 1 or 0
 * output: 	None
 */
 
	int mask=1;

	LEDn = LEDn < 1 ? 1 : LEDn;
	LEDn = LEDn > 4 ? 4 : LEDn;
	LEDst = LEDst > 0 ? 1 : 0;

	mask = mask << (LEDn - 1);
	
	if(LEDst) //turn on
	{
		
		LATE = LATE | mask;
	}
	else // turn off
	{
		LATE = LATE & ~mask;
	}
}

void setSpeed(int velL, int velR)
{
/*
 * Function:	User API to set motor speed 
 * input: 	Left Motor Speed (velL) [0-100]
 * 			Righ Motor Speed (velR) [0-100]
 * output: 	None
 */
 
	DisableInterrupts();
	velLeft = velL;
	velRight = velR;
	EnableInterrupts();
}
void actuateMotors()
{

	velLeft = velLeft > 100 ? 100 : velLeft;
	velLeft = velLeft < -100 ? -100 : velLeft;

	velRight = velRight > 100 ? 100 : velRight;
	velRight = velRight < -100 ? -100 : velRight;
	
	if(velLeft < 0)
	{
		velLeft = -velLeft;
		M1_REVERSE;
	}
	else
	{
		M1_FORWARD;
	}

	if(velRight < 0)
	{
		velRight = -velRight;
		M2_REVERSE;
	}
	else
	{
		M2_FORWARD;
	}
	OC1RS = ((PR3+1)*velLeft) / 100;
	OC2RS = ((PR3+1)*velRight) / 100;
}	

void _int_(_TIMER_2_VECTOR) isr_t2(void)
{
/*
 * Function:	TMR2 ISR - Motor Task 10 ms
 * input: 	None
 * output: 	None
 */

	actuateMotors();			// velLefteft, velRight are global vars
												// set by SetVel2()

	IFS0bits.T2IF = 0;
}
