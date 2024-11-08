/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "APITwitter.h"
#include <fstream>


/*******************************************************************************
 * LOCAL FUNCTIONS DECLARATIONS
 ******************************************************************************/
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);

static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);


/*******************************************************************************
 * CLASS METHODS DEFINITIONS
 ******************************************************************************/
APITwitter::APITwitter(Model& m)
{
	curl = curl_easy_init();
	curlm = curl_multi_init();
	aux.clear();
	file.clear();
	running = 0;
	connectToTwitter(m);
}

APITwitter::~APITwitter()
{
	curl_multi_remove_handle(curlm, curl);
	curl_easy_cleanup(curl);
	curl_multi_cleanup(curlm);
}

void APITwitter::update(void* model) 
{
	Model* pm = (Model*)model;
	if (pm->getState() == CONFIGURATION)
	{
		curlmConfiguration(*pm);
		pm->setState(DOWNLOADING);
	}

	if (pm->getState() == MORE)
	{
		moreTweets(*pm);
		pm->setState(DOWNLOADING);
	}
}


void APITwitter::cycle(void* model)
{
	Model* pm = (Model*)model;
	if (pm->getState() == DOWNLOADING)
	{
		if (!downloadTweets())
		{
			pm->setState(FINISHED);
		}
	}

	if (pm->getState() == FINISHED)
	{
		getTweets(*pm);
		pm->setState(LCD_);
		
	}

}



bool APITwitter::downloadTweets()
{
	bool ret = true;
	CURLMcode res;
	res = curl_multi_perform(curlm, &running);
	if (!running)
	{
		ret = false;
	}

	return ret;
}

void APITwitter::getTweets(Model& m)
{
	m.clearTweets();
	//m.clearDates();
	m.setCurr(0);
	m.setDate(0);
	m.setTwpos(0);

	ofstream o("tweets.json");
	o << aux;
	o.close();

	try {

		ifstream i("tweets.json");
		i >> file;
		i.close();
	}

	catch (exception& e)
	{
		if (!aux.empty())
		{
			json::iterator it = file.end();
			it--;
			file.erase(it);
		}

		else
		{
			m.setError(COMMUNICATION_ERROR);
		}
	}

	if (!file.empty() && file.is_array())
	{
		for (auto& element : file)
		{
			auto twt = element["text"];
			auto date = element["created_at"];

			m.addTweet(twt);
			m.addDate(date);
		}

		m.setError(OKEY);
	}

	else if (file.empty())
	{
		m.setError(NO_TWEETS);
	}

	else
	{
		for (json::iterator it = file.begin(); it != file.end(); ++it)
		{
			if (it.key() == "errors")
			{
				m.setError(INEXISTENT_USERNAME);
			}

			else if (it.key() == "error")
			{
				m.setError(PRIVATE_ACCOUNT);
			}
		}
	}
}

void APITwitter::moreTweets(Model& m)
{
	//Reseteo todo;
	aux.clear();
	file.clear();
	curl_multi_remove_handle(curlm, curl);

	//URL, cambio cantidad
	string path = "https://api.twitter.com/1.1/statuses/user_timeline.json?";
	path += "screen_name=" + m.getUsername() + "&count=" + to_string(m.getQuantity());

	curl_easy_setopt(curl, CURLOPT_URL, path.c_str());
	curl_multi_add_handle(curlm, curl);

}


void APITwitter::connectToTwitter(Model& m)
{
	m.setError(COMMUNICATION_ERROR);

	CURLcode res;

	if (curl)
	{
		//Le decimos a cURL que imprima todos los mensajes que se envían entre cliente y servidor.
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//Seteamos primero la pagina donde nos vamos a conectar.
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Le decimos a cURL que trabaje con HTTP.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS | CURLPROTO_HTTP);

		//Le decimos a cURL que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &aux);

		//Seteo para seguir el redireccionamiento
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//Seteo el metodo de autenticacion
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Seteo la key y secretkey
		curl_easy_setopt(curl, CURLOPT_USERPWD, "VaRwi4BgCSJwetlaNLmAx7UgA:B2zdNXM71k0gxhUPOiDRCbVJeRhJN2VrlowVnxAa9hOIYwfbIP");

		//Incluye header "Content-type"
		struct curl_slist* header = NULL;
		header = curl_slist_append(header, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

		//Incluye "grant_type"
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 29L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");

		//Kcyo estaba en internet
		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			//Getting acces token
			file = json::parse(aux); //De string a JSON
			aux.clear();
			auto tkn = file["access_token"]; //Busco token
			aux = tkn.get<string>(); //De JSON a string

			m.setError(OKEY);
		}

		curl_slist_free_all(header);
	}

	file.clear();
}


void APITwitter::curlmConfiguration(Model& m)
{
	//Resetea el handler
	curl_multi_remove_handle(curlm, curl);
	curl_easy_reset(curl);

	//URL
	string path = "https://api.twitter.com/1.1/statuses/user_timeline.json?";
	path += "screen_name=" + m.getUsername() + "&count=" + to_string(m.getQuantity());

	//Aces token
	string auth = "Authorization: Bearer " + aux;

	if (curlm && curl)
	{
		//Le decimos a cURL que imprima todos los mensajes que se envían entre cliente y servidor.
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//Seteamos primero la pagina donde nos vamos a conectar.
		curl_easy_setopt(curl, CURLOPT_URL, path.c_str());

		// Le decimos a cURL que trabaje con HTTP.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS | CURLPROTO_HTTP);

		//Le decimos a cURL que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &aux);

		//Seteo para seguir el redireccionamiento
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//Seteo el metodo de autenticacion
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Incluye header "Authorization"
		struct curl_slist* header = NULL;
		header = curl_slist_append(header, auth.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

		//Para poder ver el progreso
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &(m.progress));
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);

		//Kcyo estaba en internet
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		//Atacheo el easy_handle
		curl_multi_add_handle(curlm, curl);

	}
	
	else 
	{
		m.setError(COMMUNICATION_ERROR);
	}

	aux.clear();
	file.clear();
}



/*******************************************************************************
 * LOCAL FUNCTIONS DEFINITIONS
 ******************************************************************************/

 //Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* response = (char*)contents;
	string* data = (string*)userp;
	data->append(response, size * nmemb);

	//cout << data << endl;

	return realsize;						//Recordar siempre devolver realsize
}


//Da un porcentaje del prgreso de descarga
static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	unsigned int* count = (unsigned int*)clientp;
	(*count) = (unsigned int)((dlnow * 100) / dltotal);

	//cout << *count << endl;

	return 0;
}