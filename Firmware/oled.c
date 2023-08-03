/*
 * oled.c - SSD1306 I2C OLED driver for STM32F030
 * 04-16-2015 E. Brombaugh
 */

#include <stdlib.h>
#include "oled.h"
#include "systick.h"
#include "font_8x8.h"
#include "logo.h"

#define SSD1306_I2C_ADDRESS_0   0x78	// 011110+SA0+RW - 0x3C, shifted left
#define SSD1306_I2C_ADDRESS_1   0x7A	// 011110+SA1+RW - 0x3D, shifted left
#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 * FLAG_TIMEOUT))

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

/* choose VCC mode */
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
//#define vccstate SSD1306_EXTERNALVCC
#define vccstate SSD1306_SWITCHCAPVCC

/* LCD I2C Address */
uint8_t oled_i2c_addr;

/* LCD frame buffer */
uint8_t oled_buffer[OLED_BUFSZ];

/*
 * Initialize the SSD1306
 */
void oled_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	
	/* turn on clock for I2C GPIO */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* Enable PA9, PA10 for I2C1 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);  // SCL
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4); // SDA

	/* turn on clock for I2C1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* The I2C clock is derived from the HSI */
	RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x00210507;
	I2C_Init(I2C1, &I2C_InitStructure);

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
	
	/* clear the frame buffer */
	oled_clear(0);
	
	/* probe for the I2C address */
    oled_i2c_addr = SSD1306_I2C_ADDRESS_1;
	if(oled_i2c_command(SSD1306_DISPLAYOFF))
	{
		/* Address 0 didn't respond so try Address 1 */
		oled_i2c_addr = SSD1306_I2C_ADDRESS_0;
		if(oled_i2c_command(SSD1306_DISPLAYOFF))
		{
			/* Address 1 didn't respond so hang here */
			while(1)
			{
			}
		}
	}
	
#ifdef TINY_OLED
	/* Init the OLED controller for 64x32 */
    oled_i2c_command(SSD1306_DISPLAYOFF);                    // 0xAE
    oled_i2c_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    oled_i2c_command(0x80);                                  // the suggested ratio 0x80
    oled_i2c_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    oled_i2c_command(0x1F);                                  // different for tiny
    oled_i2c_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    oled_i2c_command(0x00);                                   // no offset
	oled_i2c_command(SSD1306_SETSTARTLINE | 0x0);            // 0x40 | line
    oled_i2c_command(SSD1306_CHARGEPUMP);                    // 0x8D
	oled_i2c_command(0x14);                                  // enable?
    oled_i2c_command(SSD1306_MEMORYMODE);                    // 0x20
    oled_i2c_command(0x00);                                  // 0x0 act like ks0108
    oled_i2c_command(SSD1306_SEGREMAP | 0x1);                // 0xA0 | bit
    oled_i2c_command(SSD1306_COMSCANDEC);
    oled_i2c_command(SSD1306_SETCOMPINS);                    // 0xDA
    oled_i2c_command(0x12);
    oled_i2c_command(SSD1306_SETCONTRAST);                   // 0x81
	oled_i2c_command(0x8F);
    oled_i2c_command(SSD1306_SETPRECHARGE);                  // 0xd9
	oled_i2c_command(0xF1);
    oled_i2c_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    oled_i2c_command(0x40);
    oled_i2c_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    oled_i2c_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	oled_i2c_command(SSD1306_DISPLAYON);	                 // 0xAF --turn on oled panel
#else
	/* Init the OLED controller for 128x64 */
    oled_i2c_command(SSD1306_DISPLAYOFF);                    // 0xAE
    oled_i2c_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    oled_i2c_command(0x80);                                  // the suggested ratio 0x80
    oled_i2c_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    oled_i2c_command(0x3F);
    oled_i2c_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    oled_i2c_command(0x0);                                   // no offset
    oled_i2c_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    oled_i2c_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
		oled_i2c_command(0x10);
    else 
		oled_i2c_command(0x14);
    oled_i2c_command(SSD1306_MEMORYMODE);                    // 0x20
    oled_i2c_command(0x00);                                  // 0x0 act like ks0108
    oled_i2c_command(SSD1306_SEGREMAP | 0x1);
    oled_i2c_command(SSD1306_COMSCANDEC);
    oled_i2c_command(SSD1306_SETCOMPINS);                    // 0xDA
    oled_i2c_command(0x12);
    oled_i2c_command(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) 
		oled_i2c_command(0x9F);
    else 
		oled_i2c_command(0x8F);
    oled_i2c_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
		oled_i2c_command(0x22);
    else 
		oled_i2c_command(0xF1);
    oled_i2c_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    oled_i2c_command(0x40);
    oled_i2c_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    oled_i2c_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	
	oled_i2c_command(SSD1306_DISPLAYON);	                 // 0xAF --turn on oled panel
