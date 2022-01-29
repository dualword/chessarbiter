# Configure catch3
include_directories(./catch3/)
add_library(catch3 SHARED ./catch3/catch_amalgamated.cpp)

# Add tests
add_executable(fen_tests fen.cpp)
target_link_libraries(fen_tests chessarbiter catch3)
add_test(Fen_Test fen_tests)

add_executable(board_tests board.cpp)
target_link_libraries(board_tests chessarbiter catch3)
add_test(Board_Test board_tests)

add_executable(chessarbiter_tests chessarbiter.cpp)
target_link_libraries(chessarbiter_tests chessarbiter catch3)
add_test(ChessArbiter_Test chessarbiter_tests)

