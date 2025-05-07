// FranticDreamer 2025

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <ranges>

#include "Game.hpp"

FranticMisketGame::Game::Game(FranticMatch::Scalar rowCount, FranticMatch::Scalar columnCount)
{
	std::vector<ColourfulMisket> possibleValues =
	{
		ColourfulMisket::Red,
		ColourfulMisket::Green,
		ColourfulMisket::Blue,
		ColourfulMisket::Yellow,
		ColourfulMisket::Purple,
		ColourfulMisket::Cyan
	};

	matchTable = FranticMatch::Table<ColourfulMisket>(rowCount, columnCount, possibleValues, 3u);
	matchTable.Randomise(true);
}

bool FranticMisketGame::Game::MainMenu()
{
	std::wcout	<< CN_CLR_BLUE << L"Welcome to "
				<< CN_CLR_PURPLE << L"Frantic "
				<< CN_CLR_RED << L"M"
				<< CN_CLR_YELLOW << L"i"
				<< CN_CLR_GREEN << L"s"
				<< CN_CLR_CYAN << L"k"
				<< CN_CLR_BLUE << L"e"
				<< CN_CLR_PURPLE << L"t"
				<< CN_CLR_RESET << L"!\n\n";



	std::wcout << L"This is a test game for the " << CN_CLR_GREEN << L"FranticMatch" << CN_CLR_RESET << L" engine.\n";
	std::wcout << L"Made by " << CN_CLR_CYAN << L"FranticDreamer" << CN_CLR_RESET << ".\n\n";
	std::wcout << L"Press any key to start the game...\n";

	std::wcin.ignore();

	return true;
}

bool FranticMisketGame::Game::Update()
{
	// Clear the console first
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif

	if (!matchTable.CheckBounds(0, 0))
	{
		std::wcout << L"Game is not initialized properly.\n";
		return false;
	}

	PrintTable();

	std::wcout << TABLE_SPACING;

	std::wcout << L"Score: " << GetPlayerScore();

	std::wcout << TABLE_SPACING;

	if (ProcessInput() == InputAction::Quit)
	{
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
		std::wcout << L"Quitting the game...\n\n";
		return false;
	}

	return true;
}

void FranticMisketGame::Game::End()
{
	std::wcout << L"Game ended.\n";
	std::wcout << L"Press any key to exit...\n";
	std::wcin.get();
}

void FranticMisketGame::Game::PrintTable() const
{
	static constexpr std::wstring_view rowDivider = L"  ";
	static constexpr std::wstring_view columnDivider = L" ";

	std::vector<std::wstring> colLabels(GetColumnCount());
	FranticMatch::Scalar cellWidth = 0;

	for (FranticMatch::Scalar col = 0; col < GetColumnCount(); ++col)
	{
		colLabels[col] = GetColumnLabel(col);
		cellWidth = std::max((size_t)cellWidth, colLabels[col].length());
	}

	for (FranticMatch::Scalar row = 0; row < GetRowCount(); ++row)
	{
		for (FranticMatch::Scalar col = 0; col < GetColumnCount(); ++col)
		{
			std::wostringstream oss;
			oss << (int)matchTable(row, col);
			cellWidth = std::max((size_t)cellWidth, oss.str().length());
		}
	}
	cellWidth += 1; // Spacing

	// Column Labels
	std::wcout << std::wstring(5, ' ');
	for (const auto& label : colLabels)
	{
		std::wcout << std::wstring(cellWidth - label.length(), ' ') << label;
	}
	std::wcout << L"\n";

	// Column Divider
	std::wcout << std::wstring(5, ' ');
	for (FranticMatch::Scalar i = 0; i < GetColumnCount(); ++i)
	{
		std::wcout << std::wstring(cellWidth - 1, ' ') << columnDivider;
	}
	std::wcout << L"\n";

	// Row Labels and Rows
	for (FranticMatch::Scalar row = 0; row < GetRowCount(); ++row)
	{
		std::wstring rowLabel = std::to_wstring(row + 1);
		std::wcout << std::wstring(3 - rowLabel.length(), ' ') << rowLabel << rowDivider;

		for (FranticMatch::Scalar col = 0; col < GetColumnCount(); ++col)
		{
			const auto& misket = matchTable(row, col);
			//std::wostringstream oss;
			//oss << GetMisketDisplay(misket);
			std::wstring cell = std::wstring(GetMisketSymbol(misket));
			std::wcout << std::wstring(cellWidth - cell.length(), ' ') << GetMisketDisplay(misket);
			
			/*
			std::wostringstream oss;
			oss << (int)matchTable(row, col);
			std::wstring cell = oss.str();
			std::wcout << std::wstring(cellWidth - cell.length(), ' ') << cell;
			*/
		}
		std::wcout << L"\n";
	}
}

