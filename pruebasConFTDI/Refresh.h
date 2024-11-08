#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Observer.h"

/*******************************************************************************
 * CLASS PROTOTYPE
 ******************************************************************************/
class Refresh :public Observer
{
public:
	
	Refresh();
	~Refresh();
	void update(void*);
	void cycle(void*);

private:
	int clock;
	int currSpeed;

};