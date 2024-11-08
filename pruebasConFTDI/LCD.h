#pragma once

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "HitachiHD44780.h"
#include "BasicLCD.h"
#include "Model.h"
#include "Observer.h"



/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class LCD : public Observer
{
public:
	LCD();
	~LCD();
	void update(void* model) override;
	void cycle(void* model) override;

private:

	basicLCD* lcd;
	string adaptTweet(string tweet);
	void writeTweet(string tweet, bool refresh, size_t pos);
	void writeDate(string datIt);
	void showProgress(int progress);
	void showError(ERROR_STATE err);
	void welcomeMessage(void);
	void exitMessage(void);

	string dateParse(string date);
	string tweet;

};
