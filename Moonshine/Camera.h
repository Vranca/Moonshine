#pragma once
#include "Vector2D.h"

namespace moon
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		double rotateLeft(const double& delta);
		double rotateRight(const double& delta);
		Vector2D<double> moveForward(const double& delta);
		Vector2D<double> moveBack(const double& delta);

	private:
		Vector2D<double> m_coordinates;
		double m_directionAngle;
		double m_FOV;
		double m_rotationSpeed;
		double m_movementSpeed;

	public:
		inline const Vector2D<double>& GetCoordinates() const { return m_coordinates; }
		inline const double& GetDirectionAngle() const { return m_directionAngle; }
		inline const double& GetFOV() const { return m_FOV; }
		inline void SetCoordinates(const double& x, const double& y) { m_coordinates.x = x; m_coordinates.y = y; }
	};
}

