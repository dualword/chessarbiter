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
  bool IsEmpty(std::string);
  /// @brief Add a piece (no checks are performed on coord)
  bool AddPiece(char p, std::string);
  /// @brief Remove a piece from a square
  bool RemovePiece(std::string);
  /// @brief Get piece at a specific coordinate
  Piece GetPieceAt(std::string);
  /// @brief Get the pieces of a player
  std::vector<Piece> GetPlayerPieces(bool);
  /// @brief Count the number of a specific piece on the board
  short CountPiece(char);
  /// @brief Get the location of the first king found on the board
  std::string GetKingLocation(bool);
  /// @brief Check if a move is technically possible (does not means it is
  /// legal)
  bool IsMovePossible(std::string);
  /// @brief Clear the board
  void Clear();
  /// @brief Move a piece somewhere no matter what
  void Move(std::string);
  /// @brief Get a serialize version of the board
  std::string Serialize();
  /// @brief List all the technically possible moves of a player
  std::vector<std::string> ListPossibleMoves(bool);
};

struct NoPieceFound : public std::exception {
  const char *what() const throw() { return "No piece found"; }
};

} // namespace chessarbiter