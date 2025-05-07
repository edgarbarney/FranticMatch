// FranticDreamer 2025
#pragma once

#include <vector>
#include <span>
#include <random>
#include <set>

#define FRANTICMATCH_API

namespace FranticMatch
{
	using Scalar = int;

	/// <summary>
	/// A 2D Vector structure representing a position in a 2D grid.
	/// It can be used to represent the position of a misket on the table.
	/// </summary>
	template <typename T = Scalar>
	struct Vector2D
	{
		union 
		{
			struct 
			{
				T row;
				T column;
			};

			struct
			{
				T x;
				T y;
			};

			T indices[2];
		};
		
		constexpr Vector2D()
			: row(0u), column(0u)
		{
		}

		constexpr Vector2D(T size)
			: row(size), column(size)
		{
		}

		constexpr Vector2D(T r, T c)
			: row(r), column(c)
		{
		}

		constexpr bool operator==(const Vector2D& other) const
		{
			return row == other.row && column == other.column;
		}
		constexpr bool operator!=(const Vector2D& other) const
		{
			return !(*this == other);
		}
		constexpr bool operator<(const Vector2D& other) const
		{
			return row < other.row || (row == other.row && column < other.column);
		}
		constexpr bool operator>(const Vector2D& other) const
		{
			return row > other.row || (row == other.row && column > other.column);
		}

		T Magnitude() const
		{
			return std::sqrt(row * row + column * column);
		}

		T Length() const
		{
			return Magnitude();
		}
	};

	using MisketPosition = Vector2D<Scalar>;
	using MisketMatchGroup = std::vector<MisketPosition>;

	/// <summary>
	/// A class representing a 2D match table.
	/// It is a grid of elements that is used for matching games.
	/// </summary>
	/// <typeparam name="T">The type of the elements in the table. Aka. Misket</typeparam>
	/// <typeparam name="S">The scalar type for the vectors (positions etc.)</typeparam>
	template <typename T, typename S = Scalar>
	class FRANTICMATCH_API Table
	{
	public:
		/// <summary>
		/// Match directions for the table.
		/// This is used to determine the directions in which matches can be made.
		/// 
		/// Default is horizontal and vertical.
		/// </summary>
		struct MatchDirections
		{
			bool horizontal = true;
			bool vertical = true;
			bool diagonal = false;
		};

	private:
		S rowCount;
		S columnCount;
		std::vector<T> data;

		/// <summary>
		/// Possible values for the Miskets.
		/// This is used to randomise the table.
		/// </summary>
		std::vector<T> possibleValues;

		// Can be unsigned, but for the setup, let's keep it signed
		/// <summary>
		/// Minimum length of a match.
		/// </summary>
		int minimumMatchLength;

		inline static std::random_device randomDevice;
		inline static std::mt19937 randomGen = std::mt19937(randomDevice());

	public:
		Table()
			: rowCount(0u), columnCount(0u), data(), possibleValues(), minimumMatchLength(3u)
		{
		}

		Table(S rows, S columns, const std::vector<T>& possibleValues, int minMatchLength = 3u)
			: rowCount(rows), columnCount(columns), data(rows* columns), possibleValues(possibleValues), minimumMatchLength(minMatchLength)
		{
		}

		Table(MisketPosition size, const std::vector<T>& possibleValues, int minMatchLength = 3u)
			: rowCount(size.row), columnCount(size.column), data(size.row* size.column), possibleValues(possibleValues), minimumMatchLength(minMatchLength)
		{
		}

		T& operator()(S row, S column)
		{
			return data[Index(row, column)];
		}

		T& operator()(MisketPosition pos)
		{
			return data[Index(pos)];
		}

		const T& operator()(S row, S column) const
		{
			return data[Index(row, column)];
		}

		const T& operator()(MisketPosition pos) const
		{
			return data[Index(pos)];
		}

		T* operator[](S row)
		{
			return &data[row * columnCount];
		}

		const T* operator[](S row) const
		{
			return &data[row * columnCount];
		}

		/// <summary>
		/// Get a misket from the table.
		/// </summary>
		/// <param name="row">The row index.</param>
		/// <param name="column">The column index.</param>
		/// <returns>The misket at the specified row and column.</returns>
		const T& Get(S row, S column) const
		{
			return data[Index(row, column)];
		}

		/// <summary>
		/// Get a misket from the table.
		/// </summary>
		/// <param name="pos">The position of the misket.</param>
		/// <returns>The misket at the specified position.</returns>
		const T& Get(MisketPosition pos) const
		{
			return data[Index(pos)];
		}

