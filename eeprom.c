#include<pic.h>
#include<htc.h>
#include "Delay.h"
void eeprom_wrt(unsigned char a,unsigned char d)
{
while(WR==1); //Wait for write to finish
	EEADR  = a;
   	EEDATA = d;
	GIE    = 0;
	WREN   = 1;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR     = 1;
	WREN   = 0;
}

unsigned char eeprom_rd(unsigned char a)
{	
    EEADR = a;
    RD = 1;
    return(EEDATA);
}