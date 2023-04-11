#include "sdk.hpp"
#include "spoofcall.h"

using namespace valorant::sdk;

ugameinstance* uworld::gameinstance()
{
	return memory::read<ugameinstance*>(std::uintptr_t(std::uintptr_t(this)) + offsets::game_instance);
}

structs::tarray<ulocalplayer*> ugameinstance::localplayers()
{
	return memory::read<structs::tarray<ulocalplayer*>>(std::uintptr_t(std::uintptr_t(this)) + offsets::local_players);
}

ugameviewportclient* ulocalplayer::viewportclient()
{
	return memory::read<ugameviewportclient*>(std::uintptr_t(this) + offsets::viewportclient);
}

uobject* ugameviewportclient::get_world()
{
	return memory::read<uobject*>(std::uintptr_t(this) + offsets::world);
}

aplayercontroller* blueprints::get_player_controller(uworld* context)
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterBlueprintLibrary.GetFirstLocalPlayerController");
	if (!function)
		return nullptr;

	struct
	{
		uworld* context;
		aplayercontroller* output;
	} params;

	params.context = context;

	variables::blueprints->process_event(function, &params);
	
	return params.output;
}

structs::tarray<ashootercharacter*> blueprints::find_all_shooters_with_alliance(uworld* context, ashootercharacter* viewer, structs::earesalliance alliance, bool only_player, bool only_alive)
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterBlueprintLibrary.FindAllShooterCharactersWithAlliance");

	struct
	{
		uworld* context;
		ashootercharacter* viewer;
		structs::earesalliance alliance;
		bool only_player;
		bool only_alive;
		structs::tarray<ashootercharacter*> output;
	} params;

	params.context = context;
	params.viewer = viewer;
	params.alliance = alliance;
	params.only_player = only_player;
	params.only_alive = only_alive;

	variables::blueprints->process_event(function, &params);

	return params.output;
}