		/// <summary>
		/// Set a misket in the table.
		/// </summary>
		/// <param name="row">The row index.</param>
		/// <param name="column">The column index.</param>
		/// <param name="value">The new misket value to set.</param>
		void Set(S row, S column, const T& value)
		{
			data[Index(row, column)] = value;
		}

		/// <summary>
		/// Set a misket in the table.
		/// </summary>
		/// <param name="pos">The position of the misket.</param>
		/// <param name="value">The new misket value to set.</param>
		void Set(MisketPosition pos, const T& value)
		{
			data[Index(pos)] = value;
		}

		/// <summary>
		/// Get the number of rows in the table.
		/// </summary>
		/// <returns>The number of rows in the table.</returns>
		S GetRowCount() const
		{
			return rowCount;
		}

		/// <summary>
		/// Get the number of columns in the table.
		/// </summary>
		/// <returns>The number of columns in the table.</returns>
		S GetColumnCount() const
		{
			return columnCount;
		}

		/// <summary>
		/// Get a row of the table.
		/// </summary>
		/// <param name="rowIndex">The index of the row to get.</param>
		/// <returns>A row of the table.</returns>
		std::vector<T> GetRow(S rowIndex) const
		{
			S start = rowIndex * columnCount;
			return std::vector<T>(data.begin() + start, data.begin() + start + columnCount);
		}

		/// <summary>
		/// Get a row of the table as a span.
		/// </summary>
		/// <param name="rowIndex">The index of the row to get.</param>
		/// <returns>A row of the table as a span.</returns>
		std::span<T> GetRowSpan(S rowIndex)
		{
			return { &data[rowIndex * columnCount], columnCount };
		}

		/// <summary>
		/// Get a row of the table as a span.
		/// </summary>
		/// <param name="rowIndex">The index of the row to get.</param>
		/// <returns>A row of the table as a span.</returns>
		std::span<const T> GetRowSpan(S rowIndex) const
		{
			return { &data[rowIndex * columnCount], columnCount };
		}

		/// <summary>
		/// Get a column of the table.
		/// </summary>
		/// <param name="columnIndex">The index of the column to get.</param>
		/// <returns>A column of the table.</returns>
		std::vector<T> GetColumn(S columnIndex) const
		{
			std::vector<T> column;
			column.reserve(rowCount);
			for (S i = 0; i < rowCount; ++i)
			{
				column.emplace_back(data[Index(i, columnIndex)]);
			}
			return column;
		}

		/// <summary>
		/// Set a row in the table.
		/// </summary>
		/// <param name="rowIndex">The index of the row to set.</param>
		/// <param name="row">The misket values to set in the row.</param>
		void SetRow(S rowIndex, const std::vector<T>& row)
		{
			S start = rowIndex * columnCount;
			for (S i = 0; i < columnCount; ++i)
			{
				data[start + i] = row[i];
			}
		}

		/// <summary>
		/// Set a column in the table.
		/// </summary>
		/// <param name="columnIndex">The index of the column to set.</param>
		/// <param name="column">The misket values to set in the column.</param>
		void SetColumn(S columnIndex, const std::vector<T>& column)
		{
			for (S i = 0; i < rowCount; ++i)
			{
				data[Index(i, columnIndex)] = column[i];
			}
		}

		/// <summary>
		/// Resize the table to the specified number of rows and columns.
		/// </summary>
		/// <param name="newRows">Target number of rows.</param>
		/// <param name="newColumns">Target number of columns.</param>
		void Resize(S newRows, S newColumns)
		{
			rowCount = newRows;
			columnCount = newColumns;
			data.resize(newRows * newColumns);
		}

		/// <summary>
		/// Clear the table and reset its size to 0.
		/// </summary>
		void Clear()
		{
			data.clear();
			rowCount = 0u;
			columnCount = 0u;
		}

		/// <summary>
		/// Check if the specified row and column are within the bounds of the table.
		/// </summary>
		/// <param name="row">The row index.</param>
		/// <param name="column">The column index.</param>
		/// <returns>True if the row and column are within bounds, false otherwise.</returns>
		bool CheckBounds(S row, S column) const
		{
			return row < rowCount && column < columnCount;
		}

		/// <summary>
		/// Check if the specified position is within the bounds of the table.
		/// </summary>
		/// <param name="pos">The position to check.</param>
		/// <returns>True if the position is within bounds, false otherwise.</returns>
		bool CheckBounds(MisketPosition pos) const
		{
			return CheckBounds(pos.row, pos.column);
		}

