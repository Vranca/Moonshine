#include <iostream>
#include <SDL.h>
#include "Moonshine.h"

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
				case SDLK_UP:
					player.moveForward(delta);
					if (map.At(player.GetCoordinates().x, player.GetCoordinates().y) == '#')
						player.moveBack(delta);
					break;

				case SDLK_DOWN:
					player.moveBack(delta);
					if (map.At(player.GetCoordinates().x, player.GetCoordinates().y) == '#')
						player.moveForward(delta);
					break;

				case SDLK_LEFT:
					player.rotateLeft(delta);
					break;

				case SDLK_RIGHT:
					player.rotateRight(delta);
					break;

				default:
					break;
				}
			}
		}

		return true;
	}

	bool OnRender()
	{
		DrawRaycast(player, map);
		DrawMap();
		PresentRender();

		int nFloor = GetHeight() - ((double)(GetHeight() / 2.0));
		RenderCeiling();
		RenderFloor(nFloor);

		return true;
	}

private:
	void RenderCeiling()
	{
		SetRenderColor(15, 25, 60, 0);
		ClearRenderer();
	}

	void RenderFloor(int nFloor)
	{
		moon::Rect floor = {0, nFloor, GetWidth(), GetHeight() - nFloor };
		moon::Color c1 = {64, 155, 80, 255};
		moon::Color c2 = {10, 20, 12, 255};
		FillVerticalGradientRect(floor, c1, c2);
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
					SetRenderColor(0, 0, 0, 0);
				}
				else if ((int)player.GetCoordinates().x == x && (int)player.GetCoordinates().y == y)
				{
					SetRenderColor(255, 0, 0, 0);
				}
				else
				{
					SetRenderColor(255, 255, 255, 0);
				}
				DrawPoint(x * nMiniMapSize, y * nMiniMapSize);
				DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize);
				DrawPoint(x * nMiniMapSize, y * nMiniMapSize + 1);
				DrawPoint(x * nMiniMapSize + 1, y * nMiniMapSize + 1);
			}
		}
	}
};

int main(int argc, char* argv[])
{
	Game demo;
	demo.Construct("Demo", 800, 600);
	demo.Start();

	return 0;
}