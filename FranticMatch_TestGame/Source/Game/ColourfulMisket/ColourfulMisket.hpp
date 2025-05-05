// FranticDreamer 2025
#pragma once

#include <string>
#include <string_view>

namespace FranticMisketGame
{
	/// <summary>
	/// Enum class representing the colourful misket.
	/// Each colour represents a different type of misket.
	/// </summary>
	enum class ColourfulMisket
	{
		None = -1,
		Red,
		Green,
		Blue,
		Yellow,
		Purple,
		Cyan,
	};

	//inline constexpr std::wstring_view MISKET_SYMBOL = L"0"; // Misket symbol

	inline constexpr std::wstring_view CN_CLR_RED = L"\033[31m";
	inline constexpr std::wstring_view CN_CLR_GREEN = L"\033[32m";
	inline constexpr std::wstring_view CN_CLR_BLUE = L"\033[34m";
	inline constexpr std::wstring_view CN_CLR_YELLOW = L"\033[33m";
	inline constexpr std::wstring_view CN_CLR_PURPLE = L"\033[35m";
	inline constexpr std::wstring_view CN_CLR_CYAN = L"\033[36m";
	inline constexpr std::wstring_view CN_CLR_WHITE = L"\033[37m";
	inline constexpr std::wstring_view CN_CLR_BLACK = L"\033[30m";
	inline constexpr std::wstring_view CN_CLR_RESET = L"\033[0m";
	inline constexpr std::wstring_view CN_CLR_DEFAULT = L"\033[39m";

	inline constexpr std::wstring_view CN_BG_CLR_RED = L"\033[41m";
	inline constexpr std::wstring_view CN_BG_CLR_GREEN = L"\033[42m";
	inline constexpr std::wstring_view CN_BG_CLR_BLUE = L"\033[44m";
	inline constexpr std::wstring_view CN_BG_CLR_YELLOW = L"\033[43m";
	inline constexpr std::wstring_view CN_BG_CLR_PURPLE = L"\033[45m";
	inline constexpr std::wstring_view CN_BG_CLR_CYAN = L"\033[46m";
	inline constexpr std::wstring_view CN_BG_CLR_WHITE = L"\033[47m";
	inline constexpr std::wstring_view CN_BG_CLR_BLACK = L"\033[40m";
	inline constexpr std::wstring_view CN_BG_CLR_RESET = L"\033[0m";
	inline constexpr std::wstring_view CN_BG_CLR_DEFAULT = L"\033[49m";

	/// <summary>
	/// Get the name of the colour.
	/// </summary>
	/// <param name="colour">The colour to get the name of.</param>
	/// <returns>The name of the colour.</returns> 
	std::wstring_view GetColourName(ColourfulMisket colour);

	/// <summary>
	/// Get the ANSI escape code for the colour.
	/// </summary>
	/// <param name="colour">The colour to get the code of.</param>
	/// <returns>
	/// The ANSI escape code for the colour.
	/// <para>
	/// Reset code if the colour is None.
	/// </para>
	/// </returns>
	std::wstring GetColourCode(ColourfulMisket colour);

	/// <summary>
	/// Get the symbol for the misket.
	/// </summary>
	/// <remarks>
	/// This is just a symbol, without ANSI escape codes.
	/// 
	/// This is also a future proofing measure.
	/// For now, we may just use the same symbol for all colours.
	/// But in the future, we may want to use different symbols for different colours.
	/// This is just a placeholder for it now.
	/// </remarks>
	/// <param name="colour">The colour to get the symbol of.</param>
	/// <returns> Symbol for the colour.</returns>
	std::wstring GetMisketSymbol(ColourfulMisket colour);

	/// <summary>
	/// Get the display string for the colour.
	/// </summary>
	/// <remarks>
	/// Even though this is a string, after the print it will just be one symbol.
	/// </remarks>
	/// <param name="colour">The colour to get the display string of.</param>
	/// <returns>A string containing the misket symbol and ANSI escape codes.</returns>
	std::wstring GetMisketDisplay(ColourfulMisket colour, bool reset = true);
}