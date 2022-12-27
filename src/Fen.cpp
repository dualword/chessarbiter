#include "Fen.hpp"

namespace chessarbiter {

std::string FENParser::normalize_rank(const std::string &fen_rank) {
  std::string normalized;
  for (const char &c : fen_rank) {
    if (IS_DIGIT(c)) {
      for (char i = 0; i < (c - '0'); i++) {
        normalized += ' ';
      }
    } else {
      // Check validity
      char c2 = std::tolower(c);
      if (!(c2 == 'p' || c2 == 'r' || c2 == 'n' || c2 == 'b' || c2 == 'q' ||
            c2 == 'k' || c2 == ' ')) {
        throw InvalidFEN();
      }
      // Add
      normalized += c;
    }
  }
  // Check validity
  if (normalized.size() != 8) {
    throw InvalidFEN();
  }
  return (normalized);
}

char FENParser::NextToken(const std::string &fen, char loc) {
  while (loc < fen.size() && IS_BLANK(fen[loc])) {
    loc++;
  }
  return (loc);
}

char FENParser::NextRank(const std::string &fen, char loc) {
  loc++;
  while (loc < fen.size() && fen[loc] != '/' && fen[loc] != ' ') {
    loc++;
  }
  return (loc);
}

std::string FENParser::Serialize(const FEN &fen) {
  std::string s;
  char skip = 0;
  char rank = 0;
  for (const char &c : fen.board) {
    rank++;
    if (c == ' ') {
      skip++;
    } else {
      if (skip > 0) {
        s += std::to_string(skip);
        skip = 0;
      }
      s += c;
    }
    if (rank == 8) {
      if (skip != 0) {
        s += std::to_string(skip);
        skip = 0;
      }
      s += '/';
      rank = 0;
    }
  }

  // Remove last /
  s = s.substr(0, s.size() - 1);
  s += " ";

  // Player
  if (fen.player) {
    s += "b ";
  } else {
    s += "w ";
  }

  // Castle
  if (!(fen.white_castle_short || fen.white_castle_long ||
        fen.black_castle_short || fen.black_castle_long)) {
    s += "-";
  } else {
    if (fen.white_castle_short) {
      s += "K";
    }
    if (fen.white_castle_long) {
      s += "Q";
    }
    if (fen.black_castle_short) {
      s += "k";
    }
    if (fen.black_castle_long) {
      s += "q";
    }
  }

  // Remaining
  s += " " + fen.en_passant + " " + std::to_string(fen.halfmove) + " " +
       std::to_string(fen.move);

  return (s);
}

FEN FENParser::Parse(const std::string &fen) {
  FEN parsed;

  // Parse board
  char loc = 0;
  for (char rank = 0; rank < 8; rank++) {
    CHECK_LOC();
    char newloc = NextRank(fen, loc);
    parsed.board += normalize_rank(fen.substr(loc, newloc - loc));
    loc = newloc + 1;
  }

  // Parse player to move
  loc = NextToken(fen, loc);
  if (!(fen[loc] == 'w' || fen[loc] == 'b')) {
    throw InvalidFEN();
  }
  parsed.player = fen[loc] == 'b';
  CHECK_LOC();

  // Parse castling
  loc = NextToken(fen, loc + 1);
  char length = 0;
  char cur_loc = loc;
  while (!IS_BLANK(fen[cur_loc])) {
    length++;
    cur_loc++;
    CHECK_LOC();
  }
  parsed.white_castle_short = false;
  parsed.white_castle_long = false;
  parsed.black_castle_short = false;
  parsed.black_castle_long = false;
  std::string castle = fen.substr(loc, length);
  for (char i = 0; i < length; i++) {
    switch (fen[loc + i]) {
    case 'K':
      parsed.white_castle_short = true;
      break;
    case 'Q':
      parsed.white_castle_long = true;
      break;
    case 'k':
      parsed.black_castle_short = true;
      break;
    case 'q':
      parsed.black_castle_long = true;
      break;
    case '-':
      break;
    default:
      throw InvalidFEN();
    }
  }

  // Parse en passant
  loc = NextToken(fen, loc + length);
  if (fen[loc] != '-') {
    parsed.en_passant = fen.substr(loc, 2);
    loc++;
    // Check format
    char f = parsed.en_passant[0];
    char r = parsed.en_passant[1];
    if (!((f >= 'a' && f <= 'h') && (r >= '1' && r <= '8'))) {
      throw InvalidFEN();
    }
  }
  loc++;
  CHECK_LOC();

  // Parse half move counter
  loc = NextToken(fen, loc);
  std::string halfmove;
  while (!IS_BLANK(fen[loc])) {
    if (!IS_DIGIT(fen[loc])) {
      throw InvalidFEN();
    }
    halfmove += fen[loc];
    loc++;
    CHECK_LOC();
  }
  parsed.halfmove = stoi(halfmove);

  // Parse move counter
  loc = NextToken(fen, loc);
  std::string move;
  while (loc < fen.size() && !IS_BLANK(fen[loc])) {
    if (!IS_DIGIT(fen[loc])) {
      throw InvalidFEN();
    }
    CHECK_LOC();
    move += fen[loc];
    loc++;
  }
  parsed.move = stoi(move);

  return (parsed);
}

} // namespace chessarbiter