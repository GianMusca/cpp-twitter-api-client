#pragma once

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <vector>
#include "imgui.h"
#include "imgui_impl_allegro5.h"
//
#include "imconfig.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "model.h"

using namespace std;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_LENGHT 1000


/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/

class IMGUI : public Observer 
{

public:

	IMGUI(Model& _m);
	~IMGUI();
	
	void update(void* model) override;
	void cycle(void* model) override;


private:
	ALLEGRO_DISPLAY* getDisplay();
	ALLEGRO_EVENT_QUEUE* getEvQueue();
	bool getError();
	void graphNewFrame();
	void resizeDisplay();
	void printScreen();
	bool initAllegro(void);
	
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	Model& m;
	bool error;
	bool searchButton;
	bool defaultCB;
	bool defaultSp;
	unsigned auxSpeed;
	unsigned viewSpeed;			//esto puede ir aca o en el view.
	string user;


};