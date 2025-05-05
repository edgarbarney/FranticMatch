// FranticDreamer 2025
#pragma once

#include <string>
#include <string_view>

#include "ColourfulMisket/ColourfulMisket.hpp"

#include "FranticMatch/FranticMatch.hpp"

namespace FranticMisketGame
{
	/// <summary>
	/// A class representing the game.
	/// It is a match-N game.
	/// </summary>
	class Game
	{
	public:
		enum class InputAction
		{
			None = 0,
			Select,		/// <summary> Select a misket </summary>
			SelectSwap, /// <summary> Select a second misket to swap with the first </summary>
			Swap,		/// <summary> Select two miskets to swap </summary>
			Undo,		/// <summary> Undo the last action </summary>
			Quit,		/// <summary> Quit the game </summary>
		};

		/// <summary>
		/// The minimum number of matches required to be considered a match.
		/// </summary>
		static constexpr unsigned int MIN_MATCH_COUNT = 3;

		/// <summary>
		/// Invalid misket position.
		/// This is used to indicate that a misket is not selected.
		/// </summary>
		static constexpr FranticMatch::MisketPosition INVALID_MISKET { -1, -1 };

		/// <summary>
		/// Spacing between the table and the instructions.
		/// This is used to separate the table from the instructions.
		/// </summary>
		static constexpr std::wstring_view TABLE_SPACING = L"\n\n";

		/// <summary>
		/// Empty instruction string.
		/// Placeholder for the instructions.
		/// </summary>
		static constexpr std::wstring_view EMPTY_INFO_INSTRUCTION = L"\n\n";

		// ==
		// Command strings for the game.
		// ==

		static constexpr std::wstring_view QUIT_COMMAND = L"QUIT";
		static constexpr std::wstring_view DESELECT_COMMAND = L"DESELECT";
		static constexpr std::wstring_view BACK_COMMAND = L"BACK";

	private:
		FranticMatch::Table<ColourfulMisket> matchTable;

		FranticMatch::MisketPosition primarySelectedMisket = INVALID_MISKET;
		FranticMatch::MisketPosition secondarySelectedMisket = INVALID_MISKET;

		/// <summary>
		/// Information string for the input instructions.
		/// This is used to display the information form before, or the current state of the game.
		/// </summary>
		/// <remarks>
		/// The empty string is two new lines.
		/// It is because of the alignment of the instructions.
		/// 
		/// "Choose an action" should align in every instruction.
		/// So the empty string is used to fill the space.
		/// </remarks>
		std::wstring infoInstruction { EMPTY_INFO_INSTRUCTION };

		/// <summary>
		/// The score of the player.
		/// This is used to keep track of the score;
		int64_t playerScore = 0;

	public:
		Game() = default;
		~Game() = default;

		/// <summary>
		/// Constructor for the game.
		/// Also used as the Init function.
		/// </summary>
		/// <param name="rowCount">The number of rows that will be in the table.</param>
		/// <param name="columnCount">The number of columns that will be in the table.</param>
		Game(FranticMatch::Scalar rowCount, FranticMatch::Scalar columnCount);

		/// <summary>
		/// Event loop for the game.
		/// </summary>
		/// <returns>True if the game should continue running.</returns>
		bool Update();

		/// <summary>
		/// End the game and clean up resources.
		/// </summary>
		void End();

		/// <summary>
		/// Get the number of rows in the table.
		/// </summary>
		/// <returns>The number of rows in the table.</returns>
		FranticMatch::Scalar GetRowCount() const;

		/// <summary>
		/// Get the number of columns in the table.
		/// </summary>
		/// <returns>The number of columns in the table.</returns>
		FranticMatch::Scalar GetColumnCount() const;

		/// <summary>
		/// Calculate the index of the misket from the row and column format.
		/// </summary>
		/// <param name="input">The input string in the format of "Row-Column".</param>
		/// <returns>The index of the given misket.</returns>
		FranticMatch::MisketPosition ParseMisketPosition(const std::wstring& input) const;

		/// <summary>
		/// Is the given position valid?
		/// </summary>
		/// <param name="pos">The position to check.</param>
		/// <returns>True if the position is valid, false otherwise.</returns>
		bool IsValidMisketPosition(const FranticMatch::MisketPosition& pos) const;

	private:
		/// <summary>
		/// Prints the table similar to a chess board, letters for columns and numbers for rows.
		/// <para>
		/// If letters overflow, use two letters,
		/// if they overflow again, use three letters etc.
		/// </para>
		/// </summary>
		void PrintTable() const;

		/// <summary>
		/// Get the label for a column based on its index.
		/// </summary>
		/// <param name="index">The index of the column.</param>
		/// <returns>The label for the column.</returns>
		std::wstring GetColumnLabel(FranticMatch::Scalar index) const;

		// Debug
		void PrintMatches() const;

		/// <summary>
		/// Process the input from the user.
		/// </summary>
		InputAction ProcessInput();

		/// <summary>
		/// Print the instructions for the main game.
		/// </summary>
		void MainGameInstructions() const;

		/// <summary>
		/// Print the instructions after the first misket is selected.
		/// </summary>
		void FirstSelectInstructions() const;

		/// <summary>
		/// Input for the main game.
		/// </summary>
		InputAction MainGameInput(const std::wstring& input);

		/// <summary>
		/// Input after the first misket is selected.
		/// </summary>
		InputAction FirstSelectInput(const std::wstring& input);

		/// <summary>
		/// Swap the two selected miskets. 
		/// Note that primarySelectedMisket is the first one
		/// and secondarySelectedMisket is the second one.
		/// </summary>
		/// <param name="selectSwap">True if the first one was selected beforehand.</param>
		InputAction SwapMiskets(bool selectSwap);
	};
}