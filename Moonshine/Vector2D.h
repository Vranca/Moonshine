#pragma once

template <typename T>
struct Vector2D
{
public:
	T x;
	T y;

	Vector2D() {}
	Vector2D(T x, T y) : x(x), y(y) {}
	Vector2D(const Vector2D<T>& copy) : x(copy.x), y(copy.y) {}
		
	~Vector2D() {}

	T magnitude()
	{
		return sqrt(x * x + y * y);
	}

	T dot(const Vector2D<T>& rhs)
	{
		return (this->x * rhs.x + this->y * rhs.y);
	}

	//
	// Operator overloading
	//
	//
	// Vector operations
	//
	//***********************************************
	Vector2D<T>& operator+= (const Vector2D<T>& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vector2D<T>& operator-= (const Vector2D<T>& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	//***********************************************

	//
	// Scalar operations
	//
	//***********************************************
	Vector2D<T>& operator+= (const T& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vector2D<T>& operator-= (const T& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	Vector2D<T>& operator*= (const T& rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		return *this;
	}

	Vector2D<T>& operator/= (const T& rhs)
	{
		this->x /= rhs.x;
		this->y /= rhs.y;
		return *this;
	}

	//***********************************************
};

//
// Operator overloading
//
//
// Vector operations
//
//***********************************************
template <typename T>
inline Vector2D<T> operator+ (const Vector2D<T>& v1, const Vector2D<T>& v2)
{
	return Vector2D<T>((v1.x + v2.x), (v1.y + v2.y));
}

template <typename T>
inline Vector2D<T> operator- (const Vector2D<T>& v1, const Vector2D<T>& v2)
{
	return Vector2D<T>((v1.x - v2.x), (v1.y - v2.y));
}
//***********************************************

//
// Scalar operations
//
//***********************************************
template <typename T>
inline Vector2D<T> operator+ (const Vector2D<T>& v, const T& s)
{
	return Vector2D<T>((v.x + s), (v.y + s));
}

template <typename T>
inline Vector2D<T> operator- (const Vector2D<T>& v, const T& s)
{
	return Vector2D<T>((v.x - s), (v.y - s));
}

template <typename T>
inline Vector2D<T> operator* (const Vector2D<T>& v, const T& s)
{
	return Vector2D<T>((v.x * s), (v.y * s));
}

template <typename T>
inline Vector2D<T> operator/ (const Vector2D<T>& v, const T& s)
{
	return Vector2D<T>((v.x / s), (v.y / s));
}

//***********************************************