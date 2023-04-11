#include "sdk.hpp"

using namespace valorant;

namespace utils
{
	void box_3d(sdk::structs::fvector origin, sdk::structs::fvector extends, sdk::ucanvas* canvas, sdk::aplayercontroller* sdk)
	{
		origin -= extends / 2.f;

		sdk::structs::fvector one = origin;
		sdk::structs::fvector two = origin; two.x += extends.x;
		sdk::structs::fvector three = origin; three.x += extends.x; three.y += extends.y;
		sdk::structs::fvector four = origin; four.y += extends.y;

		sdk::structs::fvector five = one; five.z += extends.z;
		sdk::structs::fvector six = two; six.z += extends.z;
		sdk::structs::fvector seven = three; seven.z += extends.z;
		sdk::structs::fvector eight = four; eight.z += extends.z;

		sdk::structs::fvector2d s1, s2, s3, s4, s5, s6, s7, s8;
		s1 = sdk->project_world_to_screen(one);
		s2 = sdk->project_world_to_screen(two);
		s3 = sdk->project_world_to_screen(three);
		s4 = sdk->project_world_to_screen(four);
		s5 = sdk->project_world_to_screen(five);
		s6 = sdk->project_world_to_screen(six);
		s7 = sdk->project_world_to_screen(seven);
		s8 = sdk->project_world_to_screen(eight);
		if (!s1.is_valid() || !s2.is_valid() || !s3.is_valid() || !s4.is_valid() || !s5.is_valid() || !s6.is_valid() || !s7.is_valid() || !s8.is_valid())
			return;

		canvas->k2_drawline(s1, s2, 1, { 1,1,1,1 });
		canvas->k2_drawline(s2, s3, 1, { 1,1,1,1 });
		canvas->k2_drawline(s3, s4, 1, { 1,1,1,1 });
		canvas->k2_drawline(s4, s1, 1, { 1,1,1,1 });

		canvas->k2_drawline(s5, s6, 1, { 1,1,1,1 });
		canvas->k2_drawline(s6, s7, 1, { 1,1,1,1 });
		canvas->k2_drawline(s7, s8, 1, { 1,1,1,1 });
		canvas->k2_drawline(s8, s5, 1, { 1,1,1,1 });

		canvas->k2_drawline(s1, s5, 1, { 1,1,1,1 });
		canvas->k2_drawline(s2, s6, 1, { 1,1,1,1 });
		canvas->k2_drawline(s3, s7, 1, { 1,1,1,1 });
		canvas->k2_drawline(s4, s8, 1, { 1,1,1,1 });
	}
}