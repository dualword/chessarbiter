[![pipeline](https://gitlab.com/manzerbredes/chessarbiter/badges/main/pipeline.svg)](https://gitlab.com/manzerbredes/chessarbiter/-/commits/main)
[![license](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

# ChessArbiter
ChessArbiter is a library that allow you to play chess games in C++. It ensures that all the rules of classical chess are followed.

# Features
- No external dependencies
- Simple/Minimal API
- FEN parsing/serializing
- Setup a position a play moves
- Perform various tests on a position:
  - Material
  - Players in check
  - Players captures
  - Castle
  - Attacked squares
  - Checkmate
  - ...
- More features are coming soon!

# How to setup ChessArbiter
ChessArbiter can be used as a shared library in your project.
You only need to include `ChessArbiter.hpp` and linking the .so file to your executable.

# Example
Somewhere at the beginning of the file:

    #include "ChessArbiter.hpp"

Start playing:

    ChessArbiter arbiter;
    arbiter.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Now start playing!
    if(!arbiter.Play("e2e4")){
        // Handle illegal moves
    }
    if(arbiter.IsCheckmate()){
        // Game ends
    }

See `ChessArbiter.hpp` for more informations on the API.

# CMake Integration
By using the `add_subdirectory()` directive on this repository, you will be able to use the following cmake calls in your project:

    include_directories(${CHESSARBITER_INCLUDE_DIR})
    target_link_libraries(<YOUR_TARGET> chessarbiter)

