/*
 * main.c
 *
 * Part of e951 - STM32F030F4P6 based OLED, ADC, Serial expansion for E950
 * Copyright 2015/2016 E. Brombaugh
 ******************************************************************************
 * Changelog
 *
 * date: 2015-11-07 V0.0
 * First release with limited features for board bringup
 * 1) ADC input on PA0, PA1
 * 2) I2C OLED drive on PA9, PA10 @ address 0x78, no reset line.
 * 3) PA0,1 voltages drives 0-100 display on OLED
 * 4) Uses HSI internal oscillator
 * 5) USART on PA2, PA3
 *
 * date: 2015-11-20 V0.1
 * More tests for board bringup
 * 1) All switch / button / trigger inputs mirrored on display
 * 2) Clear word to end of line
 * 3) continuous serial output 
 *
 * date: 2015-11-21 V0.2
 * Working on functional operation w/ diagnostics
 * 1) Loop Logic (off/short/long, live & latching select via switch)
 * 2) Bank CV hysteresis & quantization. Pot will be atten, not ADC input.
 * 3) Serial output of loop and bank info via 1-char msgs
 * 4) Main loop executes faster than OLED update
 * 5) IRQ-driven serial input
 * 6) 1-line serial rx diagnostic on OLED
 *
 * date: 2015-11-21 V0.3
 * Synced up with E950 v0.15
 * 1) multiple RX characters per OLED update
 *
 * date: 2015-11-27 V0.4
 * Synced up with E950 v0.16
 * 1) Handles 6-byte stats packet
 * 2) Generates ACK for status packet
 *
 * date: 2015-12-04 V0.5
 * 1) LPC Text tracks E950 word/bank
 * 2) VCO Text tracks bank
 *
 * date: 2015-12-05 V0.6
 * 1) Flag continuous mode
 * 2) VCO wave index adjusted for 0-15 always
 * 3) Freq decimal pt.
 *
 * date: 2015-12-06 V0.7
 * 1) Talk button
 *
 * date: 2015-12-06 V0.8
 * 1) Probe both I2C addresses
 *
 * date: 2016-08-30 V0.9
 * 1) Swap switch inputs per Paul for new HW design
 * 2) Delete diagnostic text on display
 * 3) VCO display has thumbnails, bank and wave
 * 4) Speech display re-arranged
 *
 * date: 2016-08-31 V0.10
 * 1) invert wave thumbnails per Paul
 * 2) Speech display re-arranged
 * 3) Enlarged loop mode indicators
 * 4) Cleanup compiler warnings, disable unused ext_freq calcs
 * 5) Adjust speed range to avoid negative values
 *
 * date: 2016-08-31 V0.11
 * 1) Remove corrupted Speed/Duration indicator
 * 2) Speech display re-arranged
 * 3) Enlarged loop mode indicators to 3x
 *
 * date: 2016-09-17 V0.12
 * 1) Invert Bank CV response
 *
 * date: 2016-11-07 V0.13
 * 1) Fix JLR's bank/index bug
 * 2) Speech ROM number 1/2/3 on screen
 * 3) Wave bank A/B on screen
 * 4) Msg to screen when disconnected from E950
 * 5) Limit to 500Hz polling
 *
 * date: 2016-11-18 V0.14
 * 1) add watchdog
 * 2) VCO Bank on top line
 *
 * date: 2016-11-22 V0.15
 * 1) renumber speech banks
 *
 * date: 2016-11-22 V0.16
 * 1) Tweak bank cv quantization to prevent -1 resting state.
 * 2) Remove unused ADC input 1 processing
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f0xx.h"
#include "systick.h"
#include "usart.h"
#include "printf.h"
#include "oled.h"
#include "adc.h"
#include "words.h"
#include "wave_bank_dec.h"
#include "wwdg.h"

/*
 * recursive int to ascii sub-function
 */
int i2a(char *s, int n){
    div_t qr;
    int pos;

    if(n == 0) return 0;

    qr = div(n, 10);
    pos = i2a(s, qr.quot);
    s[pos] = qr.rem + '0';
	
    return pos + 1;
}

/*
 * int to ascii
 */
char* my_itoa(char *output_buff, int num){
    char *p = output_buff;
    if(num < 0){
        *p++ = '-';
        num *= -1;
    } else if(num == 0)
        *p++ = '0';
	
    p[i2a(p, num)]='\0';
    return output_buff;
}

/*
 * scale with hysteresis 
 */
void hyst(uint16_t *prev_val, uint16_t new_val, uint16_t scale)
{
	int16_t diff = new_val - (*prev_val*scale);
	
	diff = diff < 0 ? -diff : diff;
	
	/* noise threshold */
	if(diff > 20)
		*prev_val = new_val / scale;
}

/*
 * main routine
 */
