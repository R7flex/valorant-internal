#include "zerogui.h"

using namespace valorant;

sdk::structs::fvector2d pos = { 1920 / 2, 1080 / 2 };

namespace menu
{
	void render_menu(sdk::uobject* font, sdk::ucanvas* canvas)
	{
		menu::setup_canvas(canvas, font);
		menu::input::handle();

		static bool menu_opened = false;
		if (GetAsyncKeyState(VK_F2) & 1) menu_opened = !menu_opened;

		if (menu::window((char*)"valorant-internal", &pos, sdk::structs::fvector2d{ 500.0f, 400.0f }, menu_opened))
		{
			static int tab = 0;
			if (menu::buttontab((char*)"visuals", sdk::structs::fvector2d{ 110, 25 }, tab == 0)) tab = 0;

			if (tab == 0)
			{

			}
		}
		menu::render();
	}
}