std::wstring FranticMisketGame::Game::GetColumnLabel(FranticMatch::Scalar index) const
{
	std::wstring label;
	do
	{
		label = static_cast<wchar_t>(L'A' + (index % 26)) + label;
		index = index / 26 - 1;
	} while (index != static_cast<FranticMatch::Scalar>(-1));

	return label;
}

void FranticMisketGame::Game::PrintMatches() const
{
	std::wcout << "\nMatches:\n";
	auto matches = matchTable.FindMatchGroups(3);

	for (const auto& match : matches)
	{
		for (const auto& pos : match)
		{
			std::wcout << "(" << pos.row + 1 << ", " << GetColumnLabel(pos.column) << ") ";
		}
		std::wcout << L"\n";
	}
}

FranticMisketGame::Game::InputAction FranticMisketGame::Game::ProcessInput()
{
	if (primarySelectedMisket != INVALID_MISKET)
	{
		FirstSelectInstructions();
	}
	else
	{
		MainGameInstructions();
	}

	// Quit comand is shared between instructions
	std::wcout << L"- Quit\n";
	std::wcout << L"Type '" << QUIT_COMMAND << "' to quit the game.\n";

	std::wstring input;
	std::wcin >> input;
	std::transform(input.begin(), input.end(), input.begin(), ::towupper);

	if (input == QUIT_COMMAND)
	{
		return InputAction::Quit;
	}

	if (primarySelectedMisket != INVALID_MISKET)
	{
		return FirstSelectInput(input);
	}
	else
	{
		return MainGameInput(input);
	}
}

void FranticMisketGame::Game::MainGameInstructions() const
{
	std::wcout << infoInstruction;

	std::wcout << L"Choose an action:\n";
	std::wcout << L"- Select Misket\n";
	std::wcout << L"Choose a Misket to select. Format is \"Row-Column\" \n";
	std::wcout << L"Example: 4-B for row 4 column B\n\n";

	std::wcout << L"- Swap Miskets\n";
	std::wcout << L"Choose two Miskets to swap. Format is \"Row-Column Row-Column\" \n";
	std::wcout << L"Example: 4-B 5-C for row 4 column B and row 5 column C\n\n";
}

void FranticMisketGame::Game::FirstSelectInstructions() const
{
	std::wcout << infoInstruction;
	//std::wcout << L"Selected Misket: (" << primarySelectedMisket.row + 1 << ", " << primarySelectedMisket.column + 1 << L")\n\n";
	
	std::wcout << L"Choose an action:\n";
	std::wcout << L"- Select Misket to Swap\n";
	std::wcout << L"Choose another Misket to swap with the selected one. Format is \"Row-Column\" \n";
	std::wcout << L"Example: 4-B for row 4 column B\n\n";

	std::wcout << L"- Deselect Misket\n";
	std::wcout << L"Type '" << DESELECT_COMMAND << "' or '" << BACK_COMMAND << "' to deselect the current Misket.\n";
	std::wcout << L"and return to the previous menu.\n\n";
}

