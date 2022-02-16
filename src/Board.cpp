#include "Board.hpp"

namespace chessarbiter {

bool Board::IsEmpty(std::string coord) {
  for (Piece &p : pieces) {
    if (p.coord == coord) {
      return (false);
    }
  }
  return (true);
}

bool Board::AddPiece(char p, std::string coord) {
  if (IsEmpty(coord)) {
    Piece piece(p, coord);
    pieces.push_back(piece);
    return (true);
  }
  return (false);
}

bool Board::RemovePiece(std::string coord) {
  for (char i = 0; i < pieces.size(); i++) {
    if (pieces[i].coord == coord) {
      pieces.erase(pieces.begin() + i);
      return (true);
    }
  }
  return false;
}

Piece Board::GetPieceAt(std::string coord) {
  for (Piece &p : pieces) {
    if (p.coord == coord)
      return p;
  }
  throw NoPieceFound();
}

std::vector<Piece> Board::GetPlayerPieces(bool isBlack) {
  std::vector<Piece> pp;
  for (Piece &p : pieces) {
    if (p.isBlack == isBlack) {
      pp.push_back(p);
    }
  }
  return (pp);
}

short Board::CountPiece(char c) {
  char count = 0;
  for (Piece &p : pieces) {
    if (p.piece == c) {
      count++;
    }
  }
  return (count);
}

void Board::Clear() { pieces.clear(); }

std::string Board::GetKingLocation(bool isBlack) {
  for (Piece &p : pieces) {
    if (p.isBlack == isBlack) {
      if (p.piece == 'k' || p.piece == 'K') {
        return (p.coord);
      }
    }
  }
  throw NoPieceFound();
}

void Board::Move(std::string move) {
  std::string src = move.substr(0, 2);
  std::string dst = move.substr(2, 2);
  RemovePiece(dst); // Remove piece on dst if exists
  for (Piece &p : pieces) {
    if (p.coord == src) {
      p.coord = dst;
      break;
    }
  }
}

bool Board::IsPieceMoveUnique(char piece, std::string move_dst) {
  bool isBlack = std::islower(piece);
  unsigned char count = 0;
  for (std::string &move : ListPossibleMoves(isBlack)) {
    std::string src = move.substr(0, 2);
    std::string dst = move.substr(2, 2);
    Piece p = GetPieceAt(src); // Never fails since it is legal
    if (p.piece == piece && move_dst == dst) {
      count++;
    }
  }
  return (count <= 1);
}

std::string Board::Serialize() {
  std::string s;
  for (short i = 0; i < 8; i++) {
    for (short j = 0; j < 8; j++) {
      try {
        Piece p = GetPieceAt((char)('a' + j) + std::string() + (char)('8' - i));
        s += p.piece;
      } catch (...) {
        s += " ";
      }
    }
  }
  return (s);
}

bool Board::IsMovePossible(std::string move) {
  std::string src = move.substr(0, 2);
  std::string dst = move.substr(2, 2);
  if (src == dst) {
    return (false);
  }
  Piece srcp = GetPieceAt(src);

  // Check move is possible
  std::vector<std::string> srcm = srcp.GetMoves();
  if (find(srcm.begin(), srcm.end(), dst) == srcm.end()) {
    return (false);
  }

  // Check colors on dst square
  // Seems that checking that is empty
  // instead of catching NoPieceFound exception is
  // more performant
  if (!IsEmpty(dst)) {
    Piece dstp = GetPieceAt(dst);
    if (srcp.isBlack == dstp.isBlack)
      return (false);
  }

  // Check if no piece in between
  if (src[0] == dst[0] || src[1] == dst[1]) { // Rook like moves
    if (src[0] == dst[0]) {                   // Vertical
      char side = 1;
      if (src[1] > dst[1]) {
        side = -1;
      }
      char r = src[1] + 1 * side;
      while (r != dst[1]) {
        if (!IsEmpty(src[0] + std::string() + r)) {
          return (false);
        }
        r += 1 * side;
      }
    } else { // Horizontal
      char side = 1;
      if (src[0] > dst[0]) {
        side = -1;
      }
      char f = src[0] + 1 * side;
      while (f != dst[0]) {
        if (!IsEmpty(f + std::string() + src[1])) {
          return (false);
        }
        f += 1 * side;
      }
    }
  } else if ((src[0] - dst[0] == src[1] - dst[1]) ||
             (dst[0] - src[0] == src[1] - dst[1])) { // Bishop like moves
    // That code if for diagonal 1 (bottom left to up right)
    // Using the d2 variable allow to reuse the same code for the 2 diagonals
    char d2 = 1; // d2 stand for diagonal 2 (bottom right to up left)
    if ((dst[0] - src[0] == src[1] - dst[1])) { // If move is for diagonal 2
      d2 = -1;
    }

    // Move direction check
    char side = 1;
    if (src[0] > dst[0]) {
      side = -1;
    }

    // Setup variables
    char f = src[0] + 1 * side;
    char r = src[1] + d2 * side;

    // Perform empty square checks
    while (f != dst[0], r != dst[1]) {
      if (!IsEmpty(f + std::string() + r)) {
        return (false);
      }
      f += 1 * side;
      r += d2 * side;
    }
  }
  return (true);
}

std::vector<std::string> Board::ListPossibleMoves(bool isBlack) {
  std::vector<std::string> moves;
  for (Piece &p : pieces) {
    if (p.isBlack == isBlack) {
      for (std::string &m : p.GetMoves()) {
        if (IsMovePossible(p.coord + m)) {
          moves.push_back(p.coord + m);
        }
      }
    }
  }
  return (moves);
}
} // namespace chessarbiter