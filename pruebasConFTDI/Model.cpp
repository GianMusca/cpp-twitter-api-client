/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Model.h"
#include <fstream>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CURL_STATICLIB


/*******************************************************************************
 * CASS METHODS DEFINITIONS
 ******************************************************************************/

Model::Model(void)
{
	username.clear();
	twpos = 0;
	curr = 0;
	date = 0;
	quantity = 0;
	progress = 0;
	speed = 25;
	error = OKEY;
	state = NO_DOWNLOADING;
	end = false;
	refresh = false;
}


Model::~Model(void)
{

}

void Model::nextTweet(void)
{
	if (curr < (tweets.size()-1) && !tweets.empty())
	{
		curr++;
		date++;
		twpos = 0;
		notifyAllObservers();
	}
	else if (curr == (tweets.size()-1))
	{
		curr = 0;
		date = 0;
		twpos = 0;
		notifyAllObservers();
	}
}

void Model::repeatTweet(void)
{
	twpos = 0;
	notifyAllObservers();
}

void Model:: prevTweet(void)
{
	if (curr != 0 && !tweets.empty())
	{
		curr--;
		date--;
		twpos = 0;
		notifyAllObservers();
	}
	else if (curr == 0 && !tweets.empty())
	{
		curr = tweets.size()-1;
		date = dates.size()-1;
		twpos = 0;
		notifyAllObservers();
	}
}

void Model::addTweet(string tweet) { tweets.push_back(tweet); }
void Model::addDate(string date) { dates.push_back(date); }
void Model::clearTweets(void) { tweets.clear(); }
void Model::clearDates(void) { dates.clear(); }

string Model::getCurr(void) { return tweets[curr]; }
string Model::getDate(void) { return dates[date]; }
string Model::getUsername(void) { return username; }
ERROR_STATE Model::getError(void) { return error; }
STATE Model::getState(void) { return state; }
unsigned Model::getQuantity(void) { return quantity; }
unsigned Model::getSpeed(void) { return speed; }
size_t Model::getTwpos(void) { return twpos; }
bool Model::getRefresh(void) { return refresh; }
bool Model::getEnd(void) { return end; }

void Model::setQuantity(unsigned quantity) { this->quantity = quantity; }
void Model::setEnd(bool end) { this->end = end; }
void Model::setUsername(string username){ this->username = username; }
void Model::setCurr(int curr) { this->curr = curr; }
void Model::setDate(int date) { this->date = date; }
void Model::setTwpos(size_t twpos) { this->twpos = twpos; }
void Model::setSpeed(unsigned speed) 
{ 
	this->speed = speed; 
	notifyAllObservers();

}
void Model::setError(ERROR_STATE error) 
{ 
	this->error = error; 
	notifyAllObservers();
}
void Model::setState(STATE state) 
{ 
	this->state = state;
	notifyAllObservers();
}
void Model:: setRefresh(bool refresh) 
{ 
	this->refresh = refresh;
	notifyAllObservers();
}


