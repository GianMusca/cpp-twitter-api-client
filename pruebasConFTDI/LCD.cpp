/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "LCD.h"
#include <string>
#include "HitachiHD44780.h"

using namespace std;


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DATEPOS 8
#define MONTHPOS 4
#define YEARPOS 28
#define TIMEPOS 11
#define ENE 281
#define FEB 269
#define MAR 288
#define ABR 291
#define MAY 295
#define JUN 301
#define JUL 299
#define AGO 285
#define SEP 296
#define OCT 294
#define NOV 307
#define DIC 268


/*******************************************************************************
 * CLASS METHODS DEFINITIONS
 ******************************************************************************/

LCD::LCD()
{
	lcd = new Hitachi;
	lcd->lcdClear();

	welcomeMessage();
}

LCD::~LCD()
{
	exitMessage();
	delete lcd;
}

void LCD:: update(void* model)
{
	Model*pm = (Model*)model;

	if (pm->getError() != OKEY)
	{
		showError(pm->getError());
	}

	else if (pm->getState() == LCD_ && pm->getError() == OKEY && pm->getRefresh())
	{
		if (pm->getTwpos() == 0) 
		{
			tweet = adaptTweet(pm->getCurr());
		}
		writeDate(pm->getDate());
		writeTweet(tweet, pm->getRefresh(), pm->getTwpos());
	}

}

void LCD::cycle(void* model)
{
	Model* pm = (Model*)model;
	if (pm->getState() == DOWNLOADING)
	{
		showProgress((16.0 / 100.0) * pm->progress);
	}
}

void LCD:: showProgress(int progress)
{
	if (progress == 0)
	{
		lcd->lcdClear();
		lcd->lcdSetCursorPosition({ 0, 2 });
		*lcd << "Downloading";
	}

	for (int i = 0; i < progress; i++)
	{
		lcd->lcdSetCursorPosition({ 1,i });
		*lcd << '*';
	}
}


/*Recibo el tweet tomado del json en formato string.
Pongo el display en la segunda fila y escribo 16 caracteres del tweet usando un sub-string
Ademas agrego espacios al tweet para que sea mas lindo de ver, y los guiones*/
void LCD:: writeTweet(string tweet, bool refresh, size_t pos) 
{
	cursorPosition cursPos = { 1,0 };
	string space = "               ";
	string text = space + '-' + tweet + '-' + space;
		
	string lcdview(text, pos, 16);
	lcd->lcdSetCursorPosition(cursPos);
	if (refresh)
	{
		*lcd << lcdview.c_str();
	}
}


/*escribo la fecha en la primer fila del display, recibo un string en el que se encuentra el json de twitter
cuyo formato es "NNN MMM DD TTTTTNNN NNNNN NNYY" donde:
N= dato no necesario, pero tomado en cuenta para la posicion
D= dia del tweet
M= mes del tweet
Y= año del tweet
T= hora del tweet
*/
void LCD:: writeDate(string date)
{
	cursorPosition cursPos = { 0,0 };
	string parsedDate = dateParse(date);
	lcd->lcdSetCursorPosition(cursPos);
	*lcd << parsedDate.c_str();
}


/*Parser de la fecha, recibe la fecha en formato "NNN MMM DD TTTTTNNN NNNNN NNYY" como antes.
Busca cada elemento por separado y luego los une en el formato " DD/MM/AA – hh:mm" */
string LCD:: dateParse(string date) 
{
	string day(date, DATEPOS, 2);
	string month(date, MONTHPOS, 3);

	/*Busco el mes sumando las letras en formato ascii*/
	switch ((unsigned)month[0] + (unsigned)month[1] + (unsigned)month[2]) {
	case ENE:
		month = "01";
		break;
	case FEB:
		month = "02";
		break;
	case MAR:
		month = "03";
		break;
	case ABR:
		month = "04";
		break;
	case MAY:
		month = "05";
		break;
	case JUN:
		month = "06";
		break;
	case JUL:
		month = "07";
		break;
	case AGO:
		month = "08";
		break;
	case SEP:
		month = "09";
		break;
	case OCT:
		month = "10";
		break;
	case NOV:
		month = "11";
		break;
	case DIC:
		month = "12";
		break;
	}

	string year(date, YEARPOS, 2);
	string time(date, TIMEPOS, 5);
	string parse = day + '/' + month + '/' + year + " - " + time;
	return parse;
}

