#pragma once
#include<SDL.h>
#include<memory>
#include<functional>
#include<vector>



namespace moon
{
	class Renderer
	{
	public:
		Renderer(SDL_Window* window, const int& width, const int& height);
		~Renderer();

		int DrawVerticalLine(const int & x1, const int & x2, const int & y, const int & r, const int & g, const int & b);

		void Present();

	private:
		SDL_Renderer* m_Renderer;
		SDL_Texture* m_Texture;
		int m_Width;
		int m_Height;
		uint32_t* m_TextureBuffer;

	public:
		inline void PresentRender() { return SDL_RenderPresent(m_Renderer); }
		inline int SetRenderColor(const int& r, const int& g, const int& b, const int&a) { return SDL_SetRenderDrawColor(m_Renderer, r, g, b, a); }
		inline int DrawPoint(const int& x, const int& y) { return SDL_RenderDrawPoint(m_Renderer, x, y); }
		inline int DrawLine(const int& x1, const int& y1, const int& x2, const int& y2) { return SDL_RenderDrawLine(m_Renderer, x1, y1, x2, y2); }
		inline int DrawRect(const SDL_Rect& r) { return SDL_RenderDrawRect(m_Renderer, &r); }
		inline int FillRect(const SDL_Rect& r) { return SDL_RenderFillRect(m_Renderer, &r); }
		inline int ClearRenderer() { return SDL_RenderClear(m_Renderer); }

		int FillVerticalGradientRect(const SDL_Rect& r, const SDL_Color& c1, const SDL_Color& c2)
		{
			double nStepRInit = (double)(c1.r - c2.r) / (double)r.h;
			double nStepGInit = (double)(c1.g - c2.g) / (double)r.h;
			double nStepBInit = (double)(c1.b - c2.b) / (double)r.h;
			double nStepR = 0.0;
			double nStepG = 0.0;
			double nStepB = 0.0;

			for (int y = r.y; y < r.y + r.h; y++)
			{
				SDL_SetRenderDrawColor(m_Renderer, (int)(nStepR), (int)(nStepG), (int)(nStepB), c1.a);
				SDL_RenderDrawLine(m_Renderer, r.x, y, r.w, y);
				nStepR += nStepRInit;
				nStepG += nStepGInit;
				nStepB += nStepBInit;
			}

			return 0;
		}
	};
}