ashootercharacter* aplayercontroller::get_shooter_character()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.AresPlayerController.GetShooterCharacter");
	if (!function)
		return nullptr;

	struct
	{
		ashootercharacter* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

acknowledgedpawn* aplayercontroller::get_acknowledged_pawn()
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.Controller.K2_GetPawn");
	if (!function)
		return nullptr;

	struct
	{
		acknowledgedpawn* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

aplayercameramanager* aplayercontroller::get_camera_manager()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.AresPlayerController.GetPlayerCameraManager");
	if (!function)
		return nullptr;

	struct
	{
		aplayercameramanager* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

uskeletalmeshcomponent* ashootercharacter::get_mesh()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterCharacter.GetPawnMesh");
	if (!function)
		return nullptr;

	struct
	{
		uskeletalmeshcomponent* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

structs::fvector2d aplayercontroller::project_world_to_screen(structs::fvector world)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.PlayerController.ProjectWorldLocationToScreen");
	if (!function)
		return structs::fvector2d();

	struct
	{
		structs::fvector world;
		structs::fvector2d screen;
		bool relative_viewport;
	} params;

	params.world = world;
	params.relative_viewport = true;

	this->process_event(function, &params);

	return params.screen;
}

void ucanvas::k2_drawline(structs::fvector2d screenpos_a, structs::fvector2d screenpos_b, float thickness, structs::flinearcolor color)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.Canvas.K2_DrawLine");
	if (!function)
		return;

	struct
	{
		structs::fvector2d screenpos_a;
		structs::fvector2d screenpos_b;
		float thickness;
		structs::flinearcolor color;
	} params;

	params.screenpos_a = screenpos_a;
	params.screenpos_b = screenpos_b;
	params.thickness = thickness;
	params.color = color;

	this->process_event(function, &params);
}

structs::fvector uskeletalmeshcomponent::get_bone_location(int index)
{
	sdk::structs::fmatrix Matrix;
	reinterpret_cast<sdk::structs::fmatrix* (__fastcall*)(uskeletalmeshcomponent*, sdk::structs::fmatrix*, int)>(valorant::image_base + offsets::bone_matrix)(this, &Matrix, index); // E8 ?? ?? ?? ?? 48 8B 47 30 F3 0F 10 45 ??
	return { Matrix.WPlane.x, Matrix.WPlane.y, Matrix.WPlane.z };
}

bool ashootercharacter::is_alive()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterCharacter.IsAlive");
	if (!function)
		return false;

	struct
	{
		bool output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

float ashootercharacter::health()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterCharacter.GetHealth");
	if (!function)
		return false;

	struct
	{
		float output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

structs::fstring system::get_object_name(uobject* object)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.KismetSystemLibrary.GetObjectName");
	if (!function)
		return structs::fstring();

	struct
	{
		uobject* object;
		structs::fstring output;
	} params;

	params.object = object;

	variables::kismet_system->process_event(function, &params);

	return params.output;
}

void ucanvas::k2_drawtext(uobject* font, structs::fstring text, structs::fvector2d screenposition, structs::fvector2d scale, structs::flinearcolor color, float kerning, structs::flinearcolor shadowcolor, structs::fvector2d shadowoffset, bool centrex, bool centrey, bool outline, structs::flinearcolor outlinecolor)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.Canvas.K2_DrawText");
	if (!function)
		return;

	struct
	{
		uobject* font;
		structs::fstring text;
		structs::fvector2d screenposition;
		structs::fvector2d scale;
		structs::flinearcolor color;
		float kerning;
		structs::flinearcolor shadowcolor;
		structs::fvector2d shadowoffset;
		bool centrex;
		bool centrey;
		bool outline;
		structs::flinearcolor outlinecolor;
	} params;

	params.font = font;
	params.text = text;
	params.screenposition = screenposition;
	params.scale = scale;
	params.color = color;
	params.kerning = kerning;
	params.shadowcolor = shadowcolor;
	params.shadowoffset = shadowoffset;
	params.centrex = centrex;
	params.centrey = centrey;
	params.outline = outline;
	params.outlinecolor = outlinecolor;

	this->process_event(function, &params);
}

uobject* system::get_outer_object(uobject* object)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.KismetSystemLibrary.GetOuterObject");
	if (!function)
		return nullptr;

	struct
	{
		uobject* object;
		uobject* output;
	} params;

	variables::kismet_system->process_event(function, &params);

	return params.output;
}

uobject* uengine::get_font()
{
	return memory::read<uobject*>(std::uintptr_t(this) + 0x88);
}

uobject* ugameviewportclient::get_gameinstance()
{
	return memory::read<uobject*>(std::uintptr_t(this) + 0x88);
}

uengine* ugameinstance::get_uengine()
{
	return memory::read<uengine*>(std::uintptr_t(this) + 0x28);
}

float math::distance_2d(structs::fvector2d v1, structs::fvector2d v2)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.KismetMathLibrary.Distance2D");
	if (!function)
		return 0;

	struct
	{
		structs::fvector2d v1;
		structs::fvector2d v2;
		float output;
	} params;

	params.v1 = v1;
	params.v2 = v2;

	variables::math_system->process_event(function, &params);

	return params.output;
}

structs::fvector math::find_look_at_rotation(structs::fvector start, structs::fvector end)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.KismetMathLibrary.FindLookAtRotation");
	if (!function)
		return 0;

	struct
	{
		structs::fvector start;
		structs::fvector end;
		structs::fvector output;
	} params;

	params.start = start;
	params.end = end;

	variables::math_system->process_event(function, &params);

	return params.output;
}

structs::fvector aplayercameramanager::get_camera_location()
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.PlayerCameraManager.GetCameraLocation");
	if (!function)
		return 0;

	struct
	{
		structs::fvector output;
	} params;

	this->process_event(function, &params);

	return params.output;
}

void aplayercontroller::set_control_rotation(structs::fvector angle)
{
	uobject* function = uobject::find_object<uobject*>(L"Engine.Controller.SetControlRotation");
	if (!function)
		return;

	struct
	{
		structs::fvector angle;
	} params;

	params.angle = angle;

	this->process_event(function, &params);
}

uinventory* ashootercharacter::get_inventory()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.ShooterCharacter.GetInventory");
	if (!function)
		return 0;

	struct
	{
		uinventory* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}


currentequippable* uinventory::get_current_equippable()
{
	uobject* function = uobject::find_object<uobject*>(L"ShooterGame.AresInventory.GetCurrentEquippable");
	if (!function)
		return 0;

	struct
	{
		currentequippable* output;
	} params;

	this->process_event(function, &params);

	return params.output;
}
