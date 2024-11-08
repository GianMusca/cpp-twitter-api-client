/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Drivers.h"

/*******************************************************************************
*	NAMESPACES
******************************************************************************/
using namespace std;

/*******************************************************************************
*	CONSTANT AND MACRO DEFINITIOS
******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//Escribe un nybble
static bool lcdWriteNibble(FT_HANDLE deviceHandler, BYTE value, DWORD ms = 1);

//Escribe un byte 
static bool lcdWriteByte(FT_HANDLE deviceHandler, BYTE value, BYTE rs, DWORD ms = 1);

/*******************************************************************************
 * GLOBAL FUNCTIONS DEFINITION
 ******************************************************************************/
FT_HANDLE* lcdInit(char * description, FT_STATUS& status_)
{
	FT_HANDLE* ret = nullptr;
	UCHAR Mask = 0xFF;	//Selects all FTDI pins.
	UCHAR Mode = 1; 	// Set asynchronous bit-bang mode

	std::chrono::seconds MaxTime(CONNECTING_TIME);/*The display has a settling time after the physical connection so the attempt to connect
	will be done for a few seconds*/

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> current = start;

	FT_HANDLE* lcdHandle = new FT_HANDLE;
	FT_STATUS status = !FT_OK;
	while (status != FT_OK && ((current - start) < MaxTime))//loop till succesful connection o max connecting time is exceeded
	{
		//status = FT_Open(iDevice, lcdHandle);
		status = FT_OpenEx((void*)description, FT_OPEN_BY_DESCRIPTION, lcdHandle);
		status_ = status;
	}

	if (status != FT_OK)
	{
		cout << "No se pudo abrir el LCD" << endl;
	}
	else if (FT_SetBitMode(*lcdHandle, Mask, Mode) != FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
	{
		cout << "No se pudo configurar el LCD" << endl;
	}
	else
	{
		lcdWriteNibble(*lcdHandle, HIGH(LCD_FSET | EIGHT_BITS) | LCD_IR, 5);
		lcdWriteNibble(*lcdHandle, HIGH(LCD_FSET | EIGHT_BITS) | LCD_IR, 1);
		lcdWriteNibble(*lcdHandle, HIGH(LCD_FSET | EIGHT_BITS) | LCD_IR, 1);
		lcdWriteNibble(*lcdHandle, HIGH(LCD_FSET | FOUR_BITS) | LCD_IR, 1);
		lcdWriteIR(*lcdHandle, LCD_FSET | FOUR_BITS | TWO_LINES | F5X8, 1);
		lcdWriteIR(*lcdHandle, LCD_DC | B_OFF | C_ON | D_ON, 1);
		lcdWriteIR(*lcdHandle, LCD_CLR, 10);
		lcdWriteIR(*lcdHandle, LCD_EMS | INCREMENT | S_OFF);
		ret = lcdHandle;
	}
	return ret;
}

bool lcdWriteIR(FT_HANDLE deviceHandler, BYTE value, DWORD ms)
{
	return lcdWriteByte(deviceHandler, value, LCD_IR, ms);
}

bool lcdWriteDR(FT_HANDLE deviceHandler, BYTE value, DWORD ms)
{
	return lcdWriteByte(deviceHandler, value, LCD_DR, ms);
}


void lcdDestroy(FT_HANDLE* ftHandle)
{
	if (ftHandle != nullptr)
	{
		FT_Close(ftHandle);
		delete ftHandle;
	}
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static bool lcdWriteNibble(FT_HANDLE deviceHandler, BYTE value, DWORD ms)
{
	BYTE enable = LCD_E_ON | value;
	BYTE disable = LCD_E_OFF | value;
	DWORD bytesWritten = 0;
	bool ret = false;

	if (FT_Write(deviceHandler, &disable, 1, &bytesWritten) != FT_OK)
	{
		bytesWritten = 0;
		DWORD lastError = FT_W32_GetLastError(deviceHandler);
	}
	else if (FT_Write(deviceHandler, &enable, 1, &bytesWritten) != FT_OK)
	{
		bytesWritten = 0;
	}
	else
	{

			//std::this_thread::sleep_for(chrono::milliseconds(ms));
			std::this_thread::sleep_for(chrono::microseconds(ms*10));

		if (FT_Write(deviceHandler, &disable, 1, &bytesWritten) != FT_OK)
		{
			cout << "Not disable!" << endl;
		}
		ret = true;
	}
	return ret;
}

static bool lcdWriteByte(FT_HANDLE deviceHandler, BYTE value, BYTE rs, DWORD ms)
{
	BYTE nybbleH = HIGH(value) | rs;
	BYTE nybbleL = (LOW(value) << 4) | rs;

	if (lcdWriteNibble(deviceHandler, nybbleH, ms) && lcdWriteNibble(deviceHandler, nybbleL, ms))
	{
		return true;
	}
	else
	{
		return false;
	}
}