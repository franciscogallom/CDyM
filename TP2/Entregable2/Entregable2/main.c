

#include <avr/io.h>
#include "sEOS.h"
#include "MEF.h"
#include "common.h"


int main(void)
{
    LCDinit();
	
	MEF_init();
	
	TIMER0_Init();
	
	
	
    while (1) 
    {
		sEOS_Dispatch_Tasks();
    }
	return 0;
	
}

