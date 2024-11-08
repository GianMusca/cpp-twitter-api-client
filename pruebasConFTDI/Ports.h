#ifndef PORTS_H
#define PORTS_H

//PORTS
#define D7 (1<<7)
#define D6 (1<<6)
#define D5 (1<<5)
#define D4 (1<<4)
#define D3 (1<<3)
#define D2 (1<<2)
#define D1 (1<<1)
#define D0 (1<<0)

//MASKS
#define HIGH(B)		((B)&0xF0)
#define LOW(B)		((B)&0x0F)

#define LCD_RS	D1
#define LCD_E	D0

#define LCD_IR (LCD_RS^LCD_RS)  
#define LCD_DR LCD_RS

#define LCD_E_ON LCD_E
#define LCD_E_OFF (LCD_E^LCD_E)

//CLEAR
#define LCD_CLR	D0

//RETURN HOME
#define LCD_RTN	D1

//ENTRY MODE SET
#define INCREMENT	D1
#define DECREMENT	(!D1)
#define S_ON		D0
#define S_OFF		(!D0)
#define LCD_EMS		D2

//DISPLAY ON/OFF CONTROL
#define D_ON	D2
#define C_ON	D1
#define B_ON	D0
#define D_OFF	(!D2)
#define C_OFF	(!D1)
#define B_OFF	(!D0)
#define LCD_DC	D3

//DDRAM address
#define LCD_DDRAM	D7


//FUNCTION SET
#define N_			D3
#define TWO_LINES   N_
#define ONE_LINE    (!N_)

#define DL			D4
#define FOUR_BITS   (!DL)
#define EIGHT_BITS	DL

#define F_			D2
#define F5X10		F_
#define F5X8		(!F_)

#define LCD_FSET	D5

#endif // !PORTS_H



