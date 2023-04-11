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

			struct fvector2d
			{
				float x, y;

				fvector2d() : x(0), y(0) {}

				fvector2d(float x, float y) : x(x), y(y) {}

				fvector2d operator + (const fvector2d& other) const { return fvector2d(x + other.x, y + other.y); }

				fvector2d operator- (const fvector2d& other) const { return fvector2d(x - other.x, y - other.y); }

				fvector2d operator* (float scalar) const { return fvector2d(x * scalar, y * scalar); }

				fvector2d& operator=  (const fvector2d& other) { x = other.x; y = other.y; return *this; }

				fvector2d& operator+= (const fvector2d& other) { x += other.x; y += other.y; return *this; }

				fvector2d& operator-= (const fvector2d& other) { x -= other.x; y -= other.y; return *this; }

				fvector2d& operator*= (const float other) { x *= other; y *= other; return *this; }

				friend bool operator==(const fvector2d& one, const fvector2d& two) { return one.x == two.x && one.y == two.y; }

				friend bool operator!=(const fvector2d& one, const fvector2d& two) { return !(one == two); }

				friend bool operator>(const fvector2d& one, const fvector2d& two) { return one.x > two.x && one.y > two.y; }

				friend bool operator<(const fvector2d& one, const fvector2d& two) { return one.x < two.x&& one.y < two.y; }

				bool is_valid()
				{
					if (x == 0 || y == 0)
						return false;
					else
						return true;
				}

			};

			struct fvector
			{
				float x, y, z;

				fvector() : x(0.f), y(0.f), z(0.f) {}

				fvector(float x, float y, float z) : x(x), y(y), z(z) {}

				fvector(float InF) : x(InF), y(InF), z(InF) { }

				float Size() const { return sqrtf(x * x + y * y + z * z); }

				float Sum() const { return x + y + z; }

				float Size2D() const { return sqrtf(x * x + y * y); }

				float SizeSquared() const { return x * x + y * y + z * z; }

				float DistTo(const fvector& V) const { return (*this - V).Size(); }

				fvector operator+(const fvector& other) const { return fvector(x + other.x, y + other.y, z + other.z); }

				fvector operator-(const fvector& other) const { return fvector(x - other.x, y - other.y, z - other.z); }

				fvector operator*(const fvector& V) const { return fvector(x * V.x, y * V.y, z * V.z); }

				fvector operator/(const fvector& V) const { return fvector(x / V.x, y / V.y, z / V.z); }

				bool operator==(const fvector& V) const { return x == V.x && y == V.y && z == V.z; }

				bool operator!=(const fvector& V) const { return x != V.x || y != V.y || z != V.z; }

				fvector operator-() const { return fvector(-x, -y, -z); }

				fvector operator+(float Bias) const { return fvector(x + Bias, y + Bias, z + Bias); }

				fvector operator-(float Bias) const { return fvector(x - Bias, y - Bias, z - Bias); }

				fvector operator*(float Scale) const { return fvector(x * Scale, y * Scale, z * Scale); } 

				fvector operator/(float Scale) const { const float RScale = 1.f / Scale; return fvector(x * RScale, y * RScale, z * RScale); }

				fvector operator=(const fvector& V) { x = V.x; y = V.y; z = V.z; return *this; }

				fvector operator+=(const fvector& V) { x += V.x; y += V.y; z += V.z; return *this; }

				fvector operator-=(const fvector& V) { x -= V.x; y -= V.y; z -= V.z; return *this; }

				fvector operator*=(const fvector& V) { x *= V.x; y *= V.y; z *= V.z; return *this; }

				fvector operator/=(const fvector& V) { x /= V.x; y /= V.y; z /= V.z; return *this; }

				fvector operator*=(float Scale) { x *= Scale; y *= Scale; z *= Scale; return *this; }

				fvector operator/=(float V) { const float RV = 1.f / V; x *= RV; y *= RV; z *= RV; return *this; }

				float operator|(const fvector& V) const { return x * V.x + y * V.y + z * V.z; }

				fvector operator^(const fvector& V) const { return fvector(y * V.z - z * V.y, z * V.x - x * V.z, x * V.y - y * V.x); }

				inline float Distance(fvector v) {
					return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
				}

				bool is_valid()
				{
					if (x == 0 || y == 0)
						return false;
					else
						return true;
				}

				static const fvector ZeroVector;

				static const fvector OneVector;
			};
			struct alignas(16) FPlane : public fvector {
				float W;
			};
			enum class EAresAlliance : uint8_t {
				Alliance_Ally = 0,
				Alliance_Enemy = 1,
				Alliance_Neutral = 2,
				Alliance_Any = 3,
				Alliance_Count = 4,
				Alliance_MAX = 5
			};

			struct fmatrix {
				struct FPlane XPlane;
				struct FPlane YPlane;
				struct FPlane ZPlane;
				struct FPlane WPlane;
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

			class fstring : public tarray<wchar_t>
			{
			public:
				inline fstring()
				{
				};

				fstring(const wchar_t* other)
				{
					max = count = *other ? static_cast<int>(std::wcslen(other)) + 1 : 0;

					if (count)
					{
						data = const_cast<wchar_t*>(other);
					}
				};
				fstring(const wchar_t* other, int count)
				{
					data = const_cast<wchar_t*>(other);;
					max = count = count;
				};

				inline bool is_valid() const
				{
					return data != nullptr;
				}

				inline const wchar_t* wide() const
				{
					return data;
				}

				int multi(char* name, int size) const
				{
					return WideCharToMultiByte(CP_UTF8, 0, data, count, name, size, nullptr, nullptr) - 1;
				}

				std::string to_str() const
				{
					auto length = std::wcslen(data);

					std::string str(length, '\0');

					std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(data, data + length, '?', &str[0]);

					return str;
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