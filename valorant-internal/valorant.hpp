#include "sdk.hpp"
#include "vmthook.h"

using namespace valorant;

namespace valorant
{
	using draw_transition = void(*)(sdk::ugameviewportclient* viewportclient, sdk::ucanvas* canvas);
	draw_transition draw_transition_o = 0;

	void draw_transition_hook(sdk::ugameviewportclient* viewportclient, sdk::ucanvas* canvas)
	{
		if (!canvas)
			return draw_transition_hook(viewportclient, canvas);

		sdk::uworld* world = reinterpret_cast<sdk::uworld*>(viewportclient->get_world());

		sdk::aplayercontroller* controller = sdk::blueprints::get_player_controller(world);

		sdk::ashootercharacter* character = controller->get_shooter_character();

		sdk::aplayercameramanager* camera = controller->get_camera_manager();

		if (camera != nullptr)
		{
			sdk::structs::tarray<sdk::ashootercharacter*> actors = sdk::blueprints::find_all_shooters_with_alliance(world, character, sdk::structs::earesalliance::any, false, true);
			for (int idx = 0; idx < actors.count; idx++)
			{
				sdk::ashootercharacter* actor = actors[idx];
				if (!actor) continue; if (actor == character) continue;
				sdk::uskeletalmeshcomponent* mesh = actor->get_mesh();
				if (!mesh) continue;

				sdk::structs::fvector head_location = mesh->get_bone_location(0);
				if (!head_location.is_valid()) continue;
				sdk::structs::fvector2d head_location_2d = controller->project_world_to_screen(head_location);
				if (!head_location_2d.is_valid()) continue;

				canvas->k2_drawline(head_location_2d, { 1920 / 2, 1080 }, 1, { 1,1,1,1 });
			}
		}

		return draw_transition_o(viewportclient, canvas);
	}

	namespace start
	{
		__forceinline void init_cheat(std::uintptr_t image_base)
		{
			valorant::image_base = image_base;

			variables::blueprints = sdk::uobject::find_object<sdk::uobject*>(L"ShooterGame.Default__ShooterBlueprintLibrary");
			variables::gameplay_statics = sdk::uobject::find_object<sdk::uobject*>(L"Engine.Default__GameplayStatics");
			variables::kismet_system = sdk::uobject::find_object<sdk::uobject*>(L"Engine.Default__KismetSystemLibrary");

			sdk::uworld* world = 0;
			const wchar_t* uworld_names[] = {
			   L"/Game/Maps/Init/Init.Init",
			   L"/Game/Maps/Menu/MainMenuV2.MainMenuV2",
			   L"/Game/Maps/Poveglia/Range.Range",
			   L"/Game/Maps/Ascent/Ascent.Ascent",
			   L"/Game/Maps/Canyon/Canyon.Canyon",
			   L"/Game/Maps/Bonsai/Bonsai.Bonsai",
			   L"/Game/Maps/Duality/Duality.Duality",
			   L"/Game/Maps/Foxtrot/Foxtrot.Foxtrot",
			   L"/Game/Maps/Port/Port.Port",
			   L"/Game/Maps/Triad/Triad.Triad",
			   L"/Game/Maps/Pitt/Pitt.Pitt",
			};
			for (int index = 0; index < sizeof(uworld_names) / sizeof(wchar_t*); index++)
			{
				world = sdk::uobject::find_object<sdk::uworld*>(uworld_names[index]);
				if (world) { break; }
			}

			sdk::structs::tarray<sdk::ulocalplayer*> localplayers_array = world->gameinstance()->localplayers();
			sdk::ulocalplayer* localplayer = localplayers_array[0];
			sdk::ugameviewportclient* viewportclient = localplayer->viewportclient();

			vmtshadow hook(viewportclient);
			draw_transition_o = (draw_transition)hook.apply(0x68, reinterpret_cast<std::uintptr_t*>(draw_transition_hook));
		}
	}
}