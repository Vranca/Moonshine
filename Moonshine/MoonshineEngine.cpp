#include "MoonshineEngine.h"
#include <chrono>
#include <string>
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
			//m_Renderer = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED), [](SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); });
			m_RendererNew = std::make_unique<Renderer>(m_Window.get(), w, h);
			if (!m_RendererNew)
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
	m_Run = true;

	double dFpsTimer = 0.0;
	int nFpsCounter = 0;

	OnCreate();

	/*
	volatile int fps = 0;
	double elap = 0;
	*/

	while (m_Run)
	{
		t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = t2 - t1;
		t1 = t2;
		delta = elapsedTime.count();

		/*
		elap += delta;
		fps++;
		*/

		OnUpdate(delta);
		OnRender();
		PresentRender();

		/*
		if (elap >= 1)
		{
			elap = 0;
			fps = 0;
		}
		*/
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

double moon::MoonshineEngine::Raycast(const Vector2D<double>& origin, double fRayAngle, const Map& map, char target)
{
	Vector2D<double> rayAngle(cos(fRayAngle), sin(fRayAngle));
	Vector2D<double> deltaDistance(std::abs(1.0 / rayAngle.x), std::abs(1.0 / rayAngle.y));
	Vector2D<double> distance(0.0, 0.0);

	if (rayAngle.x < 0)
		distance.x = (origin.x - int(origin.x)) * deltaDistance.x;
	else
		distance.x = ((int)origin.x + 1.0 - origin.x) * deltaDistance.x;

	if (rayAngle.y < 0)
		distance.y = (origin.y - (int)origin.y) * deltaDistance.y;
	else
		distance.y = ((int)origin.y + 1.0 - origin.y) * deltaDistance.y;

	Vector2D<int> testTile((int)origin.x, (int)origin.y);
	Vector2D<int> step;
	step.x = rayAngle.x < 0 ? -1 : 1;
	step.y = rayAngle.y < 0 ? -1 : 1;

	while (true)
	{
		if (distance.x < distance.y)
		{
			testTile.x += step.x;

			if (map.At(testTile.x, testTile.y) == target)
				return distance.x;

			distance.x += deltaDistance.x;
		}
		else
		{
			testTile.y += step.y;

			if (map.At(testTile.x, testTile.y) == target)
				return distance.y;

			distance.y += deltaDistance.y;
		}
	}
}

