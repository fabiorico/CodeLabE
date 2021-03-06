/**
 * \mainpage Interfacing the MR32 platform
 *
 *	MR32 platform is aimed for Micro Rato competition
 * 
 * This library abstract the user from MR32 platform, provinding basic 
 * interfacing functions.
 * 
 */
 

/** 
 * \file labE.h
 *
 * \brief Functions for interfacing MR32 platform. 
 *	Based on  mr32.h and mr32.c of J.L.Azevedo - sweet.ua.pt/jla/
 * \date 25-03-2014 
 * 
 **/ 
 
 
#include <detpic32.h>

// boolean
#define TRUE 1					///< define TRUE, same as 1 
#define FALSE 0					///< define FALSE, same as 0 


/**
 * \name Start and Stop Button
 *   Reading button macro gives the state of the button. 1 if pressed, 0 if not pressed - Active high logic
 * */
///@{ 
#define START 	(!PORTBbits.RB3)		
#define STOP 	(!PORTBbits.RB4)
///@}

/**
 * \name LED Management
 *  	Writing 0 or 1 to LEDn macro turn off and on  the LED respectively. Reading LEDn macro gives LED state
 * */
///@{ 
#define LED1   LATEbits.LATE0 	
#define LED2   LATEbits.LATE1	
#define LED3   LATEbits.LATE2	
#define LED4   LATEbits.LATE3 	 
#define LED5   LATBbits.LATB15	
///@}



/**
 * \name Tick Flags
 *   After reseting the flag (i.e. tick10ms=0;) it will become set (i.e with value 1) after 10ms (aprox)
*/
///@{ 
unsigned char tick10ms;
unsigned char tick20ms;
unsigned char tick40ms;
unsigned char tick80ms;
unsigned char tick160ms;
///@}


/* Prototypes */

/** 
 * \fn led(int LEDn, int LEDst);
 *
 * \brief Change the state of [LEDn] to [LEDst]
 * 
 * \param LEDn integer value between 1 and 4
 * \param LEDst integer value, 0 or 1; change the stato to off and on respectively
 * 
 * \returns none
 */
void led(int LEDn, int LEDst);	


/** 
 * \fn delay_ms(unsigned int ms)
 *
 * \brief Halt the program execution for [ms] miliseconds
 * 
 * \param ms unsigned integer between [1-50000] that corresponds to wait period in miliseconds
 * 
 * \returns none
 */
void delay_ms(unsigned int ms);				

/** 
 * \fn delay_us(unsigned int us)
 *
 * \brief Halt the program execution for [us] micrseconds
 * 
 * \param us unsigned integer between [1-1000] that corresponds to wait period in microseconds
 * 
 * \returns none
 */
void delay_us(unsigned int ms);				

/** 
 * \fn setSpeed(int velL, int velR)
 *
 * \brief Define motor speed and direction. Speed is globaly available in velLeft and velRigth variables
 * 
 * \param velL	Left motor speed [-100 100]	
 * \param velR 	Rigth motor speed [-100 100]
 * 
 * \returns none
 */
void setSpeed(int velL, int velR);	

void initPIC32(void);						

