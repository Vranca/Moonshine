#include "Camera.h"
#include <iostream>



moon::Camera::Camera()
	: m_coordinates(0.0, 0.0), m_directionAngle(30.0), m_FOV(3.14159 / 3), m_movementSpeed(10.0), m_rotationSpeed(2.0)
{
}

moon::Camera::~Camera()
{
}

double moon::Camera::rotateLeft(const double& delta)
{
	m_directionAngle -= m_rotationSpeed * delta;
	return m_directionAngle;
}

double moon::Camera::rotateRight(const double& delta)
{
	m_directionAngle += m_rotationSpeed * delta;
	return m_directionAngle;
}

Vector2D<double> moon::Camera::moveForward(const double& delta)
{
	m_coordinates.x += cos(m_directionAngle) * m_movementSpeed * delta;
	m_coordinates.y += sin(m_directionAngle) * m_movementSpeed * delta;
	return m_coordinates;
}

Vector2D<double> moon::Camera::moveBack(const double& delta)
{
	m_coordinates.x -= cos(m_directionAngle) * m_movementSpeed * delta;
	m_coordinates.y -= sin(m_directionAngle) * m_movementSpeed * delta;
	return m_coordinates;
}
