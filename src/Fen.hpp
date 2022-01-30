#include <iostream>
#include <sstream>
#include <string>

#define FEN__IS_DIGIT(c)                                                            \
  (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||     \
   c == '6' || c == '7' || c == '8' || c == '9')
#define FEN__IS_BLANK(c) (c == ' ' || c == '\n' || c == '\t' || c == '\r')
#define FEN__CHECK_LOC() {if(loc>=fen.size()){throw InvalidFEN();}}

namespace chessarbiter {

class FEN {
public:
  std::string board;
  bool player;
  bool white_castle_short;
  bool white_castle_long;
  bool black_castle_short;
  bool black_castle_long;
  std::string en_passant;
  short halfmove;
  short move;
  FEN()
      : board(""), player(false), white_castle_short(true),
        white_castle_long(true), black_castle_short(true),
        black_castle_long(true), en_passant("-"), halfmove(0), move(1) {}
};

class FENParser {
private:
  static std::string normalize_rank(std::string fen_rank);
  static char NextToken(std::string fen, char loc);
  static char NextRank(std::string fen, char loc);

public:
  /// @brief Parse a FEN from a string (can throw InvalidFEN)
  static FEN Parse(std::string);
  /// @brief Generate a fen string from the FEN object
  static std::string Serialize(FEN fen);
};

struct InvalidFEN : public std::exception {
  const char *what() const throw() { return "No piece found"; }
};
} // namespace chessarbiter
