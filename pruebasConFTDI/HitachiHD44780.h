#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "BasicLCD.h"


/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class Hitachi : public basicLCD {
public:

	Hitachi();
	~Hitachi();

	bool lcdInitOk() override;
	FT_STATUS lcdGetError() override;

	bool lcdClear() override;
	bool lcdClearToEOL() override;

	basicLCD& operator<<(const unsigned char c) override;
	basicLCD& operator<<(const char *c) override;

	bool lcdMoveCursorUp() override;
	bool lcdMoveCursorDown() override;
	bool lcdMoveCursorRight() override;
	bool lcdMoveCursorLeft() override;

	bool lcdSetCursorPosition(const cursorPosition) override;

	cursorPosition lcdGetCursorPosition() override;

private:

	FT_HANDLE* ftHandle;
	FT_STATUS status;
	void lcdUpdateCursor() override;
};