int main(void)
{
	char number_str[17];
	int16_t c, decode_state = 0;
	uint8_t ext_bank = 0, ext_glitch = 0, ext_cont = 0;
	//uint32_t ext_freq_temp = 0, ext_freq = 0;
	uint8_t ext_morph_dur = 0;
	uint8_t ext_idx_word_temp = 0, ext_idx_word = 0;
	int8_t sb, lb, st, lt, ss, ls;
	int8_t sp = 0, lp = 0, se, le;
	int8_t prev_loop_state = -1, loop_state = 0;
	uint16_t prev_bank = 255, bank, pot;
	uint8_t talk, prev_talk = 255;
	uint32_t display_delay_goal;
	uint32_t disconnect_cnt = 500;
    
	/* initialize the hardware */
	systick_init();
	systick_delayms(10);	// let OLED power up a bit before initializing
	usart_init();
	init_printf(0,usart_putc);

	oled_init();
	adc_init();
	
	/* draw logo for 2 secs */
	oled_draw_logo();
	oled_drawstr(24,56, "E951 V0.16", 1);
	oled_refresh();
	oled_clear(0);
	systick_delayms(2000);		
	oled_refresh();
		
	/* turn on watchdog with 683us*63 = 43.69ms timeout */
	wwdg_init();
	
	/* Loop forever */
	display_delay_goal = systick_goal_ms(20);
	while(1)
	{
        /* Limit polling to 500Hz */
        systick_delayms(2);
        
        /* tickle the watchdog */
        WWDG_SetCounter(127);
        
		/* get CV value scaled to 0-20 */
		hyst(&bank, 4096-adc_get_data(1), 196);
		
		/* get talk state */
		talk = systick_get_button(2);
		
		/* loop logic */
		sb = systick_get_button(0);
		st = systick_get_trigger(0);
		lb = systick_get_button(1);
		lt = systick_get_trigger(1);
		ss = sb | st;
		ls = lb | lt;
		if(systick_get_switch())
		{
			/* latching mode */
			/* rising edge detect */
			if((sp==0) & (ss == 1))
				se = 1;
			else
				se = 0;
			sp = ss;
			
			if((lp==0) & (ls==1))
				le = 1;
			else
				le = 0;
			lp = ls;
			
			switch(loop_state)
			{
				case 0:
					/* off */
					if(se)
						loop_state = 1;	// advance to short
					else if(le)
						loop_state = 2;
					break;
				
				case 1:
					/* short */
					if(se)
						loop_state = 0;	// return to off
					else if(le)
						loop_state = 2; // advance to long
					break;
				
				case 2:
					/* long */
					if(se)
						loop_state = 1;	// advance to short
					else if(le)
						loop_state = 0; // return to off
					break;
				
				default:
					/* illegal - return to off */
					loop_state = 0;
			}
		}
		else
		{
			/* live mode */
			if(ss & ~ls)
				loop_state = 1;
			else if(ls & ~ss)
				loop_state = 2;
			else
				loop_state = 0;
			
			/* update previous states - not used */
			sp = ss;
			lp = ls;
		}
		
		/* send serial msgs */
		if(loop_state != prev_loop_state)
		{
			usart_putc(NULL, loop_state + '@');
			prev_loop_state = loop_state;
		}

		if(bank != prev_bank)
		{
			usart_putc(NULL, bank + '`');
			prev_bank = bank;
		}
		
		if(talk != prev_talk)
		{
			usart_putc(NULL, talk + 0x20);
			prev_talk = talk;
		}
		
		/* receive & parse status pkt */
		while((c = usart_getc())!=EOF)
		{
			/* look for packet header */
			if(c&0x80)
				decode_state = 0;
			
			/* update state */
			switch(decode_state)
			{
				case 0:
					/* header w/ bank */
					ext_bank = c&0x0f;
					ext_cont = ((c&0x10)>>4);
					ext_glitch = ((c&0x20)>>5);
					ext_idx_word_temp = ((c&0x40)<<1);
					decode_state++;
					break;
				
				case 1:
					/* freq high 7 */
					//ext_freq_temp = c&0x7f;
					decode_state++;
					break;
				
				case 2:
					/* freq mid 7 */
					//ext_freq_temp = (ext_freq_temp<<7) | (c&0x7f);
					decode_state++;
					break;
				
				case 3:
					/* freq low 7 */
					//ext_freq = (ext_freq_temp<<7) | (c&0x7f);
					decode_state++;
					break;
				
				case 4:
					/* morph or duration */
					ext_morph_dur = c&0x7f;
					decode_state++;
					break;
				
				case 5:
					/* wave index or word */
					ext_idx_word = ext_idx_word_temp + (c&0x7f);
					usart_putc(NULL, 128);	// ACK!
					decode_state++;
                
                    /* reset counter */
                    disconnect_cnt = 0;
					break;
				
				default:
				case 6:
					/* hold at illegal state for next header */
					break;
			}
		}
        
        /* update disconnect counter */
        disconnect_cnt++;
		
		/* Update OLED */
		if(!systick_check(display_delay_goal))
		{
            /* clear screen */
            oled_clear(0);
            
#if 0
			/* E950 status */
			my_itoa(number_str, ext_bank);
			oled_drawstr(0,10, number_str, 3);
			
			my_itoa(number_str, ext_freq/10);
			my_itoa(temp_str, ext_freq%10);
			strcat(number_str, ".");
			strcat(number_str, temp_str);
			oled_drawstr(32,10, number_str, 3);
			
			my_itoa(number_str, ext_morph_dur);
			oled_drawstr(0,20, number_str, 3);
			
			my_itoa(number_str, ext_idx_word);
			oled_drawstr(32,20, number_str, 3);
			
			my_itoa(number_str, ext_glitch);
			oled_drawstr(64,20, number_str, 3);
#endif
			
			if(disconnect_cnt > 500)
            {
                /* No connect text */
                oled_drawstr(0,28, "  Disconnected", 1);
            }
            else
            {
                /* VCO or Speech */
                if(ext_bank < 2)
                {
                    /* VCO */
                    uint8_t x,y,x1,y1,x2,y2;
                    int16_t acc;
                    
                    /* Top Line and bank indicator */
                    oled_drawstr(0,0, "   VCO Bank ", 1);
                    if(ext_bank == 0)
                        oled_drawstr(96,0, "A", 3);
                    else
                        oled_drawstr(96,0, "B", 3);
                    
                    /* dividing line */
                    oled_drawrect(0,9,128, 1, 1);
                     
                    /* Low wave */
                    oled_drawstr(0,15, "Waves ", 1);
                    my_itoa(number_str, ext_idx_word);
                    oled_drawstr(48,15, number_str, 1);
                    oled_drawstr(64,15, ":", 1);
                    
                    /* High wave */
                    my_itoa(number_str, ext_idx_word+1);
                    oled_drawstr(72,15, number_str, 3);
                    
                    /* draw thumbnail wave */
                    x1=y1=0;
                    for(x=0;x<32;x++)
                    {
                        acc = wave_bank_dec[ext_idx_word*32+x]*(127-ext_morph_dur);
                        acc += wave_bank_dec[(ext_idx_word+1)*32+x]*ext_morph_dur;
                        y = (acc>>7)^0x80;

                        /* line draw */
                        x2=x*2+32;
                        y2=63-(y>>3);
                        if(x>1)
                            oled_drawline(x1,y1,x2,y2,1);
                        x1=x2;
                        y1=y2;
                    }
                }
                else
                {
                    /* Speech */
                    uint16_t rom, u_idx;
                
                    /* Operating mode */
                    oled_drawstr(0,0, " Speech ROM ", 1);
                    
                    /* ROM indicator */
                    rom = (ext_bank-2)>>2;
                    my_itoa(number_str, rom+1);                
                    oled_drawstr(104,0, number_str, 1);
                    oled_drawrect(0,9,128, 1, 1);
                    
                    /* Loop Mode */
                    oled_drawstr(64,18, "Loop", 1);
                    switch(loop_state)
                    {
                        case 0:
                            //oled_drawstr(48,19, " ", 3);
                            oled_drawmagchar(100,12,'-',1,3);
                            break;
                        case 1:
                            //oled_drawstr(48,19, "S", 3);
                            oled_drawmagchar(100,12,'S',1,3);
                            break;
                        case 2:
                            //oled_drawstr(48,19, "L", 3);
                            oled_drawmagchar(100,12,'L',1,3);
                            break;
                        default:
                            //oled_drawstr(48,19, " ", 3);
                            oled_drawmagchar(100,12,' ',1,3);
                    }
                    
                    /* Bank Text */
                    oled_drawstr(0,14, "Bank   ", 1);
                    my_itoa(number_str, ext_bank-1);
                    oled_drawstr(40,14, number_str, 1);

                    /* Glitch */
                    if(ext_glitch)
                        oled_drawstr(0,25, "Glitch", 1);
                    
                    /* Word Index */
                    oled_drawstr(0,35, "Word ", 1);
                    my_itoa(number_str, ext_idx_word);
                    oled_drawstr(40,35, number_str, 3);
                    
                    /* Continuous */
                    if(ext_cont)
                        oled_drawstr(88,35, "Cont.", 3);
                
                    /* Utterance */
                    oled_drawrect(0,44,128, 1, 1);
                    u_idx = rom_offset[rom] + ext_idx_word;
                    if(u_idx <= MAX_WORDS)  // check if legal index
                        oled_drawstr(0,50, (char *)words[u_idx], 3);
                }
			}
            
			oled_refresh();
			
			/* next update time */
			display_delay_goal = systick_goal_ms(20);
		}
	}
}