void LCD::showError(ERROR_STATE err)
{
	static ERROR_STATE lasterr = OKEY;
	if (lasterr != err)
	{
		lcd->lcdClear();			//Si no hago esto me hace el clear todo el tiempo
		lasterr = err;				//Y queda feo
	}

	lcd->lcdSetCursorPosition({ 0,0 });
	string error;

	switch (err)
	{
	case INEXISTENT_USERNAME:
		error = " Error usuario    inexistente";
		break;

	case NO_TWEETS:
		error = " Error usuario     sin tweets";
		break;

	case PRIVATE_ACCOUNT:
		error = "  Error usuario     privado";
		break;

	case COMMUNICATION_ERROR:
		error = "  Error de      comunicacion";
		break;

	default:
		error = "Error desconocido";
		break;
	}

	*lcd << error.c_str();


}

void LCD::welcomeMessage()
{
	lcd->lcdSetCursorPosition({ 0,4 });
	*lcd << "WELCOME!";
	lcd->lcdSetCursorPosition({ 1,0 });
	*lcd << "EDA TP9 TWITTER";
}

void LCD::exitMessage()
{
	lcd->lcdClear();
	lcd->lcdSetCursorPosition({ 0,0 });
	*lcd << "GOODBYE!";
	lcd->lcdSetCursorPosition({ 1,9 });
	*lcd << "AGUSTIN";
}


string LCD::adaptTweet(string tweet) 
{
	size_t pos = 0;

	while (pos != string::npos)
	{
		if ((pos = tweet.find('Ã', pos ? (pos + 1) : pos)) != string::npos)
		{
			switch (tweet[pos+1])
			{
				case '‘' :
				tweet.replace(pos, 1, 1, 'N');
				tweet.erase(pos + 1, 1);
				break;

				case '±' :
				tweet.replace(pos, 1, 1, 'n');
				tweet.erase(pos + 1, 1);
				break;

				case '' :
				tweet.replace(pos, 1, 1, 'A');
				tweet.erase(pos + 1, 1);
				break;

				case '¡' :
				tweet.replace(pos, 1, 1, 'a');
				tweet.erase(pos + 1, 1);
				break;
				
				case '‰' :
				tweet.replace(pos, 1, 1, 'E');
				tweet.erase(pos + 1, 1);
				break;
				
				case '©' : 
				tweet.replace(pos, 1, 1, 'e');
				tweet.erase(pos + 1, 1);
				break;
			
				case '' :
				tweet.replace(pos, 1, 1, 'I');
				tweet.erase(pos + 1, 1);
				break;
				
				case '­' :
				tweet.replace(pos, 1, 1, 'i');
				tweet.erase(pos + 1, 1);
				break;
				
				case '“' :
				tweet.replace(pos, 1, 1, 'O');
				tweet.erase(pos + 1, 1);
				break;
				
				case '³' :
				tweet.replace(pos, 1, 1, 'o');
				tweet.erase(pos + 1, 1);
				break;
				
				case 'š' :
				tweet.replace(pos, 1, 1, 'U');
				tweet.erase(pos + 1, 1);
				break;
				
				case 'º' :
				tweet.replace(pos, 1, 1, 'u');
				tweet.erase(pos + 1, 1);
				break;

				default:
					break;
			}
		}

		else if ((pos = tweet.find("â€¦", pos ? (pos + 1) : pos)) != string::npos)
		{
			tweet.replace(pos, 3, 3, '.');
		}

	}

	return tweet;

}