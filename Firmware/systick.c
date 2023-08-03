/*
 * systick.c - systick routines
 */

#include "systick.h"
#include "debounce.h"

/* uncomment for proto hardware */
//#define PROTO_BUTTONS

static __IO uint32_t TimingDelay, TimingCount;
debounce_state dbs_btn_short, dbs_btn_long, dbs_btn_talk;

/*
 * init the system tick infrastructure
 */
void systick_init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* set up GPIO for button & trigger input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* GPIOF Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

	/* set up GPIO for button & trigger input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* set up debounce objects for buttons */
	init_debounce(&dbs_btn_short, 15);
	init_debounce(&dbs_btn_long, 15);
	init_debounce(&dbs_btn_talk, 15);
	
	/* Init the timers */
	TimingDelay = 0;
	TimingCount = 0;

	/* start the 1ms system tick */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

/*
 * non-blocking compute goal for number of milliseconds delay
 */
uint32_t systick_goal_ms(uint32_t ms)
{
	return ms + TimingCount;
}

/*
 * non-blocking check goal for number of milliseconds delay
 */
uint32_t systick_check(uint32_t goal)
{
	return TimingCount < goal;
}

/*
 * blocking delay for number of milliseconds
 */
void systick_delayms(uint32_t ms)
{
	TimingDelay = ms;

	while(TimingDelay != 0);
}

/*
 * systick_get_button(n) - returns state of button #n
 */
uint8_t systick_get_button(uint8_t num)
{
	/* debounced state */
	switch(num)
	{
		case 0:
			return dbs_btn_short.state;
		case 1:
			return dbs_btn_long.state;
		case 2:
			return dbs_btn_talk.state;
		default:
			return 0;
	}
}

/*
 * systick_get_trigger(n) - returns state of trigger #n
 */
uint8_t systick_get_trigger(uint8_t num)
{
	if(num==0)
		return (((~GPIOA->IDR) >> 6)&1);	// short trig
	else
		return (((~GPIOA->IDR) >> 7)&1);	// long trig
}

/*
 * systick_get_switch() - returns state of opt switch
 */
uint8_t systick_get_switch(void)
{
	return ((~GPIOF->IDR)&1);
}

/*
 * IRQ handler
 */
void SysTick_Handler(void)
{
	/* debounce buttons */
#ifdef PROTO_BUTTONS
    /* Button hookups on prototype board */
	debounce(&dbs_btn_short, (((~GPIOA->IDR) >> 4)&1));
	debounce(&dbs_btn_long, (((~GPIOA->IDR) >> 5)&1));
	debounce(&dbs_btn_talk, (((~GPIOF->IDR) >> 1)&1));
#else
    /* Button hookups on production board */
	debounce(&dbs_btn_short, (((~GPIOA->IDR) >> 5)&1));
	debounce(&dbs_btn_long, (((~GPIOF->IDR) >> 1)&1));
	debounce(&dbs_btn_talk, (((~GPIOA->IDR) >> 4)&1));
#endif
    
	/* update ms delay timer */
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	
	/* update the global count */
	TimingCount++;
}