FranticMisketGame::Game::InputAction FranticMisketGame::Game::MainGameInput(const std::wstring& input)
{
	size_t pos = input.find(L" ");

	if (pos != std::wstring::npos)
	{
		// Swap action
		std::wistringstream iss(input);
		std::wstring first, second;
		iss >> first >> second;

		primarySelectedMisket = ParseMisketPosition(first);
		secondarySelectedMisket = ParseMisketPosition(second);

		return SwapMiskets(false);

		//infoInstruction = L"Invalid input. Please try again.\n\n";
		//return InputAction::None;
	}

	pos = input.find(L"-");

	if (pos != std::wstring::npos)
	{
		// Select action
		std::wistringstream iss(input);
		std::wstring inputStr;
		iss >> inputStr;

		auto misketPos = ParseMisketPosition(inputStr);
		if (misketPos != INVALID_MISKET)
		{
			if (primarySelectedMisket == INVALID_MISKET)
			{
				primarySelectedMisket = misketPos;
				infoInstruction = std::format
				(
					L"Selected Misket:{} ({}, {}){}\n\n", 
					GetColourCode(matchTable(primarySelectedMisket)),
					primarySelectedMisket.row + 1, 
					GetColumnLabel(primarySelectedMisket.column), 
					CN_CLR_RESET
				);
				return InputAction::Select;
			}
			else
			{
				secondarySelectedMisket = misketPos;
				infoInstruction = std::format(L"Miskets to Swap: ({}, {}) and ({}, {})\n\n", primarySelectedMisket.row + 1, GetColumnLabel(primarySelectedMisket.column), secondarySelectedMisket.row + 1, GetColumnLabel(secondarySelectedMisket.column));
				return InputAction::SelectSwap;
			}
		}
		else
		{
			infoInstruction = L"Invalid input format. Please use the format \"Row-Column\".\n\n";
			return InputAction::None;
		}
	}

	infoInstruction = L"Invalid input. Please try again.\n\n";
	return InputAction::None;
}

FranticMisketGame::Game::InputAction FranticMisketGame::Game::FirstSelectInput(const std::wstring& input)
{
	if (input == DESELECT_COMMAND || input == BACK_COMMAND)
	{
		infoInstruction = EMPTY_INFO_INSTRUCTION;
		primarySelectedMisket = INVALID_MISKET;
		secondarySelectedMisket = INVALID_MISKET;
		return InputAction::Undo;
	}

	size_t pos = input.find(L"-");

	if (pos != std::wstring::npos)
	{
		// Select action
		std::wistringstream iss(input);
		std::wstring inputStr;
		iss >> inputStr;

		auto misketPos = ParseMisketPosition(inputStr);
		if (misketPos != INVALID_MISKET)
		{
			if (primarySelectedMisket == INVALID_MISKET)
			{
				infoInstruction = L"Invalid input. Please try again.\n\n";
				return InputAction::None;
			}
			else if (primarySelectedMisket == misketPos)
			{
				infoInstruction = L"You have already selected this Misket!\n\n";
				return InputAction::None;
			}
			else if (!matchTable.IsAdjacent(primarySelectedMisket, misketPos))
			{
				infoInstruction = std::wstring(CN_CLR_RED) + L"Selected Miskets are not adjacent!\n\n" + std::wstring(CN_CLR_RESET);
				return InputAction::None;
			}
			else
			{
				secondarySelectedMisket = misketPos;
				return SwapMiskets(true);
			}
		}
		else
		{
			infoInstruction = L"Invalid input format. Please use the format \"Row-Column\".\n\n";
			return InputAction::None;
		}
	}

	infoInstruction = L"Invalid input. Please try again.\n\n";
	return InputAction::None;
}

