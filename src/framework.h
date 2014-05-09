/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	Here we define all the mathematical classes like Vector3, Matrix44 and some extra useful geometrical functions
*/

#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#include <vector>
#include <cmath>
#include <cstdlib>
#include <stdint.h>

#ifdef WIN32
	#define M_PI_2 (PI/2.)
#endif

#ifndef PI
#define PI 3.14159265359
#endif
#define DEG2RAD 0.0174532925

#ifdef DOUBLE_PRECISION
typedef double Float;
#else
typedef float Float;
#endif //DOUBLE_PRECISION

//save us some typing
typedef unsigned int Uint;

template<typename T>
inline T clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }

Uint isqrt(Uint root);

class Vector3;

//Color class to store colors in unsigned byte
class Color
{
public:
	union
	{
		struct { unsigned char r;
				 unsigned char g;
				 unsigned char b; };
		unsigned char v[3];
	};
	Color() { r = g = b = 0; }
	Color(Float r, Float g, Float b) { this->r = (unsigned char)r; this->g = (unsigned char)g; this->b = (unsigned char)b; }
	void operator = (const Vector3& v);

	void set(Float r, Float g, Float b) { this->r = (unsigned char)clamp<Float>(r,0.0,255.0); this->g = (unsigned char)clamp<Float>(g,0.0,255.0); this->b = (unsigned char)clamp<Float>(b,0.0,255.0); }
	void random() { r = rand() % 255; g = rand() % 255; b = rand() % 255; }

	Color operator * (Float v) { return Color((unsigned char)(r*v), (unsigned char)(g*v), (unsigned char)(b*v)); }
	void operator *= (Float v) { r = (unsigned char)(r * v); g = (unsigned char)(g * v); b = (unsigned char)(b * v); }
	Color operator / (Float v) { return Color((unsigned char)(r/v), (unsigned char)(g/v), (unsigned char)(b/v)); }
	void operator /= (Float v) { r = (unsigned char)(r / v); g = (unsigned char)(g / v); b = (unsigned char)(b / v); }
	Color operator + (const Color& v) { return Color((Float)(r+v.r), (Float)(g+v.g), (Float)(b+v.b) ); }
	void operator += (const Color& v) { r += v.r; g += v.g; b += v.b; }
	Color operator - (const Color& v) { return Color((Float)(r-v.r), (Float)(g-v.g), (Float)(b-v.b)); }
	void operator -= (const Color& v) { r -= v.r; g -= v.g; b -= v.b; }
	bool operator != (const Color &c) { return (r!=c.r || g!=c.g || b!=c.b); }
	bool operator == (const Color &c) { return (r==c.r && g==c.g && b==c.b); }

	//some colors to help
	static const Color WHITE;
	static const Color BLACK;
	static const Color GRAY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color PURPLE;

	Color getComplementary();
};

inline Color operator * (const Color& c, Float v) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
inline Color operator * (Float v, const Color& c) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
//*********************************

class Vector3
{
public:
	union
	{
		struct { Float x,y,z; };
		Float v[3];
	};

	Vector3() { x = y = z = 0.0f; }
	Vector3(Float x, Float y, Float z) { this->x = x; this->y = y; this->z = z;	}

	Float length();
	Float length() const;

	void set(Float x, Float y, Float z) { this->x = x; this->y = y; this->z = z; }

	Vector3& normalize();
	void random(Float range);
	void random(Vector3 range);

	Float distance(const Vector3& v) const;

	Vector3 cross( const Vector3& v ) const;
	Float dot( const Vector3& v ) const;
};

//****************************
//Matrix44 class
class Matrix44
{
	public:

		//This matrix works in
		union { //allows to access the same var using different ways
			struct
			{
				Float        _11, _12, _13, _14;
				Float        _21, _22, _23, _24;
				Float        _31, _32, _33, _34;
				Float        _41, _42, _43, _44;
			};
			Float M[4][4]; //[row][column]
			Float m[16];
		};

		Matrix44();
		Matrix44(const Float* v);

		void set(); //multiply with opengl matrix
		void load(); //load in opengl matrix
		void clear();
		void setIdentity();
		void transpose();

		//get base vectors
		Vector3 rightVector() { return Vector3(m[0],m[1],m[2]); }
		Vector3 topVector() { return Vector3(m[4],m[5],m[6]); }
		Vector3 frontVector() { return Vector3(m[8],m[9],m[10]); }

