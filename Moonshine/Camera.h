#pragma once
#include "Vector2D.h"

namespace moon
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		double rotateLeft(double delta);
		double rotateRight(double delta);
		double rotate(double delta, double rotation);
		Vector2D<double> moveForward(double delta);
		Vector2D<double> moveBack(double delta);

	private:
		Vector2D<double> m_coordinates;
		double m_directionAngle;
		double m_FOV;
		double m_rotationSpeed;
		double m_movementSpeed;

	public:
		inline const Vector2D<double>& GetCoordinates() const { return m_coordinates; }
		inline double GetDirectionAngle() const { return m_directionAngle; }
		inline double GetFOV() const { return m_FOV; }
		inline void SetCoordinates(double x, double y) { m_coordinates.x = x; m_coordinates.y = y; }
	};
}

