

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define WIRE_WRITE Wire.write
#include <avr/pgmspace.h>
#include "OLED_SSD1306.h"
#include "oledfont.c"
#include <Wire.h>

int8_t _i2caddr, _vccstate;

OLED_SSD1306::OLED_SSD1306()
{
  
 
}
void OLED_SSD1306::ssd1306_command(uint8_t c) 
{
	uint8_t control = 0x00;   // Co = 0, D/C = 0
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(control);
    WIRE_WRITE(c);
    Wire.endTransmission();
}	

void OLED_SSD1306:: begin(unsigned char address)
{
	_i2caddr = address;
	_vccstate = SSD1306_SWITCHCAPVCC;
	unsigned char vccstate = _vccstate;
	Wire.begin();
	delay(100);
	
	ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x9F); }
    else 
      { ssd1306_command(0xCF); }
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);       	// 0xA6
	ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

/*
ssd1306_command(0xAE);//display off
  ssd1306_command(0x00);//Set Memory Addressing Mode
  ssd1306_command(0x10);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  ssd1306_command(0x40);//Set Page Start Address for Page Addressing Mode,0-7
  ssd1306_command(0x81);//Set COM Output Scan Direction
  ssd1306_command(0xCF);//---set low column address
  ssd1306_command(0xA1);//---set high column address
  ssd1306_command(0xC8);//--set start line address
  ssd1306_command(0xA6);//--set contrast control register
  ssd1306_command(0xA8);
  ssd1306_command(0x3F);//--set segment re-map 0 to 127
  ssd1306_command(0xD3);//--set normal display
  ssd1306_command(0x00);//--set multiplex ratio(1 to 64)
  ssd1306_command(0xD5);//
  ssd1306_command(0x80);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  ssd1306_command(0xD9);//-set display offset
  ssd1306_command(0xF1);//-not offset
  ssd1306_command(0xDA);//--set display clock divide ratio/oscillator frequency
  ssd1306_command(0x12);//--set divide ratio
  ssd1306_command(0xDB);//--set pre-charge period
  ssd1306_command(0x40);//
  ssd1306_command(0x20);//--set com pins hardware configuration
  ssd1306_command(0x02);
  ssd1306_command(0x8D);//--set vcomh
  ssd1306_command(0x14);//0x20,0.77xVcc
  ssd1306_command(0xA4);//--set DC-DC enable
  ssd1306_command(0xA6);//
  ssd1306_command(0xAF);//--turn on oled panel 
*/
}
void OLED_SSD1306:: clearscreen()
{
	ssd1306_command(SSD1306_COLUMNADDR);
	ssd1306_command(0);   // Column start address (0 = reset)
	ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

	ssd1306_command(SSD1306_PAGEADDR);
	ssd1306_command(0); // Page start address (0 = reset)
	
	ssd1306_command(7); // Page end address
	
	 for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) 
	 {
      // send a bunch of data in one xmission
      Wire.beginTransmission(_i2caddr);
      WIRE_WRITE(0x40);
      for (uint8_t x=0; x<16; x++) 
	  {
			WIRE_WRITE(0x00);
			i++;
      }
      i--;
      Wire.endTransmission();
    }
	
/*	
	 unsigned char m,n;
  for(m=0;m<8;m++)
  {
    ssd1306_command(0xb0+m);	//page0-page1
    ssd1306_command(0x00);		//low column start address
    ssd1306_command(0x10);		//high column start address
    //Begin_IIC_Data();
	Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(0x40);
    for(n=0;n<128;n++)
    {
      WIRE_WRITE(0x00);
    }
    Wire.endTransmission();
  }
  */
}

void OLED_SSD1306::setpos(unsigned char x, unsigned char y)
{
/*  Wire.beginTransmission(_i2caddr);
  WIRE_WRITE(0x00);	//write command
  
  
  WIRE_WRITE(0xb0+y);
  WIRE_WRITE(((x&0xf0)>>4)|0x10);//|0x10
  WIRE_WRITE((x&0x0f)|0x01);//|0x01
  
  Wire.endTransmission();
  */
    ssd1306_command(SSD1306_COLUMNADDR);
	ssd1306_command(x);   // Column start address (0 = reset)
	ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

	ssd1306_command(SSD1306_PAGEADDR);
	unsigned char yy = map(y,0,64,0,8);
	ssd1306_command(yy); // Page start address (0 = reset)
	
	ssd1306_command(7); // Page end address
 }

/*void OLED_SSD1306::Char_F6x8_(unsigned char x, unsigned char y, char* ch)
{
  unsigned char c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if(x>126)
    {
      x=0;
      y++;
    }
    setpos(x,y);
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(0x40);
    
    for(i=0;i<6;i++)
    {
      WIRE_WRITE(font6x8[c*6+i]);
	  
    }
     Wire.endTransmission();
    x += 6;
    j++;
  }
}
*/
 void OLED_SSD1306::Char_F6x8(unsigned char x, unsigned char y, String ch)
{
  unsigned char c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if(x>126)
    {
      x=0;
      y++;
    }
    setpos(x,y);
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(0x40);
   char dat[9];
   unsigned int add = c*6;
  
	for(i=0;i<6;i++)
    {
		dat[i]=	  pgm_read_byte(&font6x8[add+i]);
	}
	WIRE_WRITE(dat,6);
	
     Wire.endTransmission();
    x += 6;
    j++;
  }
}

void  OLED_SSD1306::Char_F8x16(unsigned char x, unsigned char y,String ch)
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		
		unsigned char dat[20];
		unsigned char cnt_dat=0;
		for(i=0;i<8;i++)
		{
			dat[cnt_dat]= pgm_read_byte(&font8X16[c*16+i]);
			cnt_dat++;
		}
		
		Serial.print("\r\n");
		for(i=0;i<8;i++)
		{
		 dat[cnt_dat]= pgm_read_byte(&font8X16[c*16+i+8]);
		 cnt_dat++;
		}
		
		setpos(x,y);
		Wire.beginTransmission(_i2caddr);
		WIRE_WRITE(0x40);
		WIRE_WRITE(dat,8);
		Wire.endTransmission();
		setpos(x,y+8);
		Wire.beginTransmission(_i2caddr);
		WIRE_WRITE(0x40);
		WIRE_WRITE(dat+8,8);
		Wire.endTransmission();
		
		x+=8;
		j++;
	}
	
}

void OLED_SSD1306::print(unsigned char x, unsigned char y, String ch)
{
	Char_F6x8(x,y,"                     ");
	Char_F6x8(x,y,ch);
}

void OLED_SSD1306::print(unsigned char x, unsigned char y, String ch,unsigned char clr_len)
{
	String clr;
	for(unsigned char i=0;i<clr_len;i++)
		clr += ' ';
	Char_F6x8(x,y,clr);
	Char_F6x8(x,y,ch);
}
