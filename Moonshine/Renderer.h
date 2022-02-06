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
		Renderer(SDL_Window* window, int width, int height);
		~Renderer();

		void Present();

	private:
		SDL_Renderer* m_Renderer;
		SDL_Texture* m_Texture;
		int m_Width;
		int m_Height;
		uint32_t* m_TextureBuffer;

	public:
		void DrawPoint(int x, int y, uint32_t argb);
		void DrawLine(int x1, int y1, int x2, int y2, uint32_t argb);
		void DrawRect(const Rect& r, uint32_t argb);
		void FillRect(const Rect& r, uint32_t argb);
		void FillVerticalGradientRect(const Rect& r, uint32_t argb0, uint32_t argb1);
		void Clear();
		void Clear(uint32_t argb);

	private:
		void DrawLineLow(int x0, int y0, int x1, int y1, uint32_t argb);
		void DrawLineHigh(int x0, int y0, int x1, int y1, uint32_t argb);
		void DrawVerticalLine(int y0, int y1, int x, uint32_t argb);
		void DrawHorizontalLine(int x0, int x1, int y, uint32_t argb);
	};
}
