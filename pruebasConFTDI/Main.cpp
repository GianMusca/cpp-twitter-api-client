/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Model.h"
#include "LCD.h"
#include "ImGuiClass.h"
#include "APITwitter.h"
#include "Refresh.h"

int main()
{
	Model m;
	IMGUI ui(m);
	APITwitter api(m);
	Refresh refresh;
	LCD lcd;
	m.attach(ui);
	m.attach(lcd);
	m.attach(api);
	m.attach(refresh);
	while (!m.getEnd())
	{
		ui.cycle(&m);
		api.cycle(&m);
		lcd.cycle(&m);
		refresh.cycle(&m);
	}
}