#include "Renderer.h"
#include <stdint.h>


namespace moon
{
	Renderer::Renderer(SDL_Window* window, int width, int height)
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

		delete[] m_TextureBuffer;
	}

	void Renderer::Present()
	{
		SDL_UpdateTexture(m_Texture, NULL, m_TextureBuffer, m_Width * sizeof(uint32_t));
		SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);
		SDL_RenderPresent(m_Renderer);
	}

	void Renderer::DrawPoint(int x, int y, uint32_t argb)
	{
		m_TextureBuffer[x + y * m_Width] = argb;
	}

	void Renderer::DrawLine(int x0, int y0, int x1, int y1, uint32_t argb)
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

	void Renderer::DrawRect(const Rect & r, uint32_t argb)
	{
		uint32_t* px_1 = m_TextureBuffer + r.x + r.y * m_Width;
		uint32_t* px_2 = m_TextureBuffer + r.x + (r.y + r.h) * m_Width;
		for (int x = r.x; x < r.x + r.w; x++)
		{
			(*px_1) = argb;
			(*px_2) = argb;
			px_1++;
			px_2++;
		}

		px_1 = m_TextureBuffer + r.x + (r.y + 1) * m_Width;
		px_2 = m_TextureBuffer + (r.x + r.w - 1) + (r.y + 1) * m_Width;
		uint32_t offset = m_Width - r.w + 1;
		for (int y = r.y; y < r.y + r.h; y++)
		{
			(*px_1) = argb;
			(*px_2) = argb;
			px_1 += m_Width;
			px_2 += m_Width;
		}
	}

	/*
	 * Fill a rectangle with a color in ARGB format
	 * Iterating with a pointer so adrress is calculated once
	 *
	 * r    - rectangle to be filled
	 * argb - color in ARGB8888 format
	 */
	void Renderer::FillRect(const Rect & r, uint32_t argb)
	{
		uint32_t* pixel = m_TextureBuffer + r.x + r.y * m_Width;
		uint32_t offset = m_Width - r.w + 1;
		for (int y = r.y; y < r.y + r.h - 1; y++)
		{
			for (int x = r.x; x < r.x + r.w - 1; x++)
			{
				(*pixel) = argb;
				pixel++;
			}
			pixel += offset;
		}
	}


	void Renderer::FillVerticalGradientRect(const Rect & r, uint32_t argb0, uint32_t argb1)
	{
		double nStepRInit = (double)((argb1 >> 16 & 0x000000FF) - (argb0 >> 16 & 0x000000FF)) / (double)r.h;
		double nStepGInit = (double)((argb1 >> 8 & 0x000000FF) - (argb0 >> 8 & 0x000000FF)) / (double)r.h;
		double nStepBInit = (double)((argb1 & 0x000000FF) - (argb0 & 0x000000FF)) / (double)r.h;
		double nStepR = 0.0;
		double nStepG = 0.0;
		double nStepB = 0.0;

		for (int y = r.y; y < r.y + r.h; y++)
		{
			DrawHorizontalLine(r.x, r.x + r.w - 1, y, (argb0 + ((uint32_t)nStepR << 16) + ((uint32_t)nStepG << 8) + ((uint32_t)nStepB)));
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

	void Renderer::Clear(uint32_t argb)
	{
		uint32_t* pixel = m_TextureBuffer;
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				*(pixel) = argb;
				pixel++;
			}
	}

	void Renderer::DrawLineLow(int x0, int y0, int x1, int y1, uint32_t argb)
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
	void Renderer::DrawLineHigh(int x0, int y0, int x1, int y1, uint32_t argb)
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
	void Renderer::DrawVerticalLine(int y0, int y1, int x, uint32_t argb)
	{
		uint32_t* pixel = m_TextureBuffer + x + y0 * m_Width;
		for (int y = y0; y <= y1; y++)
		{
			(*pixel) = argb;
			pixel += m_Width;
		}
	}
	void Renderer::DrawHorizontalLine(int x0, int x1, int y, uint32_t argb)
	{
		uint32_t* pixel = m_TextureBuffer + x0 + y * m_Width;
		for (int x = x0; x <= x1; x++)
		{
			(*pixel) = argb;
			pixel++;
		}
	}
}
