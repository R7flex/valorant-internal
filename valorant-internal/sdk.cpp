#include "sdk.hpp"

using namespace valorant::sdk;

ugameinstance* uworld::gameinstance()
{
	return *reinterpret_cast<ugameinstance**>(this + offsets::game_instance);
}

structs::tarray<ulocalplayer*> ugameinstance::localplayers()
{
	return *reinterpret_cast<structs::tarray<ulocalplayer*> *>(this + offsets::local_players);
}

ugameviewportclient* ulocalplayer::viewportclient()
{
	return *reinterpret_cast<ugameviewportclient**>(this + offsets::viewportclient);
}

uobject* ugameviewportclient::get_world()
{
	return *reinterpret_cast<uobject**>(this + offsets::world);
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
