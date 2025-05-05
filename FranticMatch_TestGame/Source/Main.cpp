// FranticDreamer 2025

// This is the main file for a test game that utilises FranticMatch engine.
// FranticMatch is an engine for creating matching games.
// And this is a test match-3 game to test the engine.
// 
// In this file, wide characters are used to support Unicode.
// The game is designed to be run in a console window.

#include <iostream>
#include <sstream>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <conio.h>
#endif

#include "Game/Game.hpp"

int main(int argc, char* argv[])
{
	// Set C++ and C locale to UTF-8
	std::locale::global(std::locale("en_US.UTF-8"));
	std::wcout.imbue(std::locale());

#ifdef _WIN32
	// Set Windows console to UTF-8
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	size_t rowCount = 8;
	size_t columnCount = 8;

	FranticMisketGame::Game mainGame(rowCount, columnCount);

	bool running = true;

	while (running)
	{
		running = mainGame.Update();
	}

	mainGame.End();
}
