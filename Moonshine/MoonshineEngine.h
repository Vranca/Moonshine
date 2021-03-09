#pragma once
#include <thread>
#include <SDL.h>
#include "Camera.h"
#include"Renderer.h"
#include "Map.h"


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
		std::unique_ptr<Renderer> m_RendererNew;
		
		int m_Width;
		int m_Height;
		bool m_Run;

	public:
		inline int GetWidth()	{	return m_Width;		}
		inline int GetHeight()	{	return m_Height;	}

		inline void SetTitle(const char* title) { SDL_SetWindowTitle(m_Window.get(), title); }
		inline void PresentRender()																				{	m_RendererNew->Present();									}
		inline void DrawPoint(const int& x, const int& y, const uint32_t& argb)									{	m_RendererNew->DrawPoint(x, y, argb);						}
		inline void DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const uint32_t& argb)	{	m_RendererNew->DrawLine(x1, y1, x2, y2, argb);				}
		inline void DrawRect(const Rect& r, const uint32_t& argb)												{	m_RendererNew->DrawRect(r, argb);							}
		inline void FillRect(const Rect& r, const uint32_t& argb)												{	m_RendererNew->FillRect(r, argb);							}
		inline void ClearRenderer()																				{	m_RendererNew->Clear();										}
		inline void ClearRenderer(const uint32_t& argb)															{	m_RendererNew->Clear(argb);									}
		inline void FillVerticalGradientRect(const Rect& r, const uint32_t& argb0, const uint32_t& argb1)		{	m_RendererNew->FillVerticalGradientRect(r, argb0, argb1);	}
	};
}
