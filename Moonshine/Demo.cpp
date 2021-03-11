#include <iostream>
#include <SDL.h>
#include "Moonshine.h"
#include <stdint.h>

class Game : public moon::MoonshineEngine
{
private:
	moon::Map map;
	moon::Camera player;
	SDL_Event e;

public:
	bool OnCreate()
	{
		player.SetCoordinates(26.5, 14.5);
		SDL_ShowCursor(0);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		return true;
	}

	bool OnUpdate(const double& delta)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			//User presses a key
			if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_w:
				case SDLK_UP:
					player.moveForward(delta);
					if (map.At(player.GetCoordinates().x, player.GetCoordinates().y) == '#')
						player.moveBack(delta);
					break;

				case SDLK_s:
				case SDLK_DOWN:
					player.moveBack(delta);
					if (map.At(player.GetCoordinates().x, player.GetCoordinates().y) == '#')
						player.moveForward(delta);
					break;

				case SDLK_a:
				case SDLK_LEFT:
					player.rotateLeft(delta);
					break;

				case SDLK_d:
				case SDLK_RIGHT:
					player.rotateRight(delta);
					break;
				case SDLK_ESCAPE:
					Stop();
					break;

				default:
					break;
				}
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				static int xpos = GetWidth() / 2; // = 400 to center the cursor in the window
				//static int ypos = GetHeight() / 2; // = 300 to center the cursor in the window
				xpos += e.motion.xrel;
				//ypos += e.motion.yrel;

				player.rotate(delta, (e.motion.xrel));
			}
		}

		return true;
	}

	bool OnRender()
	{
		int nFloor = GetHeight() - ((double)(GetHeight() / 2.0));
		RenderCeiling();
		RenderFloor(nFloor);

		RenderWall();
		DrawMap();

		// Render tests
		// DrawRect({ 200, 200, 200, 200 }, 0);

		return true;
	}

private:
	void RenderCeiling()
	{
		ClearRenderer(0xFF0F193C);
	}

	void RenderWall()
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			double fRayAngle = (player.GetDirectionAngle() - player.GetFOV() / 2.0) + ((double)x / (double)GetWidth()) * player.GetFOV();

			double fDistanceToWall = Raycast(player.GetCoordinates(), fRayAngle, map, '#') * cos(player.GetDirectionAngle() - fRayAngle);

			RenderRaycast(x, fDistanceToWall);
		}
	}

	void RenderRaycast(const int& x, const double& fDistanceToWall)
	{
		int wallShading = 160 - fDistanceToWall * 10;
		wallShading = wallShading < 30 ? 30 : wallShading;
		int top = (double)(GetHeight() / 2.0) - (double)GetHeight() / ((double)fDistanceToWall);
		int bottom = GetHeight() - top;

		top = top < 0 ? 0 : top;
		bottom = bottom >= GetHeight() ? GetHeight()-1 : bottom;

		uint32_t wallColor = 0xFF000000 | (wallShading << 16) | (wallShading << 8) | wallShading;
		DrawLine(x, top, x, bottom, wallColor);
	}

	void RenderFloor(int nFloor)
	{
		moon::Rect floor = {0, nFloor, GetWidth(), GetHeight() - nFloor };
		uint32_t argb0 = (0xFF000000 | 64 << 16 | 155 << 8 | 155);
		uint32_t argb1 = (0xFF000000 | 10 << 16 | 20 << 8 | 12);
		FillVerticalGradientRect(floor, argb1, argb0);
	}

	void DrawMap()
	{
		int nMiniMapSize = 2;

		for (int y = 0; y < map.GetHeight(); y++)
		{
			for (int x = 0; x < map.GetWidth(); x++)
			{
				if (map.At(x, y) == '#')
				{
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize, 0);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize, 0);
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize + 1, 0);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize + 1, 0);
				}
				else if ((int)player.GetCoordinates().x == x && (int)player.GetCoordinates().y == y)
				{
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize, 0xFFFF0000);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize, 0xFFFF0000);
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize + 1, 0xFFFF0000);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize + 1, 0xFFFF0000);
				}
				else
				{
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize, 0xFFFFFFFF);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize, 0xFFFFFFFF);
					DrawPoint(x * nMiniMapSize, y * nMiniMapSize + 1, 0xFFFFFFFF);
					DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize + 1, 0xFFFFFFFF);
				}
			}
		}
	}
};

int main(int argc, char* argv[])
{
	Game demo;

	if(demo.Construct("Demo", 1000, 600))
		demo.Start();

	return 0;
}