#include "zeroinput.h"
#include "sdk.hpp"

wchar_t* s2wc(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

namespace menu
{
	namespace colors
	{
		valorant::sdk::structs::flinearcolor Text{ 1.0f, 1.0f, 1.0f, 1.0f };
		valorant::sdk::structs::flinearcolor Text_Shadow{ 0.0f, 0.0f, 0.0f, 0.0f };
		valorant::sdk::structs::flinearcolor Text_Outline{ 0.0f, 0.0f, 0.0f, 0.30f };

		valorant::sdk::structs::flinearcolor Window_Background{ 0.009f, 0.009f, 0.009f, 1.0f };
		valorant::sdk::structs::flinearcolor Window_Header{ 0.10f, 0.15f, 0.84f, 1.0f };

		valorant::sdk::structs::flinearcolor Button_Idle{ 0.10f, 0.15f, 0.84f, 1.0f };
		valorant::sdk::structs::flinearcolor Button_Hovered{ 0.15f, 0.20f, 0.89f, 1.0f };
		valorant::sdk::structs::flinearcolor Button_Active{ 0.20f, 0.25f, 0.94f, 1.0f };

		valorant::sdk::structs::flinearcolor Checkbox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		valorant::sdk::structs::flinearcolor Checkbox_Hovered{ 0.22f, 0.30f, 0.72f, 1.0f };
		valorant::sdk::structs::flinearcolor Checkbox_Enabled{ 0.20f, 0.25f, 0.94f, 1.0f };

		valorant::sdk::structs::flinearcolor Combobox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		valorant::sdk::structs::flinearcolor Combobox_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		valorant::sdk::structs::flinearcolor Combobox_Elements{ 0.239f, 0.42f, 0.82f, 1.0f };

		valorant::sdk::structs::flinearcolor Slider_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		valorant::sdk::structs::flinearcolor Slider_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		valorant::sdk::structs::flinearcolor Slider_Progress{ 0.22f, 0.30f, 0.72f, 1.0f };
		valorant::sdk::structs::flinearcolor Slider_Button{ 0.10f, 0.15f, 0.84f, 1.0f };

		valorant::sdk::structs::flinearcolor ColorPicker_Background{ 0.006f, 0.006f, 0.006f, 1.0f };
	}

	namespace postrenderer
	{
		struct DrawList
		{
			int type = -1; //1 = FilledRect, 2 = TextLeft, 3 = TextCenter, 4 = Draw_Line
			valorant::sdk::structs::fvector2d pos;
			valorant::sdk::structs::fvector2d size;
			valorant::sdk::structs::flinearcolor color;
			char* name;
			bool outline;

			valorant::sdk::structs::fvector2d from;
			valorant::sdk::structs::fvector2d to;
			int thickness;
		};
		DrawList drawlist[128];

		void drawFilledRect(valorant::sdk::structs::fvector2d pos, float w, float h, valorant::sdk::structs::flinearcolor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 1;
					drawlist[i].pos = pos;
					drawlist[i].size = valorant::sdk::structs::fvector2d{ w, h };
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextLeft(char* name, valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::flinearcolor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 2;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void TextCenter(char* name, valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::flinearcolor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 3;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		void Draw_Line(valorant::sdk::structs::fvector2d from, valorant::sdk::structs::fvector2d to, int thickness, valorant::sdk::structs::flinearcolor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 4;
					drawlist[i].from = from;
					drawlist[i].to = to;
					drawlist[i].thickness = thickness;
					drawlist[i].color = color;
					return;
				}
			}
		}
	}

	valorant::sdk::ucanvas* canvas;
	valorant::sdk::uobject* font;


	bool hover_element = false;
	valorant::sdk::structs::fvector2d menu_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	valorant::sdk::structs::fvector2d first_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };

	valorant::sdk::structs::fvector2d last_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
	valorant::sdk::structs::fvector2d last_element_size = valorant::sdk::structs::fvector2d{ 0, 0 };

	int current_element = -1;
	valorant::sdk::structs::fvector2d current_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
	valorant::sdk::structs::fvector2d current_element_size = valorant::sdk::structs::fvector2d{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;

	bool pushY = false;
	float pushYvalue = 0.0f;

	void setup_canvas(valorant::sdk::ucanvas* _canvas, valorant::sdk::uobject* _font)
	{
		canvas = _canvas;
		font = _font;
	}

	valorant::sdk::structs::fvector2d CursorPos()
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		return valorant::sdk::structs::fvector2d{ (float)cursorPos.x, (float)cursorPos.y };
	}
	bool MouseInZone(valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::fvector2d size)
	{
		valorant::sdk::structs::fvector2d cursor_pos = CursorPos();

		if (cursor_pos.x > pos.x && cursor_pos.y > pos.y)
			if (cursor_pos.x < pos.x + size.x && cursor_pos.y < pos.y + size.y)
				return true;

		return false;
	}

	void Draw_Cursor(bool toogle)
	{
		if (toogle)
		{
			valorant::sdk::structs::fvector2d cursorPos = CursorPos();
			canvas->k2_drawline(valorant::sdk::structs::fvector2d{ cursorPos.x, cursorPos.y }, valorant::sdk::structs::fvector2d{ cursorPos.x + 35, cursorPos.y + 10 }, 1, valorant::sdk::structs::flinearcolor{ 0.30f, 0.30f, 0.80f, 1.0f });


			int x = 35;
			int y = 10;
			while (y != 30) //20 steps
			{
				x -= 1; if (x < 15) x = 15;
				y += 1; if (y > 30) y = 30;

				canvas->k2_drawline(valorant::sdk::structs::fvector2d{ cursorPos.x, cursorPos.y }, valorant::sdk::structs::fvector2d{ cursorPos.x + x, cursorPos.y + y }, 1, valorant::sdk::structs::flinearcolor{ 0.30f, 0.30f, 0.80f, 1.0f });
			}

			canvas->k2_drawline(valorant::sdk::structs::fvector2d{ cursorPos.x, cursorPos.y }, valorant::sdk::structs::fvector2d{ cursorPos.x + 15, cursorPos.y + 30 }, 1, valorant::sdk::structs::flinearcolor{ 0.30f, 0.30f, 0.80f, 1.0f });
			canvas->k2_drawline(valorant::sdk::structs::fvector2d{ cursorPos.x + 35, cursorPos.y + 10 }, valorant::sdk::structs::fvector2d{ cursorPos.x + 15, cursorPos.y + 30 }, 1, valorant::sdk::structs::flinearcolor{ 0.30f, 0.30f, 0.80f, 1.0f });
		}
	}

	void SameLine()
	{
		sameLine = true;
	}
	void PushNextElementY(float y, bool from_last_element = true)
	{
		pushY = true;
		if (from_last_element)
			pushYvalue = last_element_pos.y + last_element_size.y + y;
		else
			pushYvalue = y;
	}
	void NextColumn(float x)
	{
		offset_x = x;
		PushNextElementY(first_element_pos.y, false);
	}
	void ClearFirstPos()
	{
		first_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
	}

	void TextLeft(char* name, valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::flinearcolor color, bool outline)
	{
		int length = strlen(name) + 1;
		canvas->k2_drawtext(font, valorant::sdk::structs::fstring{ s2wc(name), length }, pos, valorant::sdk::structs::fvector2d{ 0.97f, 0.97f }, color, false, colors::Text_Shadow, valorant::sdk::structs::fvector2d{ pos.x + 1, pos.y + 1 }, false, true, true, colors::Text_Outline);
	}
	void TextCenter(char* name, valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::flinearcolor color, bool outline)
	{
		int length = strlen(name) + 1;
		canvas->k2_drawtext(font, valorant::sdk::structs::fstring{ s2wc(name), length }, pos, valorant::sdk::structs::fvector2d{ 0.97f, 0.97f }, color, false, colors::Text_Shadow, valorant::sdk::structs::fvector2d{ pos.x + 1, pos.y + 1 }, true, true, true, colors::Text_Outline);
	}

	void GetColor(valorant::sdk::structs::flinearcolor* color, float* r, float* g, float* b, float* a)
	{
		*r = color->r;
		*g = color->g;
		*b = color->b;
		*a = color->a;
	}
	UINT32 GetColorUINT(int r, int g, int b, int a)
	{
		UINT32 result = (BYTE(a) << 24) + (BYTE(r) << 16) + (BYTE(g) << 8) + BYTE(b);
		return result;
	}

	void Draw_Line(valorant::sdk::structs::fvector2d from, valorant::sdk::structs::fvector2d to, int thickness, valorant::sdk::structs::flinearcolor color)
	{
		canvas->k2_drawline(valorant::sdk::structs::fvector2d{ from.x, from.y }, valorant::sdk::structs::fvector2d{ to.x, to.y }, thickness, color);
	}
	void drawFilledRect(valorant::sdk::structs::fvector2d initial_pos, float w, float h, valorant::sdk::structs::flinearcolor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			canvas->k2_drawline(valorant::sdk::structs::fvector2d{ initial_pos.x, initial_pos.y + i }, valorant::sdk::structs::fvector2d{ initial_pos.x + w, initial_pos.y + i }, 1.0f, color);
	}
	void DrawFilledCircle(valorant::sdk::structs::fvector2d pos, float r, valorant::sdk::structs::flinearcolor color)
	{
		float smooth = 0.07f;

		double PI = 3.14159265359;
		int size = (int)(2.0f * PI / smooth) + 1;

		float angle = 0.0f;
		int i = 0;

		for (; angle < 2 * PI; angle += smooth, i++)
		{
			Draw_Line(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ pos.x + cosf(angle) * r, pos.y + sinf(angle) * r }, 1.0f, color);
		}
	}
	void DrawCircle(valorant::sdk::structs::fvector2d pos, int radius, int numSides, valorant::sdk::structs::flinearcolor Color)
	{
		float PI = 3.1415927f;

		float Step = PI * 2.0 / numSides;
		int Count = 0;
		valorant::sdk::structs::fvector2d V[128];
		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = radius * cos(a) + pos.x;
			float Y1 = radius * sin(a) + pos.y;
			float X2 = radius * cos(a + Step) + pos.x;
			float Y2 = radius * sin(a + Step) + pos.y;
			V[Count].x = X1;
			V[Count].y = Y1;
			V[Count + 1].x = X2;
			V[Count + 1].y = Y2;
			//Draw_Line(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ X2, Y2 }, 1.0f, Color); // Points from Centre to ends of circle
			Draw_Line(valorant::sdk::structs::fvector2d{ V[Count].x, V[Count].y }, valorant::sdk::structs::fvector2d{ X2, Y2 }, 1.0f, Color);// Circle Around
		}
	}

	valorant::sdk::structs::fvector2d dragPos;
	bool window(char* name, valorant::sdk::structs::fvector2d* pos, valorant::sdk::structs::fvector2d size, bool isOpen)
	{
		elements_count = 0;
		
		if (!isOpen)
			return false;

		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos->x, pos->y }, size);

		//Drop last element
		if (current_element != -1 && !GetAsyncKeyState(0x1))
		{
			current_element = -1;
		}

		//Drag
		if (hover_element && GetAsyncKeyState(0x1))
		{

		}
		else if ((isHovered || dragPos.x != 0) && !hover_element)
		{
			if (input::is_mouse_clicked(0, elements_count, true))
			{
				valorant::sdk::structs::fvector2d cursorPos = CursorPos();

				cursorPos.x -= size.x;
				cursorPos.y -= size.y;

				if (dragPos.x == 0)
				{
					dragPos.x = (cursorPos.x - pos->x);
					dragPos.y = (cursorPos.y - pos->y);
				}
				pos->x = cursorPos.x - dragPos.x;
				pos->y = cursorPos.y - dragPos.y;
			}
			else
			{
				dragPos = valorant::sdk::structs::fvector2d{ 0, 0 };
			}
		}
		else
		{
			hover_element = false;
		}


		offset_x = 0.0f; offset_y = 0.0f;
		menu_pos = valorant::sdk::structs::fvector2d{ pos->x, pos->y };
		first_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
		current_element_pos = valorant::sdk::structs::fvector2d{ 0, 0 };
		current_element_size = valorant::sdk::structs::fvector2d{ 0, 0 };

		//Bg
		drawFilledRect(valorant::sdk::structs::fvector2d{ pos->x, pos->y }, size.x, size.y, colors::Window_Background);
		//drawFilledRect(valorant::sdk::structs::fvector2d{ pos->x, pos->y }, 122, size.y, valorant::sdk::structs::flinearcolor{ 0.006f, 0.006f, 0.006f, 1.0f });//My tabs bg

		//Header
		drawFilledRect(valorant::sdk::structs::fvector2d{ pos->x, pos->y }, size.x, 25.0f, colors::Window_Header);

		offset_y += 25.0f;

		//Title
		valorant::sdk::structs::fvector2d titlePos = valorant::sdk::structs::fvector2d{ pos->x + size.x / 2, pos->y + 25 / 2 };
		TextCenter(name, titlePos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

		return true;
	}

	void Text(char* text, bool center = false, bool outline = false)
	{
		elements_count++;

		float size = 25;
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 10, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}

		if (!sameLine)
			offset_y += size + padding.y;

		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + 5.0f, pos.y + size / 2 };
		if (center)
			TextCenter(text, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, outline);
		else
			TextLeft(text, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, outline);

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}
	bool buttontab(char* name, valorant::sdk::structs::fvector2d size, bool active)
	{
		elements_count++;

		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 5, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size);

		//Bg
		if (active)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Active);
		}
		else if (isHovered)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.y + padding.y;

		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x / 2, pos.y + size.y / 2 };
		TextCenter(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;

		if (isHovered && input::is_mouse_clicked(0, elements_count, false))
			return true;

		return false;
	}
	bool Button(char* name, valorant::sdk::structs::fvector2d size)
	{
		elements_count++;
		
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 5, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size);
		
		//Bg
		if (isHovered)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Idle);
		}
		
		if (!sameLine)
			offset_y += size.y + padding.y;
		
		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x / 2, pos.y + size.y / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
			TextCenter(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
		
		
		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
		
		if (isHovered && input::is_mouse_clicked(0, elements_count, false))
			return true;
		
		return false;
	}
	void Checkbox(char* name, bool* value)
	{
		elements_count++;
		
		float size = 18;
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 10, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ size, size });
		
		//Bg
		if (isHovered)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size, size, colors::Checkbox_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size, size, colors::Checkbox_Idle);
		}
	
		if (!sameLine)
			offset_y += size + padding.y;
	
		if (*value)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x + 3, pos.y + 3 }, size - 6, size - 6, colors::Checkbox_Enabled);
			//drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x + 9, pos.y + 9 }, size - 18, size - 18, Colors::Checkbox_Hovered);
		}
		
		
		
		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size + 5.0f, pos.y + size / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
			TextLeft(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
		
		
		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
		
		if (isHovered && input::is_mouse_clicked(0, elements_count, false))
			*value = !*value;
	}
	void SliderInt(char* name, int* value, int min, int max)
	{
		elements_count++;

		valorant::sdk::structs::fvector2d size = valorant::sdk::structs::fvector2d{ 240, 50 };
		valorant::sdk::structs::fvector2d slider_size = valorant::sdk::structs::fvector2d{ 200, 10 };
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 10, 15 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, slider_size);

		if (!sameLine)
			offset_y += size.y + padding.y;

		//Bg
		if (isHovered || current_element == elements_count)
		{
			//Drag
			if (input::is_mouse_clicked(0, elements_count, true))
			{
				current_element = elements_count;

				valorant::sdk::structs::fvector2d cursorPos = CursorPos();
				*value = ((cursorPos.x - pos.x) * ((max - min) / slider_size.x)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::Slider_Hovered);
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y + 5.0f }, 5.0f, 5.0f, colors::Slider_Progress);

			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::Slider_Idle);
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y + 5.0f }, 5.0f, 5.0f, colors::Slider_Progress);
		}


		//Value
		float oneP = slider_size.x / (max - min);
		drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, oneP * (*value - min), slider_size.y, colors::Slider_Progress);
		//drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min) - 10.0f, pos.y + slider_size.y - 5.0f + padding.y }, 20.0f, 20.0f, Colors::Slider_Button);
		DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 3.3f + padding.y }, 10.0f, colors::Slider_Button);
		DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 3.3f + padding.y }, 5.0f, colors::Slider_Progress);

		char buffer[32];
		sprintf_s(buffer, "%i", *value);
		valorant::sdk::structs::fvector2d valuePos = valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 25 + padding.y };
		TextCenter(buffer, valuePos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + 5, pos.y + 10 };
		TextLeft(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}
	void SliderFloat(char* name, float* value, float min, float max, char* format = (char*)"%.0f")
	{
		elements_count++;

		valorant::sdk::structs::fvector2d size = valorant::sdk::structs::fvector2d{ 210, 40 };
		valorant::sdk::structs::fvector2d slider_size = valorant::sdk::structs::fvector2d{ 170, 7 };
		valorant::sdk::structs::fvector2d adjust_zone = valorant::sdk::structs::fvector2d{ 0, 20 };
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 10, 15 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y - adjust_zone.y }, valorant::sdk::structs::fvector2d{ slider_size.x, slider_size.y + adjust_zone.y * 1.5f });

		if (!sameLine)
			offset_y += size.y + padding.y;

		//Bg
		if (isHovered || current_element == elements_count)
		{
			//Drag
			if (input::is_mouse_clicked(0, elements_count, true))
			{
				current_element = elements_count;

				valorant::sdk::structs::fvector2d cursorPos = CursorPos();
				*value = ((cursorPos.x - pos.x) * ((max - min) / slider_size.x)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::Slider_Hovered);
			DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x, pos.y + padding.y + 9.3f }, 3.1f, colors::Slider_Progress);
			DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + slider_size.x, pos.y + padding.y + 9.3f }, 3.1f, colors::Slider_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::Slider_Idle);
			DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x, pos.y + padding.y + 9.3f }, 3.1f, colors::Slider_Progress);
			DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + slider_size.x, pos.y + padding.y + 9.3f }, 3.1f, colors::Slider_Idle);
		}


		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x, pos.y + 5 };
		TextLeft(name, textPos, colors::Text, false);

		//Value
		float oneP = slider_size.x / (max - min);
		drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y + slider_size.y + padding.y }, oneP * (*value - min), slider_size.y, colors::Slider_Progress);
		DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 2.66f + padding.y }, 8.0f, colors::Slider_Button);
		DrawFilledCircle(valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 2.66f + padding.y }, 4.0f, colors::Slider_Progress);

		char buffer[32];
		sprintf_s(buffer, format, *value);
		valorant::sdk::structs::fvector2d valuePos = valorant::sdk::structs::fvector2d{ pos.x + oneP * (*value - min), pos.y + slider_size.y + 20 + padding.y };
		TextCenter(buffer, valuePos, colors::Text, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}



	bool checkbox_enabled[256];
	void Combobox(char* name, valorant::sdk::structs::fvector2d size, int* value, const char* arg, ...)
	{
		elements_count++;

		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 5, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size);

		//Bg
		if (isHovered || checkbox_enabled[elements_count])
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Combobox_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Combobox_Idle);
		}

		if (!sameLine)
			offset_y += size.y + padding.y;

		//Text
		valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x + 5.0f, pos.y + size.y / 2 };
		TextLeft(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

		//Elements
		bool isHovered2 = false;
		valorant::sdk::structs::fvector2d element_pos = pos;
		int num = 0;

		if (checkbox_enabled[elements_count])
		{
			current_element_size.x = element_pos.x - 5.0f;
			current_element_size.y = element_pos.y - 5.0f;
		}
		va_list arguments;
		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
		{
			//Selected Element
			if (num == *value)
			{
				valorant::sdk::structs::fvector2d _textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x / 2, pos.y + size.y / 2 };
				TextCenter((char*)arg, _textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
			}

			if (checkbox_enabled[elements_count])
			{
				element_pos.y += 25.0f;
			
				isHovered2 = MouseInZone(valorant::sdk::structs::fvector2d{ element_pos.x, element_pos.y }, valorant::sdk::structs::fvector2d{ size.x, 25.0f });
				if (isHovered2)
				{
					hover_element = true;
					postrenderer::drawFilledRect(valorant::sdk::structs::fvector2d{ element_pos.x, element_pos.y }, size.x, 25.0f, colors::Combobox_Hovered);
					
					//Click
					if (input::is_mouse_clicked(0, elements_count, false))
					{
						*value = num;
						checkbox_enabled[elements_count] = false;
					}
				}
				else
				{
					postrenderer::drawFilledRect(valorant::sdk::structs::fvector2d{ element_pos.x, element_pos.y }, size.x, 25.0f, colors::Combobox_Idle);
				}
			
				postrenderer::TextLeft((char*)arg, valorant::sdk::structs::fvector2d{ element_pos.x + 5.0f, element_pos.y + 15.0f }, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);
			}
			num++;
		}
		va_end(arguments);
		if (checkbox_enabled[elements_count])
		{
			current_element_size.x = element_pos.x + 5.0f;
			current_element_size.y = element_pos.y + 5.0f;
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;

		if (isHovered && input::is_mouse_clicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = !checkbox_enabled[elements_count];
		}
		if (!isHovered && !isHovered2 && input::is_mouse_clicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = false;
		}
	}

	int active_hotkey = -1;
	bool already_pressed = false;
	std::string VirtualKeyCodeToString(UCHAR virtualKey)
	{
		UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		if (virtualKey == VK_LBUTTON)
		{
			return ("MOUSE0");
		}
		if (virtualKey == VK_RBUTTON)
		{
			return ("MOUSE1");
		}
		if (virtualKey == VK_MBUTTON)
		{
			return ("MBUTTON");
		}
		if (virtualKey == VK_XBUTTON1)
		{
			return ("XBUTTON1");
		}
		if (virtualKey == VK_XBUTTON2)
		{
			return ("XBUTTON2");
		}

		CHAR szName[128];
		int result = 0;
		switch (virtualKey)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_RCONTROL: case VK_RMENU:
			case VK_LWIN: case VK_RWIN: case VK_APPS:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= KF_EXTENDED;
			default:
				result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}
		
		return szName;
	}
	void Hotkey(char* name, valorant::sdk::structs::fvector2d size, int* key)
	{
		elements_count++;

		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 5, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y + (last_element_size.y / 2) - size.y / 2;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size);

		//Bg
		if (isHovered)
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.y + padding.y;

		if (active_hotkey == elements_count)
		{
			//Text
			valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x / 2, pos.y + size.y / 2 };
			TextCenter((char*)"[Press Key]", textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (!menu::input::is_any_mouse_down())
			{
				already_pressed = false;
			}

			if (!already_pressed)
			{
				for (int code = 0; code < 255; code++)
				{
					if (GetAsyncKeyState(code))
					{
						*key = code;
						active_hotkey = -1;
					}
				}
			}
		}
		else
		{
			//Text
			valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size.x / 2, pos.y + size.y / 2 };
			TextCenter((char*)VirtualKeyCodeToString(*key).c_str(), textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered)
			{
				if (input::is_mouse_clicked(0, elements_count, false))
				{
					already_pressed = true;
					active_hotkey = elements_count;

					//Queue Fix
					for (int code = 0; code < 255; code++)
						if (GetAsyncKeyState(code)) { }
				}
			}
			else
			{
				if (input::is_mouse_clicked(0, elements_count, false))
				{
					active_hotkey = -1;
				}
			}
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	int active_picker = -1;
	valorant::sdk::structs::flinearcolor saved_color;
	bool ColorPixel(valorant::sdk::structs::fvector2d pos, valorant::sdk::structs::fvector2d size, valorant::sdk::structs::flinearcolor* original, valorant::sdk::structs::flinearcolor color)
	{
		postrenderer::drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size.x, size.y, color);

		//Выбранный цвет
		if (original->r == color.r && original->g == color.g && original->b == color.b)
		{
			postrenderer::Draw_Line(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ pos.x + size.x - 1, pos.y }, 1.0f, valorant::sdk::structs::flinearcolor{ 0.0f, 0.0f, 0.0f, 1.0f });
			postrenderer::Draw_Line(valorant::sdk::structs::fvector2d{ pos.x, pos.y + size.y - 1 }, valorant::sdk::structs::fvector2d{ pos.x + size.x - 1, pos.y + size.y - 1 }, 1.0f, valorant::sdk::structs::flinearcolor{ 0.0f, 0.0f, 0.0f, 1.0f });
			postrenderer::Draw_Line(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ pos.x, pos.y + size.y - 1 }, 1.0f, valorant::sdk::structs::flinearcolor{ 0.0f, 0.0f, 0.0f, 1.0f });
			postrenderer::Draw_Line(valorant::sdk::structs::fvector2d{ pos.x + size.x - 1, pos.y }, valorant::sdk::structs::fvector2d{ pos.x + size.x - 1, pos.y + size.y - 1 }, 1.0f, valorant::sdk::structs::flinearcolor{ 0.0f, 0.0f, 0.0f, 1.0f });
		}

		//Смена цвета
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size);
		if (isHovered)
		{
			if (input::is_mouse_clicked(0, elements_count, false))
				*original = color;
		}

		return true;
	}
	void ColorPicker(char* name, valorant::sdk::structs::flinearcolor* color)
	{
		elements_count++;

		float size = 25;
		valorant::sdk::structs::fvector2d padding = valorant::sdk::structs::fvector2d{ 10, 10 };
		valorant::sdk::structs::fvector2d pos = valorant::sdk::structs::fvector2d{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y + offset_y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (pushY)
		{
			pos.y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.y - menu_pos.y;
		}
		bool isHovered = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ size, size });

		if (!sameLine)
			offset_y += size + padding.y;

		if (active_picker == elements_count)
		{
			hover_element = true;

			float sizePickerX = 250;
			float sizePickerY = 250;
			bool isHoveredPicker = MouseInZone(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, valorant::sdk::structs::fvector2d{ sizePickerX, sizePickerY - 60 });

			//Background
			postrenderer::drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, sizePickerX, sizePickerY - 65, colors::ColorPicker_Background);

			//float pixedSize = sizePickerY / pixels;
			//valorant::sdk::structs::flinearcolor temp_color{1.0f, 1.0f, 1.0f, 1.0f};
			//float iterator = 0.0f;
			//
			//for (int y = 0; y < pixels; y++)
			//{
			//	for (int x = 0; x < pixels; x++)
			//	{
			//		ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixedSize * x, pos.y + pixedSize * y }, pixedSize, color, temp_color);
			//		temp_color.R -= (1.0f - saved_color.R) / pixels;
			//		temp_color.G -= (1.0f - saved_color.G) / pixels;
			//		temp_color.B -= (1.0f - saved_color.B) / pixels;
			//	}
			//	
			//	iterator += 1.0f / pixels;
			//	temp_color = valorant::sdk::structs::flinearcolor{ 1.0f - iterator, 1.0f - iterator, 1.0f - iterator, 1.0f };
			//}

			valorant::sdk::structs::fvector2d pixelSize = valorant::sdk::structs::fvector2d{ sizePickerX/12, sizePickerY/12 };

			//0
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 174/255.f, 235/255.f, 253/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 136/255.f, 225/255.f, 251/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 108/255.f, 213/255.f, 250/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 89/255.f, 175/255.f, 213/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 76/255.f, 151/255.f, 177/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 60/255.f, 118/255.f, 140/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 43/255.f, 85/255.f, 100/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 32/255.f, 62/255.f, 74/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 0, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 255/255.f, 255/255.f, 255/255.f, 1.0f });
			}
			//1
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 175/255.f, 205/255.f, 252/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 132/255.f, 179/255.f, 252/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 90/255.f, 152/255.f, 250/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 55/255.f, 120/255.f, 250/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 49/255.f, 105/255.f, 209/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 38/255.f, 83/255.f, 165/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 28/255.f, 61/255.f, 120/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 20/255.f, 43/255.f, 86/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 1, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 247/255.f, 247/255.f, 247/255.f, 1.0f });
			}
			//2
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 153/255.f, 139/255.f, 250/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 101/255.f, 79/255.f, 249/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 64/255.f, 50/255.f, 230/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 54/255.f, 38/255.f, 175/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 39/255.f, 31/255.f, 144/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 32/255.f, 25/255.f, 116/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 21/255.f, 18/255.f, 82/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 16/255.f, 13/255.f, 61/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 2, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 228/255.f, 228/255.f, 228/255.f, 1.0f });
			}
			//3
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 194/255.f, 144/255.f, 251/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 165/255.f, 87/255.f, 249/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 142/255.f, 57/255.f, 239/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 116/255.f, 45/255.f, 184/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 92/255.f, 37/255.f, 154/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 73/255.f, 29/255.f, 121/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 53/255.f, 21/255.f, 88/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 37/255.f, 15/255.f, 63/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 3, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 203/255.f, 203/255.f, 203/255.f, 1.0f });
			}
			//4
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 224/255.f, 162/255.f, 197/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 210/255.f, 112/255.f, 166/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 199/255.f, 62/255.f, 135/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 159/255.f, 49/255.f, 105/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 132/255.f, 41/255.f, 89/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 104/255.f, 32/255.f, 71/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 75/255.f, 24/255.f, 51/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 54/255.f, 14/255.f, 36/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 4, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 175/255.f, 175/255.f, 175/255.f, 1.0f });
			}
			//5
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 235/255.f, 175/255.f, 176/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 227/255.f, 133/255.f, 135/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 219/255.f, 87/255.f, 88/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 215/255.f, 50/255.f, 36/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 187/255.f, 25/255.f, 7/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 149/255.f, 20/255.f, 6/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 107/255.f, 14/255.f, 4/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 77/255.f, 9/255.f, 3/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 5, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 144/255.f, 144/255.f, 144/255.f, 1.0f });
			}
			//6
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 241/255.f, 187/255.f, 171/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 234/255.f, 151/255.f, 126/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 229/255.f, 115/255.f, 76/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 227/255.f, 82/255.f, 24/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 190/255.f, 61/255.f, 15/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 150/255.f, 48/255.f, 12/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 107/255.f, 34/255.f, 8/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 79/255.f, 25/255.f, 6/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 6, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 113/255.f, 113/255.f, 113/255.f, 1.0f });
			}
			//7
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 245/255.f, 207/255.f, 169/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 240/255.f, 183/255.f, 122/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 236/255.f, 159/255.f, 74/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 234/255.f, 146/255.f, 37/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 193/255.f, 111/255.f, 28/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 152/255.f, 89/255.f, 22/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 110/255.f, 64/255.f, 16/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 80/255.f, 47/255.f, 12/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 7, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 82/255.f, 82/255.f, 82/255.f, 1.0f });
			}
			//8
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 247/255.f, 218/255.f, 170/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 244/255.f, 200/255.f, 124/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 241/255.f, 182/255.f, 77/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 239/255.f, 174/255.f, 44/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 196/255.f, 137/255.f, 34/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 154/255.f, 108/255.f, 27/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 111/255.f, 77/255.f, 19/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 80/255.f, 56/255.f, 14/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 8, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 54/255.f, 54/255.f, 54/255.f, 1.0f });
			}
			//9
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 254/255.f, 243/255.f, 187/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 253/255.f, 237/255.f, 153/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 253/255.f, 231/255.f, 117/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 254/255.f, 232/255.f, 85/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 242/255.f, 212/255.f, 53/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 192/255.f, 169/255.f, 42/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 138/255.f, 120/255.f, 30/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 101/255.f, 87/255.f, 22/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 9, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 29/255.f, 29/255.f, 29/255.f, 1.0f });
			}
			//10
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 247/255.f, 243/255.f, 185/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 243/255.f, 239/255.f, 148/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 239/255.f, 232/255.f, 111/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 235/255.f, 229/255.f, 76/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 208/255.f, 200/255.f, 55/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 164/255.f, 157/255.f, 43/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 118/255.f, 114/255.f, 31/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 86/255.f, 82/255.f, 21/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 10, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 9/255.f, 9/255.f, 9/255.f, 1.0f });
			}
			//11
			{
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 0 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 218/255.f, 232/255.f, 182/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 1 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 198/255.f, 221/255.f, 143/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 2 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 181/255.f, 210/255.f, 103/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 3 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 154/255.f, 186/255.f, 76/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 4 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 130/255.f, 155/255.f, 64/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 5 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 102/255.f, 121/255.f, 50/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 6 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 74/255.f, 88/255.f, 36/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 7 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 54/255.f, 64/255.f, 26/255.f, 1.0f });
				ColorPixel(valorant::sdk::structs::fvector2d{ pos.x + pixelSize.x * 11, pos.y + pixelSize.y * 8 }, pixelSize, color, valorant::sdk::structs::flinearcolor{ 0/255.f, 0/255.f, 0/255.f, 1.0f });
			}
			
			

			if (isHoveredPicker)
			{
				if (input::is_mouse_clicked(0, elements_count, false))
				{

				}
			}
			else
			{
				if (input::is_mouse_clicked(0, elements_count, false))
				{
					active_picker = -1;
					//hover_element = false;
				}
			}
		}
		else
		{
			//Bg
			if (isHovered)
			{
				drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size, size, colors::Checkbox_Hovered);
				hover_element = true;
			}
			else
			{
				drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x, pos.y }, size, size, colors::Checkbox_Idle);
			}

			//Color
			drawFilledRect(valorant::sdk::structs::fvector2d{ pos.x + 4, pos.y + 4 }, size - 8, size - 8, *color);

			//Text
			valorant::sdk::structs::fvector2d textPos = valorant::sdk::structs::fvector2d{ pos.x + size + 5.0f, pos.y + size / 2 };
			TextLeft(name, textPos, valorant::sdk::structs::flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			if (isHovered && input::is_mouse_clicked(0, elements_count, false))
			{
				saved_color = *color;
				active_picker = elements_count;
			}
		}


		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}


	void render()
	{
		for (int i = 0; i < 128; i++)
		{
			if (postrenderer::drawlist[i].type != -1)
			{
				//Filled Rect
				if (postrenderer::drawlist[i].type == 1)
				{
					menu::drawFilledRect(postrenderer::drawlist[i].pos, postrenderer::drawlist[i].size.x, postrenderer::drawlist[i].size.y, postrenderer::drawlist[i].color);
				}
				//TextLeft
				else if (postrenderer::drawlist[i].type == 2)
				{
					menu::TextLeft(postrenderer::drawlist[i].name, postrenderer::drawlist[i].pos, postrenderer::drawlist[i].color, postrenderer::drawlist[i].outline);
				}
				//TextCenter
				else if (postrenderer::drawlist[i].type == 3)
				{
					menu::TextCenter(postrenderer::drawlist[i].name, postrenderer::drawlist[i].pos, postrenderer::drawlist[i].color, postrenderer::drawlist[i].outline);
				}
				//Draw_Line
				else if (postrenderer::drawlist[i].type == 4)
				{
					Draw_Line(postrenderer::drawlist[i].from, postrenderer::drawlist[i].to, postrenderer::drawlist[i].thickness, postrenderer::drawlist[i].color);
				}

				postrenderer::drawlist[i].type = -1;
			}
		}
	}
}