#endif
	
	/* update the display */
	oled_refresh();
}


/*
 * exception handler for I2C timeout
 */
uint32_t OLED_TIMEOUT_UserCallback(void)
{
	/* reset the I2C device */
	
	/* return error */
	return 1;
}

/*
 * Send a command byte to the OLED via I2C
 */
uint32_t oled_i2c_command(uint8_t cmd)
{
	uint32_t  Timeout;
	
	/* Configure slave address, nbytes, reload and generate start */
	I2C_TransferHandling(I2C1, oled_i2c_addr, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	Timeout = LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
	{
		if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
	}

	/* Send type for command */
	I2C_SendData(I2C1, 0);  

	/* Wait until TXIS flag is set */
	Timeout = LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
	{
		if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
	}

	/* Send command */
	I2C_SendData(I2C1, cmd);

	/* Wait until STOPF flag is set */
	Timeout = LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
	{
		if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
	}   

	/* Clear STOPF flag */
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

	return 0;
}

/*
 * Send a block of data bytes to the OLED via I2C
 */
uint32_t oled_i2c_data(uint8_t *data, uint8_t sz)
{
	uint32_t  Timeout;
	uint8_t i;
	
	/* Configure slave address, nbytes, reload and generate start */
	I2C_TransferHandling(I2C1, oled_i2c_addr, 1+sz, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	Timeout = LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
	{
		if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
	}

	/* Send type for data */
	I2C_SendData(I2C1, 0x40);  

	for(i=0;i<sz;i++)
	{
		/* Wait until TXIS flag is set */
		Timeout = LONG_TIMEOUT;
		while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
		{
			if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
		}

		/* Send data */
		I2C_SendData(I2C1, data[i]);
	}
	
	/* Wait until STOPF flag is set */
	Timeout = LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET)
	{
		if((Timeout--) == 0) return OLED_TIMEOUT_UserCallback();
	}   

	/* Clear STOPF flag */
	I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

	return 0;
}

/*
 * Get address of the frame buffer
 */
uint8_t *oled_get_fb(void)
{
	return oled_buffer;
}

/*
 * Send the frame buffer
 */
void oled_refresh(void)
{
	uint16_t i;
	
	oled_i2c_command(SSD1306_COLUMNADDR);
#ifdef TINY_OLED
	oled_i2c_command(32);   // Column start address (0 = reset)
	oled_i2c_command(32+OLED_W-1); // Column end address (127 = reset)
#else
	oled_i2c_command(0);   // Column start address (0 = reset)
	oled_i2c_command(OLED_W-1); // Column end address (127 = reset)
#endif
	
	oled_i2c_command(SSD1306_PAGEADDR);
	oled_i2c_command(0); // Page start address (0 = reset)
	oled_i2c_command(7); // Page end address

    for (i=0; i<OLED_BUFSZ; i++)
	{
		/* send a block of data */
		oled_i2c_data(&oled_buffer[i], 16);
		
		/* Adafruit only increments by 15 - why? */
		i+=15;
    }
}

/*
 * clear the display buffer
 */
void oled_clear(uint8_t color)
{
	uint16_t i;
	uint8_t byte = (color == 1) ? 0xFF : 0x00;
	
	for(i=0;i<OLED_BUFSZ;i++)
	{
		oled_buffer[i] = byte;
	}
}

/*
 * draw a single pixel
 */
void oled_drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	/* clip to display dimensions */
	if ((x >= OLED_W) || (y >= OLED_H))
	return;

	/* plot */
	if (color == 1) 
		oled_buffer[x+ (y/8)*OLED_W] |= (1<<(y%8));  
	else
		oled_buffer[x+ (y/8)*OLED_W] &= ~(1<<(y%8)); 
}

