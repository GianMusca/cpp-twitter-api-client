/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "HitachiHD44780.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_CADD 32
#define MIN_CADD 1

#define MAX_ROW 1
#define MIN_ROW 0

#define MAX_COL 15
#define MIN_COL 0

#define MAX_CHAR 0x7F


 /*******************************************************************************
  * CLASS METHODS DEFINITIONS
  ******************************************************************************/

Hitachi::Hitachi()
	:basicLCD()
{
	ftHandle = lcdInit(MY_LCD_DESCRIPTION, status);
	cadd = 1;	//inicializa el cursor
}

Hitachi::~Hitachi()
{
	lcdDestroy(ftHandle);
}

bool Hitachi::lcdInitOk() 
{
	if (ftHandle != nullptr)
	{
		return true;
	}

	else 
	{
		return false;
	}
}

FT_STATUS Hitachi::lcdGetError()
{
	return status;
}

bool Hitachi::lcdClear()
{
	if (lcdWriteIR(*ftHandle, LCD_CLR))
	{
		cadd = MIN_CADD;
		return true;
	}
	else
	{
		return false;
	}

}

bool Hitachi::lcdClearToEOL()
{
	bool ret = true;
	int temp = cadd;

	while (cadd <= MAX_CADD) 
	{
		if (lcdWriteDR(*ftHandle, ' ')) 
		{
			ret = false;
		}

		cadd++;
		lcdUpdateCursor();
	}
	cadd = temp;
	lcdUpdateCursor();
	return ret;
}

basicLCD& Hitachi::operator<<(const unsigned char c)
{
	if (c >= ' ' || c <= MAX_CHAR) 
	{
		lcdWriteDR(*ftHandle, c);
		cadd = cadd + 1;

		if (cadd == MAX_CADD + 1) 
		{
			cadd = MIN_CADD;
		}
		lcdUpdateCursor();
	}
	return *this;

}

basicLCD& Hitachi::operator<<(const char *c)
{
	int i;
	for (i = 0; *(c + i) != NULL; i++) 
	{
		if (*(c + i) >= ' ' || *(c + i) <= MAX_CHAR) 
		{
			lcdWriteDR(*ftHandle, *(c + i));
			cadd = cadd + 1;

			if (cadd == MAX_CADD + 2)
			{
				cadd = MIN_CADD;
				lcdClear();
			}
			lcdUpdateCursor();
		}

	}
	return *this;
}

bool Hitachi::lcdMoveCursorUp()
{
	if (cadd <= (MAX_CADD/2) || cadd > MAX_CADD)
	{
		return false;
	}
	else
	{
		cadd = cadd - (MAX_CADD/2);
		lcdUpdateCursor();

		return true;
	}
}

bool Hitachi::lcdMoveCursorDown()
{
	if (cadd < MIN_CADD || cadd > (MAX_CADD/2))
	{
		return false;
	}
	else
	{
		cadd = cadd + (MAX_CADD/2);
		lcdUpdateCursor();

		return true;
	}
}

bool Hitachi::lcdMoveCursorRight()
{
	if (cadd < MIN_CADD || cadd >= MAX_CADD)
	{
		return false;
	}
	else
	{
		cadd = cadd + 1;
		lcdUpdateCursor();

		return true;
	}
}

bool Hitachi::lcdMoveCursorLeft()
{
	if (cadd <= MIN_CADD || cadd > MAX_CADD)
	{
		return false;
	}
	else
	{
		cadd = cadd - 1;
		lcdUpdateCursor();

		return true;
	}
}

bool Hitachi::lcdSetCursorPosition(const cursorPosition pos)
{
	if (pos.row < MIN_ROW || pos.row > MAX_ROW || pos.column < MIN_COL || pos.column > MAX_COL)
	{
		return false;
	}

	else 
	{
		cadd = (pos.row * (MAX_CADD/2)) + (pos.column + 1);
		lcdUpdateCursor();
		return true;
	}
}

cursorPosition Hitachi::lcdGetCursorPosition()
{
	cursorPosition pos;
	pos.row = (cadd/(MAX_CADD/2));
	pos.column = (cadd-1) - (MAX_CADD/2)*pos.row ;

	return pos;
}

void Hitachi::lcdUpdateCursor()
{
	int cursor = cadd - 1;
	if (cursor < (MAX_CADD/2)) {
		lcdWriteIR(*ftHandle, cursor + D7);
	}
	else {
		lcdWriteIR(*ftHandle, cursor + D6 + D7 - D4);
	}
	return;
}