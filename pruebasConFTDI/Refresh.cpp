/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <chrono>
#include <thread>
#include "Refresh.h"
#include "Model.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define HYPHENS 2
#define SPACE 15


 /*******************************************************************************
  * CLASS METHODS DEFINITIONS
  ******************************************************************************/
Refresh::Refresh(void)
{
	clock = 0;
	currSpeed = 25;
}

Refresh::~Refresh(void) 
{

}

void Refresh::update(void * model) 
{
	Model* pm = (Model*)model;
	if (pm->getSpeed() != currSpeed)
	{
		clock = 0;
		currSpeed = pm->getSpeed();
	}
	
}


void Refresh::cycle(void* model)
{
	Model* pm = (Model*)model;
	if (pm->getState() != LCD_ || pm->getError() != OKEY)
	{
		return;
	}

	clock++;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	if (pm->getSpeed() == 0)
	{
		pm->setRefresh(false);
		clock = 0;
	}
	else if ( clock == (101 - pm->getSpeed()) )
	{
		pm->setRefresh(true);
		pm->setTwpos(pm->getTwpos() + 1);
		clock = 0;
	}
	else
	{
		pm->setRefresh(false);
	}

	if ((pm->getCurr().size() + HYPHENS + SPACE) == pm->getTwpos())
	{
		pm->nextTweet();
	}

	



}