		/// <summary>
		/// Randomise the table using a range of possible values.
		/// </summary>
		/// <param name="checkMatches">Should we check for matches and re-randomise matching elements?</param>
		void Randomise(bool checkMatches = true)
		{
			std::uniform_int_distribution<S> dis(0, possibleValues.size() - 1);

			for (S row = 0; row < GetRowCount(); ++row)
			{
				for (S col = 0; col < GetColumnCount(); ++col)
				{
					Set(row, col, possibleValues[dis(randomGen)]);
				}
			}

			if (!checkMatches)
			{
				return;
			}

			// Check for matches and re-randomise matching elements
			auto matchGroups = FindMatchGroups();

			while (!matchGroups.empty())
			{
				for (const auto& group : matchGroups)
				{
					for (const auto& pos : group)
					{
						Set(pos, static_cast<T>(dis(randomGen)));
					}
				}
				matchGroups = FindMatchGroups();
			}
		}
		
		/*
		// Debug
		// Get random misket from table
		T& GetRandomisedMisket()
		{
			std::uniform_int_distribution<S> dis(0, data.size() - 1);
			return data[dis(randomGen)];
		}
		*/

		/// <summary>
		/// Generate a random misket from the possible values.
		/// </summary>
		/// <returns>A generated random misket.</returns>
		T GenerateRandomMisket()
		{
			std::uniform_int_distribution<S> dis(0, possibleValues.size() - 1);
			return possibleValues[dis(randomGen)];
		}

		/// <summary>
		/// Shuffles the table.
		/// </summary>
		void Shuffle()
		{
			std::shuffle(data.begin(), data.end(), randomGen);
		}

		/// <summary>
		/// Swap two miskets in the table.
		/// </summary>
		/// <param name="row1">Row of the first misket.</param>
		/// <param name="col1">Column of the first misket.</param>
		/// <param name="row2">Row of the second misket.</param>
		/// <param name="col2">Column of the second misket.</param>
		void Swap(S row1, S col1, S row2, S col2)
		{
			std::swap((*this)(row1, col1), (*this)(row2, col2));
		}

		/// <summary>
		/// Swap two miskets in the table.
		/// </summary>
		/// <param name="pos1">Position of the first misket.</param>
		/// <param name="pos2">Position of the second misket.</param>
		void Swap(MisketPosition pos1, MisketPosition pos2)
		{
			std::swap((*this)(pos1), (*this)(pos2));
		}

		/// <summary>
		/// Check if two positions are adjacent.
		/// </summary>
		/// <param name="row1">Row of the first position.</param>
		/// <param name="col1">Column of the first position.</param>
		/// <param name="row2">Row of the second position.</param>
		/// <param name="col2">Column of the second position.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>True if the positions are adjacent, false otherwise.</returns>
		bool IsAdjacent(S row1, S col1, S row2, S col2, MatchDirections matchDirections = MatchDirections()) const
		{
			if (matchDirections.horizontal && row1 == row2 && std::abs(col1 - col2) == 1)
				return true;

			if (matchDirections.vertical && col1 == col2 && std::abs(row1 - row2) == 1)
				return true;

			if (matchDirections.diagonal && std::abs(row1 - row2) == 1 && std::abs(col1 - col2) == 1)
				return true;

			return false;
		}

		bool IsAdjacent(MisketPosition pos1, MisketPosition pos2, MatchDirections matchDirections = MatchDirections()) const
		{
			return IsAdjacent(pos1.row, pos1.column, pos2.row, pos2.column, matchDirections);
		}

