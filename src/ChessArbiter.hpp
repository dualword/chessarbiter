#pragma once
#include "Board.hpp"
#include "Fen.hpp"
#include <iostream>
#include <unordered_map>

#define INIT_BACKUP()                                                          \
  FEN fen_backup = fen;                                                        \
  char positions_backup = 0;                                                   \
  if (positions.find(fen.board) != positions.end())                            \
    positions_backup = positions[fen.board];                                   \
  std::string SAN_backup = SAN;                                                \
  char capture_backup = capture;                                               \
  bool was_enpassant_backup = was_enpassant;

#define RESTORE_BACKUP()                                                       \
  SetFEN(fen_backup);                                                          \
  if (positions_backup != 0)                                                   \
    positions[fen.board] = positions_backup;                                   \
  SAN = SAN_backup;                                                            \
  capture = capture_backup;                                                    \
  was_enpassant = was_enpassant_backup;

namespace chessarbiter {
class ChessArbiter {
  Board board;
  FEN fen;
  int wPawn, wRook, wKnight, wBishop, wQueen, wKing;
  /// @brief Use to compute occurences of positions
  std::unordered_map<std::string, char> positions;
  /// @brief FEN methods used internally
  void SetFEN(std::string);
  void SetFEN(FEN);
  std::string SAN, SAN_last;
  char capture;
  bool was_enpassant;

public:
  ChessArbiter();
  void Setup(const std::string &fen);
  std::string GetFEN();
  /// @brief Check which player is going to play
  bool IsBlackTurn();
  /// @brief Check if a side is in check
  bool IsCheck(bool);
  /// @brief Play a move (return false if it's illegal)
  bool Play(const std::string &move, char promote='Q');
  /// @brief Check if a square is attacked by a particular player
  bool IsAttacked(const std::string &square, bool);
  /// @brief Get the serialized board
  std::string GetBoard();
  /// @brief Get current position evaluation according to player's material
  int GetMaterialScore();
  /// @brief Check if position is legal to be played
  bool IsPlayable();
  /// @brief Get pieces captures by a player
  std::string GetCaptures(bool);
  char GetCapture();
  /// @brief Get the english SAN format of the last move
  std::string GetSAN();
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
  bool WasEnPassant();
  std::string ParseSAN(const std::string &SANMove);
  char ParseSANPromotion(const std::string &SANMove);
};
} // namespace chessarbiter
