#include "Board.hpp"
#include "Fen.hpp"
#include <iostream>
#include <unordered_map>

namespace chessarbiter {
class ChessArbiter {
  Board board;
  FEN fen;
  FEN fen_last; // To undo a move
  int wPawn, wRook, wKnight, wBishop, wQueen, wKing;
  /// @brief Use to compute occurences of positions
  std::unordered_map<std::string, char> positions;
  /// @brief FEN methods used internally
  void SetFEN(std::string);
  void SetFEN(FEN);

public:
  ChessArbiter();
  void Setup(std::string);
  std::string GetFEN();
  /// @brief Check which player is going to play
  bool IsBlackTurn();
  /// @brief Check if a side is in check
  bool IsCheck(bool);
  /// @brief Play a move (return false if it's illegal)
  bool Play(std::string);
  /// @brief Check if a square is attacked by a particular player
  bool IsAttacked(std::string, bool);
  /// @brief Get the serialized board
  std::string GetBoard();
  /// @brief Get current position evaluation according to player's material
  int GetMaterialScore();
  /// @brief Check if position is legal to be played
  bool IsPlayable();
  /// @brief Get pieces captures by a player
  std::string GetCaptures(bool);
  /// @brief List all the legal moves of a player
  std::vector<std::string> ListLegalMoves(bool);
  /// @brief Check if a specific castle is possible by a player
  bool IsCastlePossible(bool, bool);
  bool IsCheckMate();
  /// @brief Draws check
  bool IsDrawByFiftyMoveRule();
  bool IsDrawByNoMoves();
  bool IsDrawByRepetitions();
  bool IsDraw();
};
} // namespace chessarbiter