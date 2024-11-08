#pragma once

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <vector>
#include "Subject.h"

using namespace std;


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
typedef enum { OKEY, INEXISTENT_USERNAME, NO_TWEETS,  PRIVATE_ACCOUNT, COMMUNICATION_ERROR, OTHER_ERROR } ERROR_STATE;
typedef enum {NO_DOWNLOADING,CONFIGURATION, DOWNLOADING, FINISHED, LCD_ , MORE } STATE; 

/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class Model : public Subject
{
public:
	Model();
	~Model();
	void nextTweet(void);
	void repeatTweet(void);
	void prevTweet(void);
	void addTweet(string tweet);
	void addDate(string date);
	void clearTweets(void);
	void clearDates(void);

	string getCurr(void);
	string getDate(void);
	string getUsername(void);
	ERROR_STATE getError(void);
	STATE getState(void);
	unsigned getQuantity(void);
	unsigned getSpeed(void);
	size_t getTwpos(void);
	bool getRefresh(void);
	bool getEnd(void);

	void setQuantity(unsigned quan);
	void setError(ERROR_STATE err);
	void setState(STATE state);
	void setSpeed(unsigned speed);
	void setUsername(string username);
	void setCurr(int curr);
	void setDate(int date);
	void setTwpos(size_t twpos);
	void setRefresh(bool refresh);
	void setEnd(bool end);


	unsigned progress;

private:
	
	vector<string> tweets;
	vector<string> dates;
	bool end;
	size_t twpos;
	STATE state;
	ERROR_STATE error;
	string username;
	int curr;	//"puntero" al tweet actual
	int date;
	unsigned speed;
	unsigned quantity;
	bool refresh;
};