#include "Renderer.h"
#include <stdint.h>


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

		delete m_TextureBuffer;
	}

	void Renderer::Present()
	{
		SDL_UpdateTexture(m_Texture, NULL, m_TextureBuffer, m_Width * sizeof(uint32_t));
		SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);
		SDL_RenderPresent(m_Renderer);
	}

	void Renderer::DrawPoint(const int & x, const int & y, const uint32_t & argb)
	{
		m_TextureBuffer[x + y * m_Width] = argb;
	}

	void Renderer::DrawLine(const int & x0, const int & y0, const int & x1, const int & y1, const uint32_t& argb)
	{
		if (y0 == y1)
		{
			if (x0 < x1)
				DrawHorizontalLine(x0, x1, y0, argb);
			else
				DrawHorizontalLine(x1, x0, y0, argb);
		}
		else if (x0 == x1)
		{
			if (y0 < y1)
				DrawVerticalLine(y0, y1, x0, argb);
			else
				DrawVerticalLine(y1, y0, x0, argb);
		}
		else if (std::abs(y1 - y0) < std::abs(x1 - x0))
		{
			if (x0 > x1)
			{
				DrawLineLow(x1, y1, x0, y0, argb);
			}
			else
			{
				DrawLineLow(x0, y0, x1, y1, argb);
			}
		}
		else
		{
			if (y0 > y1)
			{
				DrawLineHigh(x1, y1, x0, y0, argb);
			}
			else
			{
				DrawLineHigh(x0, y0, x1, y1, argb);
			}
		}
	}

	void Renderer::DrawRect(const Rect & r, const uint32_t & argb)
	{
		for (int x = r.x; x < r.x + r.w; x++)
		{
			DrawPoint(x, r.y, argb);
			DrawPoint(x, r.y + r.h, argb);
		}
		for (int y = r.y; y < r.y + r.h; y++)
		{
			DrawPoint(r.x, y, argb);
			DrawPoint(r.x + r.w, y, argb);
		}
	}

	void Renderer::FillRect(const Rect & r, const uint32_t & argb)
	{
		for (int y = r.y; y < r.y + r.h; y++)
			for (int x = r.x; x < r.x + r.w; x++)
				DrawPoint(x, y, argb);
	}

	void Renderer::FillVerticalGradientRect(const Rect & r, const uint32_t & argb0, const uint32_t & argb1)
	{
		double nStepRInit = (double)((argb1 >> 16 & 0x000000FF) - (argb0 >> 16 & 0x000000FF)) / (double)r.h;
		double nStepGInit = (double)((argb1 >> 8 & 0x000000FF) - (argb0 >> 8 & 0x000000FF)) / (double)r.h;
		double nStepBInit = (double)((argb1 & 0x000000FF) - (argb0 & 0x000000FF)) / (double)r.h;
		double nStepR = 0.0;
		double nStepG = 0.0;
		double nStepB = 0.0;

		for (int y = r.y; y < r.y + r.h; y++)
		{
			DrawHorizontalLine(r.x, r.x + r.w, y, (argb0 + ((uint32_t)nStepR << 16) + ((uint32_t)nStepG << 8) + ((uint32_t)nStepB)));
			nStepR += nStepRInit;
			nStepG += nStepGInit;
			nStepB += nStepBInit;
		}
	}

	void Renderer::Clear()
	{
		uint32_t* pixel = m_TextureBuffer;
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{ 
				*(pixel) = 0;
				pixel++;
			}
	}

	void Renderer::Clear(const uint32_t & argb)
	{
		uint32_t* pixel = m_TextureBuffer;
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				*(pixel) = argb;
				pixel++;
			}
	}

	void Renderer::DrawLineLow(const int & x0, const int & y0, const int & x1, const int & y1, const uint32_t& argb)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int yi = 1;

		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		int d = (2 * dy) - dx;
		int y = y0;

		for (int x = x0; x <= x1; x++)
		{
			DrawPoint(x, y, argb);

			if (d > 0)
			{
				y += yi;
				d += (2 * (dy - dx));
			}
			else
			{
				d += (2 * dy);
			}
		}
	}
	void Renderer::DrawLineHigh(const int & x0, const int & y0, const int & x1, const int & y1, const uint32_t& argb)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int xi = 1;

		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		int d = (2 * dx) - dy;
		int x = x0;

		for (int y = y0; y <= y1; y++)
		{
			DrawPoint(x, y, argb);

			if (d > 0)
			{
				x += xi;
				d += (2 * (dx - dy));
			}
			else
			{
				d += (2 * dx);
			}
		}
	}
	void Renderer::DrawVerticalLine(const int & y0, const int & y1, const int & x, const uint32_t & argb)
	{
		for (int y = y0; y <= y1; y++)
		{
			DrawPoint(x, y, argb);
		}
	}
	void Renderer::DrawHorizontalLine(const int & x0, const int & x1, const int & y, const uint32_t & argb)
	{
		for (int x = x0; x <= x1; x++)
		{
			DrawPoint(x, y, argb);
		}
	}
}
