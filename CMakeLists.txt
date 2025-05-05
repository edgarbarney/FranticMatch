# FranticDreamer 2025
# CMakeLists file for CMake Configuration

# We can lower this
cmake_minimum_required (VERSION 3.21)

set (CMAKE_CXX_STANDARD 23) # Set C++23 Standard

set (CURDIR ${CMAKE_CURRENT_SOURCE_DIR})

project ("FranticMatch")

set (PROJECT_NAME "FranticMatch")
set (PROJECT_TITLE "FranticDreamer's Match X Engine")
set (PROJECT_VERSION "0.1.0")
string (TIMESTAMP TODAY "%d/%m/%Y")
set (PROJECT_BUILD_DATE ${TODAY})

if (CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 Bit
set (PROJECT_ARCH "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 Bit
set (PROJECT_ARCH "x86")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set (CMAKE_INCLUDE_CURRENT_DIR ON)

# Debug
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Windows only pre-processor defines
if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_DEPRECATE)
	
	add_compile_definitions(_USE_MATH_DEFINES)

	# Unicode Program
	add_compile_definitions(_UNICODE UNICODE)
endif()

include ("FranticMatch/Files.cmake")
include ("FranticMatch_TestGame/Files.cmake")

# FranticMatch Test Game
add_executable (FranticMatch_TestGame ${FRANTICMATCH_TESTGAME_SOURCEFILES})

