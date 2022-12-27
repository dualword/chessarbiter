#include "ChessArbiter.hpp"

namespace chessarbiter {
ChessArbiter::ChessArbiter()
    : wPawn(1), wRook(5), wKnight(3), wBishop(3), wQueen(9), wKing(0), SAN(""),
      capture(' '), was_enpassant(false) {
  Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void ChessArbiter::Setup(const std::string fen) {
  positions.clear();
  SetFEN(fen);
  positions[this->fen.board] = 1;
}

void ChessArbiter::SetFEN(FEN fen) { SetFEN(FENParser::Serialize(fen)); }

void ChessArbiter::SetFEN(std::string newfen) {
  fen = FENParser::Parse(newfen);

  board.Clear();
  for (int i = 0; i < 64; i++) {
    if (fen.board[i] != ' ') {
      char f = 'a' + ((i) % 8);
      char r = '8' - ((i) / 8);
      board.AddPiece(fen.board[i], f + std::string() + r);
    }
  }
}

std::string ChessArbiter::GetFEN() { return (FENParser::Serialize(fen)); }

std::string ChessArbiter::GetBoard() { return (fen.board); }

bool ChessArbiter::IsBlackTurn() { return (fen.player); }

bool ChessArbiter::IsCheck(bool isBlack) {
  std::string kingloc = board.GetKingLocation(isBlack);
  return (IsAttacked(kingloc, !isBlack));
}

bool ChessArbiter::Play(const std::string move, const char promote) {
  std::vector<std::string> moves = ListLegalMoves(fen.player);
  if (find(moves.begin(), moves.end(), move) != moves.end()) {
    Piece moved = board.GetPieceAt(move.substr(0, 2)); // This call never fail
    std::string src = move.substr(0, 2);
    std::string dst = move.substr(2, 2);
    bool IsCapture = !board.IsEmpty(dst);
    FEN newFen = fen;
    INIT_BACKUP();
    SAN = "";
    capture = ' ';
    was_enpassant=false;

    if (IsCapture) {
      capture = board.GetPieceAt(dst).piece;
    }

    // Perform the move
    if ((moved.piece == 'k' || moved.piece == 'K') && (move == "e8g8" ||
        move == "e8c8" || move == "e1g1" || move == "e1c1")) {
      if (move == "e8g8") {
        board.Move("e8g8");
        board.Move("h8f8");
        SAN = "O-O";
      } else if (move == "e8c8") {
        board.Move("e8c8");
        board.Move("a8d8");
        SAN = "O-O-O";
      } else if (move == "e1g1") {
        board.Move("e1g1");
        board.Move("h1f1");
        SAN = "O-O";
      } else {
        board.Move("e1c1");
        board.Move("a1d1");
        SAN = "O-O-O";
      }
    } else {
      // Update SAN move
      if (moved.piece == 'p' || moved.piece == 'P') {
        if (IsCapture) {
          SAN = src[0];
          SAN += "x" + dst;
        } else {
          SAN = dst;
        }
      } else {
        SAN = std::toupper(moved.piece);
        if (!board.IsPieceMoveUnique(moved.piece, dst)) {
          if (src[0] == dst[0]) {
            SAN += src[1];
          } else {
            SAN += src[0];
          }
        }
        if (IsCapture) {
          SAN += "x";
        }
        SAN += dst;
      }
      // Perform the move
      board.Move(move);
    }

    // Update halfmove
    newFen.halfmove++;
    // Check enpassant
    newFen.en_passant = "-";
    if (moved.piece == 'p' || moved.piece == 'P') {
      if (fen.player && (src[1] - dst[1] == 2)) {
        newFen.en_passant = src[0] + std::string() + (char)(src[1] - 1);
      } else if (!fen.player && (dst[1] - src[1] == 2)) {
        newFen.en_passant = src[0] + std::string() + (char)(src[1] + 1);
      }
      if (dst == fen.en_passant) {
        if (fen.player) {
          board.RemovePiece(dst[0] + std::string() + (char)(dst[1] + 1));
          capture = 'P';
        } else {
          board.RemovePiece(dst[0] + std::string() + (char)(dst[1] - 1));
          capture = 'p';
        }
        was_enpassant=true;
      }
      newFen.halfmove = 0; // Pawn moves reset half moves
    }
    // Check pawn promotion
    if(moved.piece == 'p' || moved.piece == 'P'){
      if(moved.piece == 'p' && dst[1]=='1'){
        board.RemovePiece(dst);
        board.AddPiece(tolower(promote),dst);
        SAN+="="+toupper(promote);
      } else if(dst[1]=='8'){
        board.RemovePiece(dst);
        board.AddPiece(toupper(promote),dst);
        SAN+="="+toupper(promote);
      }
    }
    // Captures reset half moves
    if (IsCapture) {
      newFen.halfmove = 0;
    }
    if (newFen.player) {
      newFen.move++;
    }
    newFen.board = board.Serialize();
    newFen.player = !newFen.player;
    // Castle update if one is true
    if (newFen.white_castle_long || newFen.white_castle_short ||
        newFen.black_castle_long || newFen.black_castle_short) {
      if (moved.piece == 'R' && src == "a1") {
        newFen.white_castle_long = false;
      } else if (moved.piece == 'R' && src == "h1") {
        newFen.white_castle_short = false;
      } else if (moved.piece == 'r' && src == "a8") {
        newFen.black_castle_long = false;
      } else if (moved.piece == 'r' && src == "h8") {
        newFen.black_castle_short = false;
      } else if (moved.piece == 'K' || (!fen.player && src == "O-")) {
        newFen.white_castle_long = false;
        newFen.white_castle_short = false;
      } else if (moved.piece == 'k' || (fen.player && src == "O-")) {
        newFen.black_castle_long = false;
        newFen.black_castle_short = false;
      }
    }

    // Update fen!
    fen = newFen;

    // Check for illegal move
    if (IsCheck(!fen.player)) {
      RESTORE_BACKUP();
      return (false);
    }

    // Update position map (repetitions draw)
    if (positions.count(fen.board) == 0) {
      positions[fen.board] = 1;
    } else {
      positions[fen.board] += 1;
    }

    return (true);
  }
  return (false);
}

bool ChessArbiter::WasEnPassant() { return (was_enpassant); }

bool ChessArbiter::IsAttacked(const std::string square, const bool by) {
  std::vector<std::string> moves = board.ListPossibleMoves(by);
  for (std::string &m : moves) {
    std::string src = m.substr(0, 2);
    std::string dst = m.substr(2, 2);
    if (dst == square) {
      // Pawn do not attack forward
      Piece p = board.GetPieceAt(src);
      if (p.piece == 'p' || p.piece == 'P') {
        if (src[0] != dst[0]) {
          return (true);
        }
      } else {
        return (true);
      }
    }
  }
  return (false);
}

bool ChessArbiter::IsCastlePossible(const bool isBlack, const bool isLong) {

  if (isBlack && isLong && fen.black_castle_long) {
    if (board.IsEmpty("d8") && board.IsEmpty("c8") && board.IsEmpty("b8")) {
      if (!IsAttacked("d8", false) && !IsAttacked("c8", false)) {
        return (true);
      }
    }
  } else if (isBlack && !isLong && fen.black_castle_short) {
    if (board.IsEmpty("f8") && board.IsEmpty("g8")) {
      if (!IsAttacked("f8", false) && !IsAttacked("g8", false)) {
        return (true);
      }
    }
  } else if (!isBlack && isLong && fen.white_castle_long) {
    if (board.IsEmpty("d1") && board.IsEmpty("c1") && board.IsEmpty("b1")) {
      if (!IsAttacked("d1", true) && !IsAttacked("c1", true)) {
        return (true);
      }
    }
  } else if (!isBlack && !isLong && fen.white_castle_short) {
    if (board.IsEmpty("f1") && board.IsEmpty("g1")) {
      if (!IsAttacked("f1", true) && !IsAttacked("g1", true)) {
        return (true);
      }
    }
  }

  return (false);
}

int ChessArbiter::GetMaterialScore() {
  int whiteScore = 0;
  int blackScore = 0;
  for (char i = 0; i < 2; i++) {
    int *score = &whiteScore;
    if (i > 0) {
      score = &blackScore;
    }
    for (Piece &p : board.GetPlayerPieces((bool)i)) {
      switch (tolower(p.piece)) {
      case 'p':
        (*score) += wPawn;
        break;
      case 'r':
        (*score) += wRook;
        break;
      case 'n':
        (*score) += wKnight;
        break;
      case 'b':
        (*score) += wBishop;
        break;
      case 'q':
        (*score) += wQueen;
        break;
      default:
        (*score) += wKing;
      }
    }
  }
  return (whiteScore - blackScore);
}

std::string ChessArbiter::GetCaptures(const bool isBlack) {
  std::string captures;
  // Pawn
  char p = 'P';
  if (!isBlack)
    p = 'p';
  for (char i = 8 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  // Rook
  p = 'R';
  if (!isBlack)
    p = 'r';
  for (char i = 2 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  // Knight
  p = 'N';
  if (!isBlack)
    p = 'n';
  for (char i = 2 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  // Bishop
  p = 'B';
  if (!isBlack)
    p = 'b';
  for (char i = 2 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  // Queen
  p = 'Q';
  if (!isBlack)
    p = 'q';
  for (char i = 1 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  // King :D
  p = 'K';
  if (!isBlack)
    p = 'k';
  for (char i = 1 - board.CountPiece(p); i > 0; i--) {
    captures += p;
  }
  return (captures);
}

std::vector<std::string> ChessArbiter::ListLegalMoves(const bool isBlack) {
  std::vector<std::string> moves;
  for (std::string &move : board.ListPossibleMoves(isBlack)) {
    std::string src = move.substr(0, 2);
    std::string dst = move.substr(2, 2);
    Piece srcp = board.GetPieceAt(src); // This call never fail
    bool IsDstEmpty = board.IsEmpty(dst);

    // Pawns side  moves
    if (srcp.piece == 'p' || srcp.piece == 'P') {
      if ((src[0] != dst[0])) {
        if (!IsDstEmpty) {
          Piece attacked = board.GetPieceAt(dst);
          if (srcp.isBlack != attacked.isBlack)
            moves.push_back(move);
        } else if (dst == fen.en_passant) {
          moves.push_back(move);
        }
      } else if (IsDstEmpty) {
        moves.push_back(move);
      }
    } else {
      moves.push_back(move);
    }
  }

  // Casling
  if (isBlack) {
    if (IsCastlePossible(isBlack, false)) {
      moves.push_back("e8g8");
    }
    if (IsCastlePossible(isBlack, true)) {
      moves.push_back("e8c8");
    }
  } else {
    if (IsCastlePossible(isBlack, false)) {
      moves.push_back("e1g1");
    }
    if (IsCastlePossible(isBlack, true)) {
      moves.push_back("e1c1");
    }
  }

  return (moves);
}

bool ChessArbiter::IsPlayable() {
  char nK = board.CountPiece('K');
  if (nK == 1 && nK == board.CountPiece('k')) {
    if (!IsCheck(!fen.player)) {
      return (true);
    }
  }
  return (false);
}

bool ChessArbiter::IsDrawByFiftyMoveRule() { return (fen.halfmove >= 100); }

bool ChessArbiter::IsDrawByNoMoves() {
  if (!IsCheck(fen.player)) {
    std::vector<std::string> moves = ListLegalMoves(fen.player);
    for (std::string &move : moves) {
      INIT_BACKUP();
      if (Play(move)) {
        RESTORE_BACKUP();
        return (false);
      }
    }
    return (true);
  }
  return (false);
}

bool ChessArbiter::IsDrawByRepetitions() {
  for (auto const &pos : positions) {
    if (pos.second >= 3) {
      return (true);
    }
  }
  return (false);
}

bool ChessArbiter::IsDraw() {
  return (IsDrawByFiftyMoveRule() || IsDrawByNoMoves() ||
          IsDrawByRepetitions());
}

bool ChessArbiter::IsCheckMate() {
  if (IsCheck(fen.player)) {
    std::vector<std::string> moves = ListLegalMoves(fen.player);
    for (std::string &move : moves) {
      INIT_BACKUP();
      if (Play(move)) {
        RESTORE_BACKUP();
        return (false);
      }
    }
    return (true);
  }
  return (false);
}

std::string ChessArbiter::GetSAN() { return (SAN); }
char ChessArbiter::GetCapture() { return (capture); }

std::string ChessArbiter::ParseSAN(const std::string SANMove) {
  std::string src, dst;
  char piece = ' ';
  char hint = ' ';
  bool isHintRank = false;

  // First castling
  if (SANMove[0] == 'O' || SANMove[0] == '0') {
    char c3 = (SANMove.size() >= 3) ? SANMove[3] : '?';
    // Long castle
    if (c3 == '-') {
      if (fen.player && IsCastlePossible(fen.player, true)) {
        return ("e8c8");
      } else if (IsCastlePossible(fen.player, true)) {
        return ("e1c1");
      }
    } else {
      if (fen.player && IsCastlePossible(fen.player, false)) {
        return ("e8g8");
      } else if (IsCastlePossible(fen.player, false)) {
        return ("e1g1");
      }
    }
  }

  // First deduce dst square in the move
  if (SANMove.size() > 0) {
    // Pawn moves
    if (std::islower(SANMove[0])) {
      piece = 'P';
      hint = SANMove[0];
      // Not a capture
      if (SANMove[1] != 'x') {
        dst = SANMove.substr(0, 2);
      } else {
        dst = SANMove.substr(2, 2);
      }
    } else {
      piece = SANMove[0];
      char c1 = (SANMove.size() >= 2) ? SANMove[1] : '?';
      char c2 = (SANMove.size() >= 3) ? SANMove[2] : '?';
      char c3 = (SANMove.size() >= 4) ? SANMove[3] : '?';
      if (c1 == 'x') {
        dst = SANMove.substr(2, 2);
      } else if (c2 == 'x') {
        hint = c1;
        dst = SANMove.substr(3, 2);
      } else if (IS_DIGIT(c2)) {
        dst = SANMove.substr(1, 2);
      } else {
        hint = c1;
        dst = SANMove.substr(2, 2);
      }
    }
  }
  isHintRank = IS_DIGIT(hint);

  // Now find src thanks to legal moves
  std::vector<std::string> src_candidates;
  for (std::string &move : ListLegalMoves(fen.player)) {
    std::string current_src = move.substr(0, 2);
    std::string current_dst = move.substr(2, 2);
    if (current_dst == dst) {
      src_candidates.push_back(current_src);
    }
  }

  // Now filter the legals move
  if (src_candidates.size() > 0) {
    if (src_candidates.size() > 1) {
      std::vector<std::string> src_candidates_filtered;
      // Filter according to pieces:
      for (std::string &cand : src_candidates) {
        Piece p = board.GetPieceAt(cand); // This call never fails
        if (std::toupper(p.piece) == piece) {
          src_candidates_filtered.push_back(cand);
        }
      }
      src_candidates = src_candidates_filtered;
      src_candidates_filtered.clear();
      // Last Filtering
      if (src_candidates.size() > 1) {
        for (std::string &cand : src_candidates) {
          char cand_hint = cand[0];
          if (isHintRank) {
            cand_hint = cand[1];
          }
          if (hint == cand_hint) {
            src_candidates_filtered.push_back(cand);
          }
        }
      }
      src_candidates = src_candidates_filtered;
    }
    src = src_candidates[0];
  }

  // Ensure that we return empty string if no matches
  if (src.size() <= 0) {
    return ("");
  }
  // Else return "srcdst" string
  return (src + dst);
}

char ChessArbiter::ParseSANPromotion(const std::string SANMove){
  if(SANMove.length()>=4 && SANMove[0] - 'a' < 8 && SANMove[2]=='='){
    char p=SANMove[3]; // Must be upper
    if(p=='Q' || p=='R' || p=='B' || p=='N'){
      return p;
    }
  }
  return 'Q';
}

} // namespace chessarbiter