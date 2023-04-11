#include "structs.hpp"

namespace valorant
{
	namespace sdk
	{
		class uobject
		{
		public:
			template <typename t>
			__forceinline static t find_object(const wchar_t* name, uobject* outer = nullptr)
			{
				return uobject::static_find_object<t>(nullptr, outer, name);
			}

			template <typename t>
			__forceinline static t static_find_object(uobject* klass, uobject* outer, const wchar_t* name)
			{
				return reinterpret_cast<t (*)(uobject*, uobject*, const wchar_t*)>(valorant::image_base + offsets::static_find_object)(klass, outer, name);
			}

			__forceinline void process_event(uobject* function, void* args)
			{
				return reinterpret_cast<void (*)(uobject*, uobject*, void*)>(valorant::image_base + offsets::process_event)(this, function, args);
			}
		};

		class uskeletalmeshcomponent : public uobject
		{
		public:
			structs::fvector get_bone_location(int index);
		};

		class aplayercameramanager : public uobject
		{

		};

		class aplayerstate : public uobject
		{

		};
		
		class acknowledgedpawn : public uobject
		{
		public:
		};

		class ashootercharacter : public uobject
		{
		public:
			uskeletalmeshcomponent* get_mesh();
			bool is_alive();
			float health();
		};

		class aplayercontroller : public uobject
		{
		public:
			ashootercharacter* get_shooter_character();
			acknowledgedpawn* get_acknowledged_pawn();
			aplayercameramanager* get_camera_manager();
			structs::fvector2d project_world_to_screen(structs::fvector world);
		};

		class ugameviewportclient : public uobject
		{
		public:
			uobject* get_world();
		};

		class ulocalplayer : public uobject
		{
		public:
			ugameviewportclient* viewportclient();
		};
		
		class ugameinstance : public uobject
		{
		public:
			structs::tarray<ulocalplayer*> localplayers();
		};

		class uworld : public uobject
		{
		public:
			ugameinstance* gameinstance();
		};

		class ucanvas : public uobject
		{
		public:
			void k2_drawline(structs::fvector2d screenpos_a, structs::fvector2d screenpos_b, float thickness, structs::flinearcolor color);
		};

		class blueprints
		{
		public:
			static aplayercontroller* get_player_controller(uworld* context);
			static structs::tarray<ashootercharacter*> find_all_shooters_with_alliance(uworld* context, ashootercharacter* viewer, structs::earesalliance alliance, bool only_player, bool only_alive);
		};

		class system
		{
		public:
			static structs::fstring get_object_name(uobject* object);
		};

	}
}

namespace variables
{
	inline valorant::sdk::uobject* blueprints;
	inline valorant::sdk::uobject* gameplay_statics;
	inline valorant::sdk::uobject* kismet_system;
}