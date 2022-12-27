#include "Piece.hpp"

namespace chessarbiter {
Piece::Piece(char c, const std::string &coord)
    : piece(c), isBlack(!isupper(c)), coord(coord) {}

std::vector<std::string> Piece::GetMoves() {
  std::vector<std::string> moves;
  char f = coord[0]; // File
  char r = coord[1]; // Rank
  if (piece == 'p' || piece == 'P') {
    char side = 1;
    if (piece == 'p') {
      side = -1;
    }
    // First two steps
    if ((r == '2' && piece == 'P') || (r == '7' && piece == 'p')) {
      moves.push_back(std::string() + f + (char)(r + 2 * side));
    }
    if ((piece == 'P' && r < '8') || (piece == 'p' && r > '1')) {
      moves.push_back(std::string() + f + (char)(r + 1 * side));
    }
    ADD_MOVE(f - 1, r + 1 * side);
    ADD_MOVE(f + 1, r + 1 * side);
  } else if (piece == 'k' || piece == 'K') {
    ADD_MOVE(f, r - 1);
    ADD_MOVE(f, r + 1);
    ADD_MOVE(f + 1, r);
    ADD_MOVE(f - 1, r);
    ADD_MOVE(f + 1, r + 1);
    ADD_MOVE(f - 1, r - 1);
    ADD_MOVE(f + 1, r - 1);
    ADD_MOVE(f - 1, r + 1);
  } else if (piece == 'n' || piece == 'N') {
    ADD_MOVE(f + 1, r + 2);
    ADD_MOVE(f - 1, r + 2);
    ADD_MOVE(f + 1, r - 2);
    ADD_MOVE(f - 1, r - 2);
    ADD_MOVE(f + 2, r + 1);
    ADD_MOVE(f - 2, r + 1);
    ADD_MOVE(f + 2, r - 1);
    ADD_MOVE(f - 2, r - 1);
  } else {
    if (piece == 'b' || piece == 'B' || piece == 'Q' || piece == 'q') {
      char rtmp = r;
      char ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp++;
        rtmp++;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp--;
        rtmp--;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp--;
        rtmp++;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp++;
        rtmp--;
        ADD_MOVE(ftmp, rtmp);
      }
    }
    if (piece == 'r' || piece == 'R' || piece == 'Q' || piece == 'q') {
      char rtmp = r;
      char ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        rtmp++;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        rtmp--;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp++;
        ADD_MOVE(ftmp, rtmp);
      }
      rtmp = r;
      ftmp = f;
      while (IS_FR_VALID(ftmp, rtmp)) {
        ftmp--;
        ADD_MOVE(ftmp, rtmp);
      }
    }
  }
  return (moves);
}
} // namespace chessarbiter