		bool inverse();
		void setUpAndOrthonormalize(Vector3 up);
		void setFrontAndOrthonormalize(Vector3 front);

		Matrix44 getRotationOnly(); //used when having scale

		//rotate only
		Vector3 rotateVector( const Vector3& v);

		//transform using world coordinates
		void traslate(Float x, Float y, Float z);
		void rotate( Float angle_in_rad, const Vector3& axis  );

		//transform using local coordinates
		void traslateLocal(Float x, Float y, Float z);
		void rotateLocal( Float angle_in_rad, const Vector3& axis  );

		//create a transformation matrix from scratch
		void setTranslation(Float x, Float y, Float z);
		void setRotation( Float angle_in_rad, const Vector3& axis );

		bool getXYZ(Float* euler) const;

		Matrix44 operator * (const Matrix44& matrix) const;
};

//Operators, they are our friends
//Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, Float v);

class Vector2
{
public:
	union
	{
		struct { Float x,y; };
		Float value[2];
	};

	Vector2() { x = y = 0.0f; }
	Vector2(Float x, Float y) { this->x = x; this->y = y; }

	Float length() { return sqrt(x*x + y*y); }
	Float length() const { return sqrt(x*x + y*y); }

	Float dot( const Vector2& v );
	Float perpdot( const Vector2& v );

	void set(Float x, Float y) { this->x = x; this->y = y; }

	Vector2& normalize() { *this *= (Float)length(); return *this; }

	Float distance(const Vector2& v);
	void random(Float range);

	void operator *= (Float v) { x*=v; y*=v; }
};

Vector2 operator * (const Vector2& a, Float v);
Vector2 operator + (const Vector2& a, const Vector2& b);
Vector2 operator - (const Vector2& a, const Vector2& b);

inline Float distance(const Vector2& a, const Vector2& b) { return (Float)(a-b).length(); }
#ifdef DOUBLE_PRECISION
inline double distance(double x, double y, double x2, double y2) { return sqrt( (x-x2)*(x-x2) + (y-y2)*(y-y2)); }
#else
inline float distance(float x, float y, float x2, float y2) { return sqrtf( (x-x2)*(x-x2) + (y-y2)*(y-y2)); }
#endif //DOUBLE_PRECISION


class Vector3u
{
public:
	union
	{
		struct { unsigned int x;
				 unsigned int y;
				 unsigned int z; };
		struct { unsigned int r;
				 unsigned int g;
				 unsigned int b; };
		unsigned int v[3];
	};
	Vector3u() { x = y = z = 0; }
	Vector3u(unsigned int x, unsigned int y, unsigned int z) { this->x = x; this->y = y; this->z = z; }

	void operator *= (Float v) { x = (unsigned int)(x * v); y = (unsigned int)(y * v); z = (unsigned int)(z * v); }
	Vector3u operator / (Float v) { return Vector3u((unsigned int)(r/v), (unsigned int)(g/v), (unsigned int)(b/v)); }
	void operator /= (Float v) { x = (unsigned int)(x / v); y = (unsigned int)(y / v); z = (unsigned int)(z / v); }
	Vector3u operator + (const Vector3u& v) { return Vector3u(x+v.x, y+v.y, z+v.z); }
	void operator += (const Vector3u& v) { x += v.x; y += v.y; z += v.z; }
	Vector3u operator - (const Vector3u& v) { return Vector3u(x-v.x, y-v.y, z-v.z); }
	void operator -= (const Vector3u& v) { x -= v.x; y -= v.y; z -= v.z; }
};

inline Vector3u operator * (Float v, const Vector3u& c) { return Vector3u((unsigned int)(c.x*v), (unsigned int)(c.y*v), (unsigned int)(c.z*v)); }

Float ComputeSignedAngle( Vector2 a, Vector2 b);
Vector3 RayPlaneCollision( const Vector3& plane_pos, const Vector3& plane_normal, const Vector3& ray_origin, const Vector3& ray_dir );

//position
class Pos2
{
public:
	union
	{
		struct { Uint x,y; };
		Uint v[2];
	};
	Pos2() { x = y = 0; }
	Pos2(Uint _x, Uint _y) { x=_x; y=_y; }
	inline void set(Uint _x, Uint _y) { x=_x; y=_y; }
};

#endif