FranticMisketGame::Game::InputAction FranticMisketGame::Game::SwapMiskets(bool selectSwap)
{
	if (IsValidMisketPosition(primarySelectedMisket) && IsValidMisketPosition(secondarySelectedMisket))
	{
		auto matches = matchTable.SwapAndGetMatches(primarySelectedMisket, secondarySelectedMisket);

		if (matches.empty())
		{
			infoInstruction = std::wstring(CN_BG_CLR_RED) + L"No matches found with selected Miskets!\n\n" + std::wstring(CN_CLR_RESET);
			primarySelectedMisket = INVALID_MISKET;
			secondarySelectedMisket = INVALID_MISKET;
			return InputAction::None;
		}

		// TODO: There should be no invalid misket positions here.
		// Sometimes, matching function returns invalid positions, -1 as row or column.
		// I couldn't get an exact reproduction of this bug.
		// This needs a fix.
		// Code below is a workaround for that:
		//
		// Check if there are any invalid positions
		// If there, we have to invalidate that match group, and remove it from the list
		// This is a bit of a hack, but until a fix, let's do this
		for (size_t i = 0; i < matches.size(); ++i)
		{
			for (const auto& pos : matches[i])
			{
				if (!IsValidMisketPosition(pos))
				{
					//std::wcout << L"Invalid Misket position: (" << pos.row + 1 << L", " << GetColumnLabel(pos.column) << L")\n";
					matches.erase(matches.begin() + i);
					--i;
					break;
				}
			}
		}

		// Matches found! Now, let's pop'em! Recursively!

		// This looks a bit weird
		// But it works
		infoInstruction = std::format
		(
			L"{}Swapped Miskets!:{} {}({}, {}){} and {}({}, {})\n\n{}",
			CN_CLR_GREEN, CN_CLR_RESET,

			GetMisketDisplay(matchTable(primarySelectedMisket), false),
			primarySelectedMisket.row + 1, 
			GetColumnLabel(primarySelectedMisket.column), 

			CN_CLR_RESET,

			GetMisketDisplay(matchTable(secondarySelectedMisket), false),
			secondarySelectedMisket.row + 1, 
			GetColumnLabel(secondarySelectedMisket.column),

			CN_CLR_RESET
		);

		primarySelectedMisket = INVALID_MISKET;
		secondarySelectedMisket = INVALID_MISKET;
	
		// Keep popping matches until there are no more matches
		while (!matches.empty())
		{
			// Calculate the score
			for (const auto& match : matches)
			{
				for (const auto& misket : match)
				{
					AddMatchScore(1);
				}
			}

			matchTable.PopMisketMatchGroups(matches);
			matches = matchTable.FindMatchGroups();
		}

		return selectSwap ? InputAction::SelectSwap : InputAction::Swap;
	}

	infoInstruction = std::wstring(CN_BG_CLR_RED) + L"Invalid Misket positions! Are you sure your input format is currect?\n\n" + std::wstring(CN_CLR_RESET);
	return InputAction::None;
}

FranticMatch::Scalar FranticMisketGame::Game::GetRowCount() const
{
	return matchTable.GetRowCount();
}

FranticMatch::Scalar FranticMisketGame::Game::GetColumnCount() const
{
	return matchTable.GetColumnCount();
}

FranticMatch::MisketPosition FranticMisketGame::Game::ParseMisketPosition(const std::wstring& input) const
{
	if (input.empty())
	{
		return INVALID_MISKET;
	}

	std::vector<std::wstring_view> strings;
	strings.reserve(2);

	for (auto&& part : std::views::split(input, L'-'))
	{
		strings.emplace_back(std::wstring_view(part.begin(), part.end()));
	}

	// Check if we have two parts
	if (strings.size() == 2)
	{
		// Rows are numbers
		// Columns are letters
		// 
		// Rows can be any number
		// Columns can be multiple letters
		// Examples: 4-B, 56-AB, 1-AAA, 423-C etc.

		bool isOnlyNumbers = std::ranges::all_of(strings[0], [](wchar_t ch)
			{
				return std::isdigit(ch);
			});

		bool isOnlyLetters = std::ranges::all_of(strings[1], [](wchar_t ch)
			{
				return std::isalpha(ch);
			});

		if (!isOnlyLetters)
		{
			return INVALID_MISKET;
		}

		if (!isOnlyNumbers)
		{
			return INVALID_MISKET;
		}

		FranticMatch::Scalar row = std::stoi(strings[0].data()) - 1;
		FranticMatch::Scalar col = 0;
		for (size_t i = 0; i < strings[1].length(); ++i)
		{
			col *= 26;
			col += strings[1][i] - L'A';
		}

		return { row, col };
	}

	return INVALID_MISKET;
}

bool FranticMisketGame::Game::IsValidMisketPosition(const FranticMatch::MisketPosition& pos) const
{
	return pos.column != INVALID_MISKET.column && pos.row != INVALID_MISKET.row &&
		matchTable.CheckBounds(pos);
}
