/*
 * wwdg.c - F0xx Watchdog
 */

#include "wwdg.h"

/*
 * Initialize the watchdog
 */
void wwdg_init(void)
{
	/* Enable WWDG clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	/* WWDG clock counter = (PCLK1 (48MHz)/4096)/8 = ~1465Hz (~683 us)  */
	WWDG_SetPrescaler(WWDG_Prescaler_8);

	/*
     * Set Window value to 127. WWDG counter should be refreshed only when the
     * counter is below 127 (and greater than 64) otherwise a reset will be
     * generated.
	 * 127 effectively disables the window
     */
	WWDG_SetWindowValue(127);

	/*
     * Enable WWDG and set counter value to 127
     * WWDG timeout = ~683 us * 64 = 43.69 ms 
	 * In this case the refresh window is
     * ~683 * (127-127) = 0ms < refresh window < ~683 * 64 = 43.69ms
	 */
	WWDG_Enable(127);
}

