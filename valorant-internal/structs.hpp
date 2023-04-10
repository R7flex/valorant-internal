#include "defines.hpp"

namespace valorant {
	namespace sdk {
		namespace structs {
			class fname {
			public:
				fname() : index() { }
				fname(std::uint32_t _index) : index(_index) { }

				std::uint32_t index;
			};

			class fkey {
			public:
				fname name;
				std::uint8_t details[20];
			};

			class fmatrix {
			public:
				double m[4][4];
			};

			class ftextdata {
			public:
				char pad_0[0x30];
				wchar_t* name;
				__int32 length;
			};

			class ftext {
			public:
				ftextdata* data;
				char pad_0[0x10];

				wchar_t* c_str() const {
					if (this->data) {
						return data->name;
					}

					return nullptr;
				}
			};

			class frotator {
			public:
				frotator() : pitch(0.f), yaw(0.f), roll(0.f) {}
				frotator(double pitch, double yaw) : pitch(pitch), yaw(yaw), roll(0.f) {}
				frotator(double pitch, double yaw, double roll) : pitch(pitch), yaw(yaw), roll(roll) {}

				frotator operator + (const frotator& other) const { return { this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll }; }
				frotator operator - (const frotator& other) const { return { this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll }; }
				frotator operator * (double scalar) const { return { this->pitch * scalar, this->yaw * scalar, this->roll * scalar }; }
				frotator operator / (double divide) const { return { this->pitch / divide, this->yaw / divide, this->roll / divide }; }

				frotator& operator *= (const double other) { this->yaw *= other; this->pitch *= other; this->roll *= other; return *this; }
				frotator& operator /= (const double other) { this->yaw /= other; this->pitch /= other; this->roll /= other; return *this; }

				frotator& operator =  (const frotator& other) { this->pitch = other.pitch; this->yaw = other.yaw; this->roll = other.roll; return *this; }
				frotator& operator += (const frotator& other) { this->pitch += other.pitch; this->yaw += other.yaw; this->roll += other.roll; return *this; }
				frotator& operator -= (const frotator& other) { this->pitch -= other.pitch; this->yaw -= other.yaw; this->roll -= other.roll; return *this; }
				frotator& operator *= (const frotator& other) { this->pitch *= other.pitch; this->yaw *= other.yaw; this->roll *= other.roll; return *this; }
				frotator& operator /= (const frotator& other) { this->pitch /= other.pitch; this->yaw /= other.yaw; this->roll /= other.roll; return *this; }

				operator bool()
				{
					return bool(this->pitch && this->yaw && this->roll);
				}

				friend bool operator==(const frotator& first, const frotator& second)
				{
					return first.pitch == second.pitch && first.yaw == second.yaw && first.roll == second.roll;
				}

				friend bool operator!=(const frotator& first, const frotator& second)
				{
					return !(first == second);
				}

			public:
				double pitch = 0.f;
				double yaw = 0.f;
				double roll = 0.f;
			};

			class fvector2d {
			public:
				fvector2d() : x(0.f), y(0.f) {}
				fvector2d(double x, double y) : x(x), y(y) {}

				fvector2d operator + (const fvector2d& other) const { return { this->x + other.x, this->y + other.y }; }
				fvector2d operator - (const fvector2d& other) const { return { this->x - other.x, this->y - other.y }; }
				fvector2d operator * (double scalar) const { return { this->x * scalar, this->y * scalar }; }
				fvector2d operator / (double divide) const { return { this->x / divide, this->y / divide }; }

				fvector2d& operator *= (const double other) { this->x *= other; this->y *= other; return *this; }
				fvector2d& operator /= (const double other) { this->x /= other; this->y /= other; return *this; }

				fvector2d& operator =  (const fvector2d& other) { this->x = other.x; this->y = other.y; return *this; }
				fvector2d& operator += (const fvector2d& other) { this->x += other.x; this->y += other.y; return *this; }
				fvector2d& operator -= (const fvector2d& other) { this->x -= other.x; this->y -= other.y; return *this; }
				fvector2d& operator *= (const fvector2d& other) { this->x *= other.x; this->y *= other.y; return *this; }
				fvector2d& operator /= (const fvector2d& other) { this->x /= other.x; this->y /= other.y; return *this; }

				operator bool()
				{
					return bool(this->x && this->y);
				}

				friend bool operator==(const fvector2d& first, const fvector2d& second)
				{
					return first.x == second.x && first.y == second.y;
				}

				friend bool operator!=(const fvector2d& first, const fvector2d& second)
				{
					return !(first == second);
				}

			public:
				double x = 0.f;
				double y = 0.f;
			};

			class fvector {
			public:
				fvector() : x(0.f), y(0.f), z(0.f) {}
				fvector(double x, double y, double z) : x(x), y(y), z(z) {}
				fvector(const frotator& rotator) : x(rotator.pitch), y(rotator.yaw), z(rotator.roll) {}

				fvector operator + (const fvector& other) const { return { this->x + other.x, this->y + other.y, this->z + other.z }; }
				fvector operator - (const fvector& other) const { return { this->x - other.x, this->y - other.y, this->z - other.z }; }
				fvector operator * (double scalar) const { return { this->x * scalar, this->y * scalar, this->z * scalar }; }
				fvector operator / (double divide) const { return { this->x / divide, this->y / divide, this->z / divide }; }

				fvector& operator *= (const double other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
				fvector& operator /= (const double other) { this->x /= other; this->y /= other; this->z /= other; return *this; }

				fvector& operator =  (const fvector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
				fvector& operator += (const fvector& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
				fvector& operator -= (const fvector& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
				fvector& operator *= (const fvector& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
				fvector& operator /= (const fvector& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

				operator bool()
				{
					return bool(this->x && this->y && this->z);
				}

				friend bool operator==(const fvector& first, const fvector& second)
				{
					return first.x == second.x && first.y == second.y && first.z == second.z;
				}

				friend bool operator!=(const fvector& first, const fvector& second)
				{
					return !(first == second);
				}

			public:
				double x = 0;
				double y = 0;
				double z = 0;
			};

			struct flinearcolor
			{
				float r, g, b, a;
				flinearcolor() : r(0), g(0), b(0), a(0) { }
				flinearcolor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
				flinearcolor(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}
			};
			template <class type>
			struct tarray {
			public:
				tarray() { this->data = nullptr; this->count = this->max = 0; };

				type* data;
				std::int32_t count, max;

				type& operator[](int i) {
					return this->data[i];
				};

				int size() {
					return this->count;
				}

				bool valid(int i) {
					return bool(i < this->count);
				}
			};

			struct fstring : private tarray<wchar_t> {
				fstring() { };
				fstring(wchar_t* other) {
					this->max = this->count = *other ? static_cast<int>(crt::wcslen(other)) + 1 : 0;

					if (this->count)
						this->data = const_cast<wchar_t*>(other);
				};

				wchar_t* c_str() {
					return this->data;
				}

				bool valid() {
					return this->data != nullptr;
				}
			};
			enum class earesalliance : uint8_t {
				ally = 0,
				enemy = 1,
				neutral = 2,
				any = 3,
				count = 4,
				max = 5
			};
		}
	}
}