		/// <summary>
		/// Find matches in the table and return as groups of matches.
		/// </summary>
		/// <param name="minMatchLength">Override for minimum length of a match.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>A vector of match groups.</returns>
		std::vector<MisketMatchGroup> FindMatchGroups(unsigned int minMatchLength = -1, MatchDirections matchDirections = MatchDirections()) const
		{
			if (minMatchLength == -1)
			{
				minMatchLength = minimumMatchLength;
			}

			std::vector<MisketMatchGroup> matchGroups;

			auto collectMatch = [&](S startRow, S startCol, S dRow, S dCol)
			{
				MisketMatchGroup currentGroup;
				S row = startRow;
				S col = startCol;
				T prev = (*this)(row, col);
				currentGroup.emplace_back(row, col);

				while (true)
				{
					row += dRow;
					col += dCol;
					if (row >= rowCount || col >= columnCount)
					{
						break;
					}

					const T& current = (*this)(row, col);
					if (current == prev)
					{
						currentGroup.emplace_back(row, col);
					}
					else
					{
						if (currentGroup.size() >= minMatchLength)
						{
							matchGroups.emplace_back(std::move(currentGroup));
						}
						currentGroup.clear();
						currentGroup.emplace_back(row, col);
						prev = current;
					}
				}

				if (currentGroup.size() >= minMatchLength)
				{
					matchGroups.emplace_back(std::move(currentGroup));
				}
			};

			// Horizontal (Left to Right)
			if (matchDirections.horizontal)
			{
				for (S row = 0; row < rowCount; ++row)
				{
					collectMatch(row, 0, 0, 1);
				}
			}

			// Vertical (Top to Bottom)
			if (matchDirections.vertical)
			{
				for (S col = 0; col < columnCount; ++col)
				{
					collectMatch(0, col, 1, 0);
				}
			}

			// Diagonal
			if (matchDirections.diagonal)
			{
				// Top-Left to Bottom-Right
				for (S row = 0; row <= rowCount - minMatchLength; ++row)
				{
					collectMatch(row, 0, 1, 1);
				}
				for (S col = 1; col <= columnCount - minMatchLength; ++col)
				{
					collectMatch(0, col, 1, 1);
				}

				// Top-Right to Bottom-Left
				for (S row = 0; row <= rowCount - minMatchLength; ++row)
				{
					collectMatch(row, columnCount - 1, 1, -1);
				}
				for (S col = columnCount - 2; col + 1 >= minMatchLength; --col)
				{
					collectMatch(0, col, 1, -1);
				}
			}

			return matchGroups;
		}

		/*
		static std::vector<MisketMatchGroup> MergeOverlappingGroups(const std::vector<MisketMatchGroup>& groups)
		{
			using Pos = std::pair<S, S>;

			std::vector<std::set<Pos>> mergedSets;

			for (const auto& group : groups)
			{
				std::set<Pos> current(group.begin(), group.end());
				std::vector<S> overlaps;

				for (S i = 0; i < mergedSets.size(); ++i)
				{
					for (const auto& pos : current)
					{
						if (mergedSets[i].count(pos))
						{
							overlaps.push_back(i);
							break;
						}
					}
				}

				if (overlaps.empty())
				{
					mergedSets.push_back(std::move(current));
				}
				else
				{
					// Merge all overlapping sets and current into one
					std::set<Pos> merged = std::move(current);
					// Merge all overlapping sets
					std::sort(overlaps.begin(), overlaps.end(), std::greater<>()); // reverse for safe erase
					for (S index : overlaps)
					{
						merged.insert(mergedSets[index].begin(), mergedSets[index].end());
						mergedSets.erase(mergedSets.begin() + index);
					}
					mergedSets.push_back(std::move(merged));
				}
			}

			// Convert sets back to vectors
			std::vector<std::vector<Pos>> result;
			for (const auto& s : mergedSets)
				result.emplace_back(s.begin(), s.end());

			return result;
		}
		*/

		/// <summary>
		/// Checks if swapping two elements results in a match.
		/// </summary>
		/// <param name="row1">Row of first element</param>
		/// <param name="col1">Column of first element</param>
		/// <param name="row2">Row of second element</param>
		/// <param name="col2">Column of second element</param>
		/// <param name="minMatchLength">Override for minimum length of a match.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>True if a match would occur after swap</returns>
		bool WouldSwapCauseMatch(S row1, S col1, S row2, S col2, unsigned int minMatchLength = -1, MatchDirections matchDirections = MatchDirections())
		{
			if (minMatchLength == -1)
			{
				minMatchLength = minimumMatchLength;
			}

			// Bounds check
			if (row1 >= rowCount || row2 >= rowCount || col1 >= columnCount || col2 >= columnCount)
				return false;

			if (row1 == row2 && col1 == col2)
				return false;

			// Swap the two elements
			Swap(row1, col1, row2, col2);

			// Check for matches after the swap
			auto matches = FindMatchGroups(minMatchLength, matchDirections);

			// Undo the swap
			Swap(row1, col1, row2, col2);

			return !matches.empty();
		}

		/// <summary>
		/// Checks if swapping two elements results in a match.
		/// </summary>
		/// <param name="pos1">Position of first element</param>
		/// <param name="pos2">Position of second element</param>
		/// <param name="minMatchLength">Override for minimum length of a match.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>True if a match would occur after swap</returns>
		bool WouldSwapCauseMatch(MisketPosition pos1, MisketPosition pos2, unsigned int minMatchLength = -1, MatchDirections matchDirections = MatchDirections())
		{
			return WouldSwapCauseMatch(pos1.row, pos1.column, pos2.row, pos2.column, minMatchLength, matchDirections);
		}

