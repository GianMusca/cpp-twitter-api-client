#pragma once

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Model.h"
#include "Observer.h"
#include "json.hpp"
#include <curl.h>

using namespace std;
using json = nlohmann::json;


/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class APITwitter : public Observer
{

public:

	APITwitter(Model& m);
	~APITwitter();
	void update(void*) override;
	void cycle(void*) override;

private:

	CURL* curl;		//Easy handle
	CURLM* curlm;   //Multi handle
	json file;		//Para parsear json
	string aux;
	int running;

	bool downloadTweets(void);
	void moreTweets(Model& m);
	void getTweets(Model& m);	//guarda los tweets en el vector tweets  
	void connectToTwitter(Model& m);
	void curlmConfiguration(Model& m);
};