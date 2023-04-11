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

		class system
		{
		public:
			static structs::fstring get_object_name(uobject* object);
			static uobject* get_outer_object(uobject* object);
		};

		class equippable_skin_data_asset : public uobject
		{
		public:
			
		};

		class currentequippable : public uobject
		{
		public:
			struct type {
				const wchar_t* search = nullptr;
				const char* config = nullptr;
				// pinguu
				std::int32_t index = 0;

				const bool is_valid() const noexcept {
					return this->search != nullptr || this->config != nullptr;
				}
			};

			static inline currentequippable::type types[] = {
				{ L"default__basepistol", "classic" },
				{ L"default__sawedoffshotgun", "shorty" },
				{ L"default__autopistol", "frenzy" },
				{ L"default__luger", "ghost" },
				{ L"default__revolver", "sheriff" },

				{ L"default__vector", "stinger" },
				{ L"default__mp5", "spectre" }, // 6

				{ L"default__pumpshotgun", "bucky" },
				{ L"default__automaticshotgun", "judge" },

				{ L"default__burst", "bulldog" },
				{ L"default__dmr", "guardian" },
				{ L"default__assaultrifle_acr", "phantom" }, // 11
				{ L"default__ak", "vandal" },

				{ L"default__leversniperrifle", "marshal" },
				{ L"default__boltsniper", "operator" },

				{ L"default__lmg", "ares" },
				{ L"default__heavymachinegun", "odin" }, // 16

				{ L"default__melee", "knife" },

				{ L"default__carbine", "phantom", 11 },
				{ L"default__automaticpistol", "frenzy", 2 },
				{ L"default__subMachinegun_mp5", "spectre", 6 },
				{ L"Default__Slim", "shorty", 1 },
				{ L"Default__hmg", "odin", 16 },
				{ L"Default__lightmachinegun", "ares", 15 },
				{ L"Default__assaultrifle_ak", "vandal", 12 },
				{ L"Default__AssaultRifle_Burst", "bulldog", 9 },
				{ L"Default__AutoShotgun", "judge", 8 },
				{ L"Default__LeverSniper", "marshal", 13 },
			};

			currentequippable::type get_type() {
				const structs::fstring name = sdk::system::get_object_name(this);

				for (std::int32_t index = 0; index < sizeof(currentequippable::types) / sizeof(currentequippable::type); index++) {
					currentequippable::type type = currentequippable::types[index];

					if (type.index == 0)
						type.index = index;

					const wchar_t* txt = type.search;
					std::wstring ws(txt);
					std::string search(ws.begin(), ws.end());

					if (crt::contains(name.to_str(), search))
						return { type.search, type.config, type.index };
				}

				return {};
			}
		};

		class uinventory : public uobject
		{
		public:
			currentequippable* get_current_equippable();
		};

		class uskeletalmeshcomponent : public uobject
		{
		public:
			structs::fvector get_bone_location(int index);
		};

		class aplayercameramanager : public uobject
		{
		public:
			structs::fvector get_camera_location();
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
			uinventory* get_inventory();
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
			void set_control_rotation(structs::fvector angle);
		};

		class ugameviewportclient : public uobject
		{
		public:
			uobject* get_world();
			uobject* get_gameinstance();
		};

		class ulocalplayer : public uobject
		{
		public:
			ugameviewportclient* viewportclient();
		};
		
		class uengine : public uobject
		{
		public:
			uobject* get_font();
		};

		class ugameinstance : public uobject
		{
		public:
			uengine* get_uengine();
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
			void k2_drawtext(uobject* font, structs::fstring text, structs::fvector2d screenposition, structs::fvector2d scale, structs::flinearcolor color, float kerning, structs::flinearcolor shadowcolor, structs::fvector2d shadowoffset, bool centrex, bool centrey, bool outline, structs::flinearcolor outlinecolor);
		};

		class blueprints
		{
		public:
			static aplayercontroller* get_player_controller(uworld* context);
			static structs::tarray<ashootercharacter*> find_all_shooters_with_alliance(uworld* context, ashootercharacter* viewer, structs::earesalliance alliance, bool only_player, bool only_alive);
		};

		class math
		{
		public:
			static float distance_2d(structs::fvector2d v1, structs::fvector2d v2);
			static structs::fvector find_look_at_rotation(structs::fvector start, structs::fvector end);
		};

	}
}

namespace variables
{
	inline valorant::sdk::uobject* blueprints;
	inline valorant::sdk::uobject* gameplay_statics;
	inline valorant::sdk::uobject* kismet_system;
	inline valorant::sdk::uobject* math_system;
}