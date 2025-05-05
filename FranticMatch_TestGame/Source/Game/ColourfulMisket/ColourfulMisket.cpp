// FranticDreamer 2025

#include "ColourfulMisket.hpp"

std::wstring_view FranticMisketGame::GetColourName(ColourfulMisket colour)
{
	switch (colour)
	{
	case ColourfulMisket::Red:		return L"Red";
	case ColourfulMisket::Green:	return L"Green";
	case ColourfulMisket::Blue:		return L"Blue";
	case ColourfulMisket::Yellow:	return L"Yellow";
	case ColourfulMisket::Purple:	return L"Purple";
	case ColourfulMisket::Cyan:		return L"Cyan";
	default: return L"None";
	}
}

std::wstring FranticMisketGame::GetColourCode(ColourfulMisket colour)
{
	switch (colour)
	{
	case ColourfulMisket::Red:		return std::wstring(CN_CLR_RED);	//+ std::wstring(CN_BG_CLR_WHITE);
	case ColourfulMisket::Green:	return std::wstring(CN_CLR_GREEN);	//+std::wstring(CN_BG_CLR_WHITE);
	case ColourfulMisket::Blue:		return std::wstring(CN_CLR_BLUE);	//+  std::wstring(CN_BG_CLR_WHITE);
	case ColourfulMisket::Yellow:	return std::wstring(CN_CLR_YELLOW);	//+ std::wstring(CN_BG_CLR_WHITE);
	case ColourfulMisket::Purple:	return std::wstring(CN_CLR_PURPLE);	//+ std::wstring(CN_BG_CLR_WHITE);
	case ColourfulMisket::Cyan:		return std::wstring(CN_CLR_CYAN);	//+ std::wstring(CN_BG_CLR_WHITE);
	default: return std::wstring(CN_CLR_RESET);
	}
}

std::wstring FranticMisketGame::GetMisketSymbol(ColourfulMisket colour)
{
	//return MISKET_SYMBOL;
	return std::to_wstring((int)colour);
}

std::wstring FranticMisketGame::GetMisketDisplay(ColourfulMisket colour, bool reset)
{
	if (reset)
	{
		return GetColourCode(colour) + std::wstring(GetMisketSymbol(colour)) + std::wstring(CN_CLR_RESET); //+ CN_BG_CLR_RESET;
	}

	return GetColourCode(colour) + std::wstring(GetMisketSymbol(colour));
}
