#pragma once
#include<SDL.h>
#include<memory>
#include<functional>
#include<vector>



namespace moon
{
	using Rect = SDL_Rect;
	using Color = SDL_Color;

	class Renderer
	{
	public:
		Renderer(SDL_Window* window, const int& width, const int& height);
		~Renderer();

		void Present();

	private:
		SDL_Renderer* m_Renderer;
		SDL_Texture* m_Texture;
		int m_Width;
		int m_Height;
		uint32_t* m_TextureBuffer;

	public:
		void DrawPoint(const int& x, const int& y, const uint32_t& argb);
		void DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const uint32_t& argb);
		void DrawRect(const Rect& r, const uint32_t& argb);
		void FillRect(const Rect& r, const uint32_t& argb);
		void FillVerticalGradientRect(const Rect& r, const uint32_t& argb0, const uint32_t& argb1);
		void Clear();
		void Clear(const uint32_t& argb);

	private:
		void DrawLineLow(const int& x0, const int& y0, const int& x1, const int& y1, const uint32_t& argb);
		void DrawLineHigh(const int& x0, const int& y0, const int& x1, const int& y1, const uint32_t& argb);
		void DrawVerticalLine(const int & y0, const int & y1, const int & x, const uint32_t & argb);
		void DrawHorizontalLine(const int & x0, const int & x1, const int & y, const uint32_t & argb);
	};
}
