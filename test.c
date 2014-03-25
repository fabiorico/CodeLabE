
#include "labE.h"


void doTheKit()
{
	char i=0;
	
	if(TRISE!=0)
		TRISE=0;
	LATE=1;
	
		for(i=0;i<3;i++)
			{
					PORTE=LATE << 1;
					delay_ms(75);
			}
		for(i=0;i<3;i++)
			{
					PORTE=LATE>>1;
					delay_ms(75);
			}
	
}
void doTheDance()
{
	int i=0;
	
	for(i=0;i<100;i++)
	{
				setSpeed(i,0-i);
				delay_ms(20);
	}
	
	for(i=0;i<100;i++)
	{
				setSpeed(100-i,-100+i);
				delay_ms(20);
	}
	for(i=0;i<100;i++)
	{
				setSpeed(0-i,i);
				delay_ms(20);
	}
	
	for(i=0;i<100;i++)
	{
				setSpeed(-100+i,100-i);
				delay_ms(20);
	}
}
int main ()
{
	initPIC32();

	LED1=0;

	printf("Press start to test motors\n"); //to see the output run bin/pterm 
	
	
	while(!START)
		doTheKit(); //loop until START is pressed
	
	
	doTheDance();
	
	while(1);
	
	return 0;
}
