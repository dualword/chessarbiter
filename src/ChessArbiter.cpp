#include "ChessArbiter.hpp"

namespace chessarbiter {
ChessArbiter::ChessArbiter()
    : wPawn(1), wRook(5), wKnight(3), wBishop(3), wQueen(9), wKing(0) {}

void ChessArbiter::Setup(std::string fen) {
  SetFEN(fen);
  fen_last = this->fen;
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

bool ChessArbiter::IsBlackTurn() { return (fen.player == 'b'); }

bool ChessArbiter::IsCheck(bool isBlack) {
  std::string kingloc = board.GetKingLocation(isBlack);
  return (IsAttacked(kingloc, !isBlack));
}

bool ChessArbiter::Play(std::string move) {
  std::vector<std::string> moves = ListLegalMoves(fen.player);
  if (find(moves.begin(), moves.end(), move) != moves.end()) {
    Piece moved = board.GetPieceAt(move.substr(0, 2)); // This call never fail
    std::string src = move.substr(0, 2);
    std::string dst = move.substr(2, 2);
    bool IsCapture = !board.IsEmpty(dst);
    FEN newFen = fen;

    // Perform the move
    if (move == "O-O" || move == "O-O-O") {
      if (fen.player && move == "O-O") {
        board.Move("e8g8");
        board.Move("h8e8");
      } else if (fen.player && move == "O-O") {
        board.Move("e8c8");
        board.Move("a8d8");
      } else if (!fen.player && move == "O-O") {
        board.Move("e1g1");
        board.Move("h1e1");
      } else {
        board.Move("e1c1");
        board.Move("a1d1");
      }
    } else {
      board.Move(move);
    }

    // Update halfmove
    newFen.halfmove++;
    // Check enpassant
    newFen.en_passant = "-";
    if (moved.piece == 'p' || moved.piece == 'P') {
      if (fen.player && (dst[1] - src[1] == 2)) {
        newFen.en_passant = src[0] + std::string() + (char)(src[1] - 1);
      } else if (!fen.player && (dst[1] - src[1] == 2)) {
        newFen.en_passant = src[0] + std::string() + (char)(src[1] + 1);
      }
      newFen.halfmove = 0; // Pawn moves reset half moves
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
    fen_last = fen;
    fen = newFen;

    // Check for illegal move
    if (IsCheck(!fen.player)) {
      SetFEN(fen_last);
      return (false);
    }

    return (true);
  }
  return (false);
}

bool ChessArbiter::IsAttacked(std::string square, bool by) {
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

bool ChessArbiter::IsCastlePossible(bool isBlack, bool isLong) {

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

std::string ChessArbiter::GetCaptures(bool isBlack) {
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

std::vector<std::string> ChessArbiter::ListLegalMoves(bool isBlack) {
  std::vector<std::string> moves;
  for (std::string &move : board.ListPossibleMoves(isBlack)) {
    std::string src = move.substr(0, 2);
    std::string dst = move.substr(2, 2);
    Piece srcp = board.GetPieceAt(src); // This call never fail
    bool IsDstEmpty = board.IsEmpty(dst);

    // Pawns side  moves
    if ((srcp.piece == 'p' || srcp.piece == 'P') && (src[0] != dst[0])) {
      if (!IsDstEmpty) {
        Piece attacked = board.GetPieceAt(dst);
        if (srcp.isBlack != attacked.isBlack)
          moves.push_back(move);
      } else if (dst == fen.en_passant) {
        moves.push_back(move);
      }
    } else {
      moves.push_back(move);
    }
  }

  // Casling
  if (IsCastlePossible(isBlack, false))
    moves.push_back("O-O");
  if (IsCastlePossible(isBlack, true))
    moves.push_back("O-O-O");

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

bool ChessArbiter::IsCheckMate() {
  if (IsCheck(fen.player)) {
    std::vector<std::string> moves = ListLegalMoves(fen.player);
    for(std::string &move: moves){
      if(Play(move)){
        SetFEN(fen_last);
        return(false);
      }
    }
    return(true);
  }
  return (false);
}

} // namespace chessarbiter