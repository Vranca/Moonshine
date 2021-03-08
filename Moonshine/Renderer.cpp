#include "Renderer.h"


namespace moon
{
	Renderer::Renderer(SDL_Window* window, const int& width, const int& height)
		: m_Width(width), m_Height(height)
	{
		m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
		m_TextureBuffer = new uint32_t[width * height];
	}


	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyTexture(m_Texture);

		delete m_Renderer;
		delete m_Texture;
	}

	int Renderer::DrawVerticalLine(const int & x1, const int & x2, const int & y, const int & r, const int & g, const int & b)
	{
		for (int point = x1; point <= x2; point++)
		{
			m_TextureBuffer[y * m_Width + point] = 0xFF000000 | (r << 16) | (g << 8) | b;
		}

		return 0;
	}
	void Renderer::Present()
	{
		SDL_UpdateTexture(m_Texture, NULL, m_TextureBuffer, m_Width * sizeof(uint32_t));
		SDL_RenderPresent(m_Renderer);
	}
}
