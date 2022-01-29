cmake_minimum_required(VERSION 3.10)
project(chessarbiter)

# Shared library
add_library(chessarbiter SHARED src/Piece.cpp src/Board.cpp src/Fen.cpp src/ChessArbiter.cpp)
include_directories(src/)

# Includes
set(CHESSARBITER_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/includes) # For conveniance
set(CHESSARBITER_INCLUDE_DIR ${CHESSARBITER_INCLUDE_DIR} PARENT_SCOPE) # To be used by other projects with add_subdirectory()
file(MAKE_DIRECTORY ${CHESSARBITER_INCLUDE_DIR})
configure_file(src/Piece.hpp ${CHESSARBITER_INCLUDE_DIR}/pgnp.hpp COPYONLY)
configure_file(src/Board.hpp ${CHESSARBITER_INCLUDE_DIR} COPYONLY)
configure_file(src/Fen.hpp ${CHESSARBITER_INCLUDE_DIR} COPYONLY)
configure_file(src/ChessArbiter.hpp ${CHESSARBITER_INCLUDE_DIR} COPYONLY)

# Unit tests
enable_testing()
add_subdirectory(./tests)