		/// <summary>
		/// Swap two elements and return the matches that would occur.
		/// </summary>
		/// <remarks>
		/// Note that this does not check if the swap is valid.
		/// Match Group returned may be empty.
		/// </remarks>
		/// <param name="row1">Row of first element</param>
		/// <param name="col1">Column of first element</param>
		/// <param name="row2">Row of second element</param>
		/// <param name="col2">Column of second element</param>
		/// <param name="minMatchLength">Override for minimum length of a match.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>Match groups that would occur after the swap</returns>
		std::vector<MisketMatchGroup> SwapAndGetMatches(S row1, S col1, S row2, S col2, unsigned int minMatchLength = -1, MatchDirections matchDirections = MatchDirections())
		{
			Swap(row1, col1, row2, col2);
			auto matches = FindMatchGroups(minMatchLength, matchDirections);

			if (matches.empty())
			{
				// No matches, undo the swap
				Swap(row1, col1, row2, col2);
			}

			return matches;
		}

		/// <summary>
		/// Swap two elements and return the matches that would occur.
		/// </summary>
		/// <remarks>
		/// Note that this does not check if the swap is valid.
		/// Match Group returned may be empty.
		/// </remarks>
		/// <param name="pos1">Position of first element.</param>
		/// <param name="pos2">Position of second element.</param>
		/// <param name="minMatchLength">Override for minimum length of a match.</param>
		/// <param name="matchDirections">Match directions to check.</param>
		/// <returns>Match groups that would occur after the swap.</returns>
		std::vector<MisketMatchGroup> SwapAndGetMatches(MisketPosition pos1, MisketPosition pos2, unsigned int minMatchLength = -1, MatchDirections matchDirections = MatchDirections())
		{
			return SwapAndGetMatches(pos1.row, pos1.column, pos2.row, pos2.column, minMatchLength, matchDirections);
		}

		/// <summary>
		/// Pop the specified miskets from the table and collapse the columns.
		/// </summary>
		/// <param name="positions">The positions of the miskets to pop.</param>
		/// <param name="possibleRandomValues">Possible values for the new miskets.</param>
		void PopMiskets(const std::vector<MisketPosition>& positions)
		{
			// Let's mark the positions of the miskets to be popped
			std::vector<std::vector<bool>> marked(GetRowCount(), std::vector<bool>(GetColumnCount(), false));
			for (const auto& pos : positions)
			{
				if (pos.row < GetRowCount() && pos.column < GetColumnCount())
					marked[pos.row][pos.column] = true;
			}

			// Collapse the marked columns
			for (Scalar col = 0; col < GetColumnCount(); ++col)
			{
				// Collect non-marked values from bottom to top
				std::vector<T> newColumn;
				for (Scalar row = GetRowCount(); row-- > 0;)
				{
					if (!marked[row][col])
						newColumn.push_back((*this)(row, col));
				}

				// Fill the column with the new values, bottom to top
				Scalar index = 0;
				for (Scalar row = GetRowCount(); row-- > 0;)
				{
					if (index < newColumn.size())
					{
						(*this)(row, col) = newColumn[index++];
					}
					else
					{
						// Generate new misket at the top
						(*this)(row, col) = GenerateRandomMisket();
					}
				}
			}
		}

		/// <summary>
		/// Pop the specified miskets from the table and collapse the columns.
		/// </summary
		/// <param name="matchGroups">The match groups to pop.</param>
		void PopMisketMatchGroups(const std::vector<MisketMatchGroup>& matchGroups)
		{
			for (const auto& group : matchGroups)
			{
				PopMiskets(group);
			}
		}

	private:
		/// <summary>
		/// Get the index of a misket in the data vector based on its row and column.
		/// </summary>
		/// <param name="row">Row index</param>
		/// <param name="column">Column index</param>
		/// <returns>The index of the misket in the data vector.</returns>
		S Index(S row, S column) const
		{
			return row * columnCount + column;
		}

		/// <summary>
		/// Get the index of a misket in the data vector based on its position.
		/// </summary>
		/// <param name="pos">The position of the misket.</param>
		/// <returns>The index of the misket in the data vector.</returns>
		S Index(MisketPosition pos) const
		{
			return pos.row * columnCount + pos.column;
		}
	};
}
