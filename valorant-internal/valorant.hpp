#include "utils.h"
#include "vmthook.h"

namespace menu
{
	void render_menu(sdk::uobject* font, sdk::ucanvas* canvas);
}

namespace valorant
{
	using draw_transition = void(*)(sdk::ugameviewportclient* viewportclient, sdk::ucanvas* canvas);
	draw_transition draw_transition_o = 0;

	void draw_transition_hook(sdk::ugameviewportclient* viewportclient, sdk::ucanvas* canvas)
	{
		if (!canvas)
			return draw_transition_hook(viewportclient, canvas);

		int target_id = 1337;
		float closest_distance = FLT_MAX;

		do
		{
			sdk::uworld* world = reinterpret_cast<sdk::uworld*>( viewportclient->get_world() );
			if (!world) continue;

			sdk::ugameinstance* gameinstance = reinterpret_cast<sdk::ugameinstance*>( viewportclient->get_gameinstance() );
			if (!gameinstance) continue;

			sdk::uengine* uengine = gameinstance->get_uengine();
			if (!uengine) continue;

			sdk::aplayercontroller* controller = sdk::blueprints::get_player_controller(world);
			if (!controller) continue;

			sdk::acknowledgedpawn* pawn = controller->get_acknowledged_pawn();

			sdk::ashootercharacter* character = controller->get_shooter_character();
			if (!character) continue;

			sdk::aplayercameramanager* camera = controller->get_camera_manager();
			if (!camera) continue;

			sdk::uobject* font = uengine->get_font();
			if (!font) continue;

			menu::render_menu(font, canvas);

			sdk::structs::tarray<sdk::ashootercharacter*> actors = sdk::blueprints::find_all_shooters_with_alliance(world, character, sdk::structs::earesalliance::any, false, true);
			for (int idx = 0; idx < actors.count; idx++)
			{
				sdk::ashootercharacter* actor = actors[idx];
				if (!actor) continue; if (actor == character) continue;
				sdk::uskeletalmeshcomponent* mesh = actor->get_mesh();
				if (!mesh) continue;

				sdk::structs::fvector head_location = mesh->get_bone_location(8);
				sdk::structs::fvector2d head_location_2d = controller->project_world_to_screen(head_location);
				sdk::structs::fvector root_location = mesh->get_bone_location(0);
				sdk::structs::fvector2d root_location_2d = controller->project_world_to_screen(root_location);
				if (!head_location_2d.is_valid() || !root_location_2d.is_valid()) continue;

				if (globals::box)
					utils::box_3d(sdk::structs::fvector(root_location.x, root_location.y, root_location.z + 100), sdk::structs::fvector(124, 124, 208), canvas, controller);

				float screen_distance = sdk::math::distance_2d(head_location_2d, {1920 / 2, 1080 / 2});
				if (screen_distance < closest_distance) {
					target_id = idx;
					closest_distance = screen_distance;
				}
			}

			if (target_id != 1337 && GetAsyncKeyState(globals::aimkey))
			{
				sdk::ashootercharacter* actor = actors[target_id];
				if (!actor || actor == character) continue;
				sdk::uskeletalmeshcomponent* mesh = actor->get_mesh();
				if (!mesh) continue;

				if (actor->is_alive())
				{
					sdk::structs::fvector camera_location = camera->get_camera_location();
					sdk::structs::fvector aim_rotation = sdk::math::find_look_at_rotation(camera_location, mesh->get_bone_location(8));
					controller->set_control_rotation(aim_rotation);
				}
			}

		} while (false);

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
			variables::math_system = sdk::uobject::find_object<sdk::uobject*>(L"Engine.Default__KismetMathLibrary");

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

			if (world == nullptr)
				return;
			
			sdk::structs::tarray<sdk::ulocalplayer*> localplayers_array = world->gameinstance()->localplayers();
			sdk::ulocalplayer* localplayer = localplayers_array[0];
			sdk::ugameviewportclient* viewportclient = localplayer->viewportclient();

			hook::vmt((void*)viewportclient, draw_transition_hook, 0x69, (void**)&draw_transition_o);
		}
	}
}