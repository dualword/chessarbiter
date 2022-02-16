cmake_minimum_required(VERSION 3.10)
project(chessarbiter)

# Shared library
add_library(chessarbiter SHARED src/Piece.cpp src/Board.cpp src/Fen.cpp src/ChessArbiter.cpp)
include_directories(src/)

# Includes
set(CHESSARBITER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src PARENT_SCOPE) # To be used by other projects with add_subdirectory()

# Unit tests
enable_testing()
add_subdirectory(./tests)
