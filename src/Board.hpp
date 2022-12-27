#include "Piece.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

namespace chessarbiter {

class Board {
  std::vector<Piece> pieces;

public:
  /// @brief Check if a square is empty
  bool IsEmpty(const std::string &coord);
  /// @brief Add a piece (no checks are performed on coord)
  bool AddPiece(char p, const std::string &coord);
  /// @brief Remove a piece from a square
  bool RemovePiece(const std::string &coord);
  /// @brief Get piece at a specific coordinate
  Piece GetPieceAt(const std::string &coord);
  /// @brief Get the pieces of a player
  std::vector<Piece> GetPlayerPieces(bool);
  /// @brief Count the number of a specific piece on the board
  short CountPiece(char);
  /// @brief Return true if at most 1 similar piece can go to move_dst
  bool IsPieceMoveUnique(char piece, const std::string &move_dst);
  /// @brief Get the location of the first king found on the board
  std::string GetKingLocation(bool);
  /// @brief Check if a move is technically possible (does not means it is
  /// legal)
  bool IsMovePossible(const std::string &move);
  /// @brief Clear the board
  void Clear();
  /// @brief Move a piece somewhere no matter what
  void Move(const std::string &move);
  /// @brief Get a serialize version of the board
  std::string Serialize();
  /// @brief List all the technically possible moves of a player
  std::vector<std::string> ListPossibleMoves(bool);
};

struct NoPieceFound : public std::exception {
  const char *what() const throw() { return "No piece found"; }
};

} // namespace chessarbiter