/*
 * invert a single pixel
 */
void oled_xorPixel(uint8_t x, uint8_t y)
{
	/* clip to display dimensions */
	if ((x >= OLED_W) || (y >= OLED_H))
	return;

	/* xor */
	oled_buffer[x+ (y/8)*OLED_W] ^= (1<<(y%8));  
}

/*
 * get a single pixel
 */
uint8_t oled_getPixel(uint8_t x, uint8_t y)
{
	uint8_t result;
	
	/* clip to display dimensions */
	if ((x >= OLED_W) || (y >= OLED_H))
	return 0;

	/* get byte @ coords */
	result = oled_buffer[x+ (y/8)*OLED_W];
	
	/* get desired bit */
	return (result >> (y%8)) & 1;
}

/*
 * Bresenham line draw
 */
void oled_drawline(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t color)
{
    /* clip to display dimensions */
    int16_t x0 = (X1 >= OLED_W) ? OLED_W-1 : X1;
    int16_t y0 = (Y1 >= OLED_H) ? OLED_H-1 : Y1;
    int16_t x1 = (X2 >= OLED_W) ? OLED_W-1 : X2;
    int16_t y1 = (Y2 >= OLED_H) ? OLED_H-1 : Y2;
    
    int16_t dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int16_t dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int16_t err = (dx>dy ? dx : -dy)/2, e2;

    for(;;)
    {
        oled_drawPixel(x0,y0,color);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

/* Draw character to the display buffer */
void oled_drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color)
{
	uint16_t i, j, col;
	uint8_t d;
    
    /* Only legal ASCII to save space */
    chr &= 0x7F;
	
    /* Raster the data into bitmap */
	for(i=0;i<8;i++)
	{
		d = fontdata[(chr<<3)+i];
		for(j=0;j<8;j++)
		{
			if(d&0x80)
				col = color;
			else
				col = (~color)&1;
			
			oled_drawPixel(x+j, y+i, col);
			
			// next bit
			d <<= 1;
		}
	}
}

/*
 * Draw magnified character to the display buffer
 */
void oled_drawmagchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, uint8_t sz)
{
	uint16_t i, j, col;
	uint8_t d;
    
    /* Only legal ASCII to save space */
    chr &= 0x7F;
	
    /* Raster the data into bitmap */
	for(i=0;i<8;i++)
	{
		d = fontdata[(chr<<3)+i];
		for(j=0;j<8;j++)
		{
			if(d&0x80)
				col = color;
			else
				col = (~color)&1;
			
			oled_drawrect(x+j*sz, y+i*sz, sz, sz, col);
            
			// next bit
			d <<= 1;
		}
	}
}

/*
 * draw a string to the display
 */
void oled_drawstr(uint8_t x, uint8_t y, char *str, uint8_t color)
{
	uint8_t c;
	
	while((c=*str++))
	{
		oled_drawchar(x, y, c, color&1);
		x += 8;
		if(x>120)
			break;
	}
	
	/* clear to end of line? */
	if(color&2)
	{
		while(x<128)
		{
			oled_drawchar(x, y, ' ', color&1);
			x += 8;
			if(x>120)
				break;
		}		
	}
}

/*
 * draw a rectangle in the buffer
 */
void oled_drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	uint8_t m, n=y, iw = w;
	
	/* scan vertical */
	while(h--)
	{
		m=x;
		w=iw;
		/* scan horizontal */
		while(w--)
		{
			/* invert pixels */
			oled_drawPixel(m++, n, color);
		}
		n++;
	}
}

/*
 * invert a rectangle in the buffer
 */
void oled_xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	uint8_t m, n=y, iw = w;
	
	/* scan vertical */
	while(h--)
	{
		m=x;
		w=iw;
		/* scan horizontal */
		while(w--)
		{
			/* invert pixels */
			oled_xorPixel(m++, n);
		}
		n++;
	}
}

/*
 * draw the logo into the buffer
 */
extern const char logo[];
void oled_draw_logo(void)
{
	uint8_t x, y, bit;
	
	for(x=0;x<OLED_W;x++)
	{
		for(y=0;y<51;y++)
		{
			bit = (logo[(x>>3)+(y<<4)]>>(7-(x%8)))&1;
			oled_drawPixel(x, y, 1-bit);
		}
	}
}
