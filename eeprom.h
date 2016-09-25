#ifndef __EEPROM_H__
#define __EEPROM_H__
#include<pic.h>
#include<htc.h>
#include "Delay.h"
void eeprom_wrt(unsigned char,unsigned char);
unsigned char eeprom_rd(unsigned char);
#endif