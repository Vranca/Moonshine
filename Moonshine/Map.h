#pragma once
#include <iostream>

namespace moon
{
	class Map
	{
	public:
		Map();
		~Map();

		bool LoadMap(const char* mapSourceFile);

	private:
		int m_Width;
		int m_Height;
		std::wstring m_Map;

	public:
		inline const int&		GetWidth()			const	{	return m_Width;					}
		inline const int&		GetHeight()			const	{	return m_Height;				}
		inline const wchar_t&	At(int x, int y)	const	{	return m_Map[y * m_Width + x];	}
	};
}

