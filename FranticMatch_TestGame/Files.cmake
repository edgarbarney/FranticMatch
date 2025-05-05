# FranticDreamer 2025

# ---
# FranticMatch Engine Test Files
# ---

set (FRANTICMATCH_TESTGAME_INCLUDEDIR "FranticMatch_TestGame/Source")
set (FRANTICMATCH_TESTGAME_SOURCEDIR "FranticMatch_TestGame/Source")

# Header files
set (GLOB FRANTICMATCH_TESTGAME_HEADERFILES

	${FRANTICMATCH_TESTGAME_SOURCEDIR}/ConsoleColours.hpp
	${FRANTICMATCH_TESTGAME_SOURCEDIR}/Game/Game.hpp
	${FRANTICMATCH_TESTGAME_SOURCEDIR}/Game/ColourfulMisket/ColourfulMisket.hpp
	)

# Source files
file (GLOB FRANTICMATCH_TESTGAME_SOURCEFILES

	${FRANTICMATCH_TESTGAME_SOURCEDIR}/Main.cpp
	${FRANTICMATCH_TESTGAME_SOURCEDIR}/Game/Game.cpp
	${FRANTICMATCH_TESTGAME_SOURCEDIR}/Game/ColourfulMisket/ColourfulMisket.cpp
	)