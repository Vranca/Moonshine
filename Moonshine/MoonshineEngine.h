#pragma once
#include <thread>
#include <SDL.h>
#include "Camera.h"
#include "Map.h"
#include <functional>

namespace moon 
{
	using Rect = SDL_Rect;
	using Color = SDL_Color;

	class MoonshineEngine
	{
	public:
		virtual bool OnCreate() = 0;
		virtual bool OnUpdate(const double& delta) = 0;
		virtual bool OnRender() = 0;

		MoonshineEngine();
		virtual ~MoonshineEngine();

		bool Construct(const char* title, int w, int h, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED, Uint32 flags = SDL_WINDOW_SHOWN);
		void Start();
		inline void Stop() { m_Run = false; }
		void Quit();

		//
		// Raycast from origin point with given angle.
		// Returns distance from origin to target.
		//
		double Raycast(const Vector2D<double>& origin, const double& fRayAngle, const moon::Map& map, const char target);

	private:
		std::thread m_mainThread;
		std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> m_Window;
		std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>> m_Renderer;
		int m_Width;
		int m_Height;
		bool m_Run;

	public:
		inline int GetWidth()	{	return m_Width;		}
		inline int GetHeight()	{	return m_Height;	}

		inline void PresentRender()														{	return SDL_RenderPresent(m_Renderer.get());								}
		inline int SetRenderColor(const int& r, const int& g, const int& b, const int&a){	return SDL_SetRenderDrawColor(m_Renderer.get(), r, g, b, a);			}
		inline int DrawPoint(const int& x, const int& y)								{	return SDL_RenderDrawPoint(m_Renderer.get(), x, y);						}
		inline int DrawLine(const int& x1, const int& y1, const int& x2, const int& y2) {	return SDL_RenderDrawLine(m_Renderer.get(), x1, y1, x2, y2);			}
		inline int DrawRect(const Rect& r)												{	return SDL_RenderDrawRect(m_Renderer.get(), &r);						}
		inline int FillRect(const Rect& r)												{	return SDL_RenderFillRect(m_Renderer.get(), &r);						}
		inline int ClearRenderer()														{	return SDL_RenderClear(m_Renderer.get());								}

		int FillVerticalGradientRect(const Rect& r, const Color& c1, const Color& c2)
		{
			double nStepRInit = (double)(c1.r - c2.r) / (double)r.h;
			double nStepGInit = (double)(c1.g - c2.g) / (double)r.h;
			double nStepBInit = (double)(c1.b - c2.b) / (double)r.h;
			double nStepR = 0.0;
			double nStepG = 0.0;
			double nStepB = 0.0;

			for (int y = r.y; y < r.y + r.h; y++)
			{
				SDL_SetRenderDrawColor(m_Renderer.get(), (int)(nStepR), (int)(nStepG), (int)(nStepB), c1.a);
				SDL_RenderDrawLine(m_Renderer.get(), r.x, y, r.w, y);
				nStepR += nStepRInit;
				nStepG += nStepGInit;
				nStepB += nStepBInit;
			}

			return 0;
		}
	};
}
