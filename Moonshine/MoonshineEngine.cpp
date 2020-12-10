#include "MoonshineEngine.h"
#include <chrono>
#include "Camera.h"
#include "Map.h"



moon::MoonshineEngine::MoonshineEngine()
{
}


moon::MoonshineEngine::~MoonshineEngine()
{
}

bool moon::MoonshineEngine::Construct(const char * title, int w, int h, int x, int y, Uint32 flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}
	else
	{
		m_Window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>(SDL_CreateWindow(title, x, y, w, h, flags), [](SDL_Window* window) { SDL_DestroyWindow(window); });
		if (!m_Window)
		{
			return false;
		}
		else
		{
			m_Renderer = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED), [](SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); });
			if (!m_Renderer)
			{
				return false;
			}
		}
	}
	m_Width = w;
	m_Height = h;

	return true;
}

void moon::MoonshineEngine::Start()
{
	auto t1 = std::chrono::steady_clock::now();
	auto t2 = t1;
	double delta = 0.0;

	OnCreate();

	while (true)
	{
		t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = t2 - t1;
		t1 = t2;
		delta = elapsedTime.count();

		OnUpdate(delta);
		OnRender();
	}

	Quit();
}

void moon::MoonshineEngine::Quit()
{
	//SDL_DestroyRenderer(m_Renderer.get());
	m_Renderer.reset();
	//SDL_DestroyWindow(m_Window.get());
	m_Window.reset();
	SDL_Quit();
}

void moon::MoonshineEngine::DrawRaycast(const moon::Camera& cam, const moon::Map& map)
{
	for (int x = 0; x < m_Width; x++)
	{
		double fRayAngle = (cam.GetDirectionAngle() - cam.GetFOV() / 2.0) + ((double)x / (double)GetWidth()) * cam.GetFOV();
		Vector2D<double> rayAngle(cos(fRayAngle), sin(fRayAngle));
		Vector2D<double> deltaDistance(std::abs(1.0 / rayAngle.x), std::abs(1.0 / rayAngle.y));
		Vector2D<double> wallDistance(0.0, 0.0);
		Vector2D<double> initDistance(0.0, 0.0);

		if (rayAngle.x < 0)
			initDistance.x = (cam.GetCoordinates().x - int(cam.GetCoordinates().x)) * deltaDistance.x;
		else
			initDistance.x = ((int)cam.GetCoordinates().x + 1.0 - cam.GetCoordinates().x) * deltaDistance.x;

		if (rayAngle.y < 0)
			initDistance.y = (cam.GetCoordinates().y - (int)cam.GetCoordinates().y) * deltaDistance.y;
		else
			initDistance.y = ((int)cam.GetCoordinates().y + 1.0 - cam.GetCoordinates().y) * deltaDistance.y;

		double fDistanceToWall = CalculateDistance(cam, initDistance, deltaDistance, rayAngle,map, '#') * cos(cam.GetDirectionAngle() - fRayAngle);

		RenderRaycast(x, fDistanceToWall);
	}
}

double moon::MoonshineEngine::CalculateDistance(const moon::Camera& cam, Vector2D<double> initDistance, Vector2D<double> deltaDistance, const Vector2D<double>& rayAngle, const moon::Map& map, const char target)
{
	Vector2D<double> distance(initDistance.x, initDistance.y);
	bool bHitTarget = false;
	int nTestX;
	int nTestY;
	int stepX = rayAngle.x < 0 ? -1 : 1;
	int stepY = rayAngle.y < 0 ? -1 : 1;
	int tileX = (int)cam.GetCoordinates().x;
	int tileY = (int)cam.GetCoordinates().y;

	while (!bHitTarget)
	{
		if (distance.x < distance.y)
		{
			tileX += stepX;

			if (map.At(tileX, tileY) == target)
				return distance.x;

			distance.x += deltaDistance.x;
		}
		else
		{
			tileY += stepY;

			if (map.At(tileX, tileY) == target)
				return distance.y;

			distance.y += deltaDistance.y;
		}
	}
}

void moon::MoonshineEngine::RenderRaycast(const int & x, const double & fDistanceToWall)
{
	int wallShading = 160 - fDistanceToWall * 10;
	wallShading = wallShading < 30 ? 30 : wallShading;
	int top = (double)(GetHeight() / 2.0) - (double)GetHeight() / ((double)fDistanceToWall);
	int bottom = GetHeight() - top;

	SetRenderColor(wallShading, wallShading, wallShading, 0);
	DrawLine(x, top, x, bottom);
}
