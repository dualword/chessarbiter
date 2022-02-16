#include "ChessArbiter.hpp"
#include <catch_amalgamated.hpp>

using namespace chessarbiter;

TEST_CASE("Setup/GetBoard", "[chessarbiter/Setup/GetBoard]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetBoard() == "rnbqkbnr"
                        "pppppppp"
                        "        "
                        "        "
                        "        "
                        "        "
                        "PPPPPPPP"
                        "RNBQKBNR");

  a.Setup("RnbqkbnR/pppppppp/8/8/8/8/PPPPPPPP/rNBQKBNr w KQkq - 0 1");
  CHECK(a.GetBoard() == "RnbqkbnR"
                        "pppppppp"
                        "        "
                        "        "
                        "        "
                        "        "
                        "PPPPPPPP"
                        "rNBQKBNr");
}

TEST_CASE("GetMaterialScore", "[chessarbiter/GetMaterialScore]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 0);

  // White better
  a.Setup("1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 5);
  a.Setup("r1bqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 3);
  a.Setup("rnbqkbnr/1ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 1);
  a.Setup("rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 9);
  a.Setup("rnbqk1nr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 3);
  a.Setup("rnbq1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 0);

  // Black better
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/1NBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == -5);
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == -3);
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == -1);
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == -9);
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN1QKBNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == -3);
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1BNR w KQkq - 0 1");
  CHECK(a.GetMaterialScore() == 0);
}

TEST_CASE("GetCaptures", "[board/GetCaptures]") {
  ChessArbiter a;

  // White captures
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "");
  a.Setup("rnbqkbnr/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "pppppppp");
  a.Setup("1nbqkbn1/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "rr");
  a.Setup("r1bqkb1r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "nn");
  a.Setup("rnbqk1nr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "b");
  a.Setup("rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "q");
  a.Setup("rnbq1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "k"); // :D

  // Black captures
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "PPPPPPPP");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/1NBQKBN1 w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "RR");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R1BQKB1R w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "NN");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNbQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "B");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "Q");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1BNR w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "K"); // :D

  // Just because we know the order of the implementation
  a.Setup("11bqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.GetCaptures(false) == "rn");
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKB11 w KQkq - 0 1");
  CHECK(a.GetCaptures(true) == "RN");
}

TEST_CASE("IsAttacked", "[chessarbiter/IsAttacked]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  // White third rank attacked by white
  CHECK(a.IsAttacked("a3", false));
  CHECK(a.IsAttacked("b3", false));
  CHECK(a.IsAttacked("c3", false));
  CHECK(a.IsAttacked("d3", false));
  CHECK(a.IsAttacked("e3", false));
  CHECK(a.IsAttacked("f3", false));
  CHECK(a.IsAttacked("g3", false));
  CHECK(a.IsAttacked("h3", false));
  // White third rank attacked by black
  CHECK_FALSE(a.IsAttacked("a3", true));
  CHECK_FALSE(a.IsAttacked("b3", true));
  CHECK_FALSE(a.IsAttacked("c3", true));
  CHECK_FALSE(a.IsAttacked("d3", true));
  CHECK_FALSE(a.IsAttacked("e3", true));
  CHECK_FALSE(a.IsAttacked("f3", true));
  CHECK_FALSE(a.IsAttacked("g3", true));
  CHECK_FALSE(a.IsAttacked("h3", true));

  // Black sixth rank attacked by black
  CHECK(a.IsAttacked("a6", true));
  CHECK(a.IsAttacked("b6", true));
  CHECK(a.IsAttacked("c6", true));
  CHECK(a.IsAttacked("d6", true));
  CHECK(a.IsAttacked("e6", true));
  CHECK(a.IsAttacked("f6", true));
  CHECK(a.IsAttacked("g6", true));
  CHECK(a.IsAttacked("h6", true));
  // Black sixth rank attacked by white
  CHECK_FALSE(a.IsAttacked("a6", false));
  CHECK_FALSE(a.IsAttacked("b6", false));
  CHECK_FALSE(a.IsAttacked("c6", false));
  CHECK_FALSE(a.IsAttacked("d6", false));
  CHECK_FALSE(a.IsAttacked("e6", false));
  CHECK_FALSE(a.IsAttacked("f6", false));
  CHECK_FALSE(a.IsAttacked("g6", false));
  CHECK_FALSE(a.IsAttacked("h6", false));

  // Remove a pawn for black
  a.Setup("rnbqkbnr/1ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.IsAttacked("a3", false));
  CHECK(a.IsAttacked("a3", true));
  CHECK_FALSE(a.IsAttacked("a4", false));
  CHECK(a.IsAttacked("a4", true));

  // Remove another pawn for black
  a.Setup("rnbqkbnr/pppp1ppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.IsAttacked("a3", true));
  CHECK(a.IsAttacked("h4", true));
  CHECK(a.IsAttacked("a3", false));
  CHECK_FALSE(a.IsAttacked("h4", false));

  // Add a crazy black knight
  a.Setup("rnbqkbnr/pppppppp/8/8/8/4n3/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.IsAttacked("d1", true));
  CHECK_FALSE(a.IsAttacked("e1", true));
  CHECK(a.IsAttacked("f1", true));
  CHECK_FALSE(a.IsAttacked("f2", true));
  CHECK_FALSE(a.IsAttacked("d1", false)); // White can't attack is own pieces
}

TEST_CASE("ListLegalMoves", "[chessarbiter/ListLegalMoves]") {
  ChessArbiter a;

  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  std::vector<std::string> moves = a.ListLegalMoves(false);
  REQUIRE(moves.size() == 20);
  CHECK(std::find(moves.begin(), moves.end(), "e2e4") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "e2e3") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "b1c3") != moves.end());
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "d1d3") != moves.end());

  // White Short Castle possible
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK11R w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  REQUIRE(moves.size() == 22);
  CHECK(std::find(moves.begin(), moves.end(), "O-O") != moves.end());

  // White Short Castle impossible
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK11R w Qkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O") != moves.end());

  // White Short Castle impossible 2
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK1NR w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O") != moves.end());

  // White Short Castle impossible 3 (queen attacks by black)
  a.Setup("rnbqkbnr/pppppqpp/8/8/8/8/PPPPP1PP/RNBQK11R w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O") != moves.end());

  // White Long Castle possible
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KNBR w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  REQUIRE(moves.size() == 23);
  CHECK(find(moves.begin(), moves.end(), "O-O-O") != moves.end());

  // White Long Castle impossible
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w Kkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O-O") != moves.end());

  // White Long Castle impossible 2
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN2KBNR w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O-O") != moves.end());

  // White Long Castle impossible 3 (rook attacks by black)
  a.Setup("rnbqkbnr/pprppppp/8/8/8/8/PP1PPPPP/R3KBNR w KQkq - 0 1");
  moves = a.ListLegalMoves(false);
  CHECK_FALSE(std::find(moves.begin(), moves.end(), "O-O-O") != moves.end());
}

TEST_CASE("IsPlayable", "[chessarbiter/IsPlayable]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK(a.IsPlayable());

  a.Setup("8/8/1q1k4/8/8/8/5K2/8 w - - 0 1");
  CHECK(a.IsPlayable());

  a.Setup("8/8/3k4/3q4/8/8/3K4/8 b - - 0 1");
  CHECK_FALSE(a.IsPlayable());

  a.Setup("8/8/3k4/3q4/8/8/3R4/8 b - - 0 1");
  CHECK_FALSE(a.IsPlayable());

  a.Setup("8/8/3k4/3q4/8/8/5K2/4K3 b - - 0 1");
  CHECK_FALSE(a.IsPlayable());

  a.Setup("1k6/8/3k4/3q4/8/8/5K2/8 b - - 0 1");
  CHECK_FALSE(a.IsPlayable());
}

TEST_CASE("Play Basic", "[chessarbiter/Play]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  // White turn
  CHECK(a.Play("e2e3"));
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
  CHECK_FALSE(a.Play("d2d3")); // Black turn
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

  // Black turn
  CHECK(a.Play("e7e5"));
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
  CHECK_FALSE(a.Play("d7d6")); // White turn
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

  // White turn
  CHECK(a.Play("b1c3"));
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppp1ppp/8/4p3/8/2N1P3/PPPP1PPP/R1BQKBNR b KQkq - 1 2");
}

TEST_CASE("IsCheckmate", "[chessarbiter/IsCheckmate]") {
  ChessArbiter a;

  // There is no checkmate
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK_FALSE(a.IsCheckMate());
  // Ensure fen did not change
  CHECK(a.GetFEN() ==
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  // There is a checkmate
  a.Setup("r1bqkbnr/1ppp1Qpp/p1n5/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
  CHECK(a.IsCheckMate());
  CHECK(a.GetFEN() ==
        "r1bqkbnr/1ppp1Qpp/p1n5/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");

  // There is a checkmate
  a.Setup("1nb1kbnr/1p3ppp/2p1p3/3p4/2P4B/2qP4/3RPPPP/1r2KBNR w Kk - 0 19");
  CHECK(a.IsCheckMate());
  CHECK(a.GetFEN() ==
        "1nb1kbnr/1p3ppp/2p1p3/3p4/2P4B/2qP4/3RPPPP/1r2KBNR w Kk - 0 19");

  // There is no checkmate
  a.Setup("1nb1kbnr/1p3ppp/2p1p3/3p4/1QP4B/2qP4/3RPPPP/r3KBNR w Kk - 2 18");
  CHECK_FALSE(a.IsCheckMate());
  CHECK(a.GetFEN() ==
        "1nb1kbnr/1p3ppp/2p1p3/3p4/1QP4B/2qP4/3RPPPP/r3KBNR w Kk - 2 18");

  // There is a checkmate
  a.Setup("r3qbr1/p1p1pkp1/1p2p1p1/8/8/8/PPPPP1PP/RNBQ1RK1 b - - 1 1");
  CHECK(a.IsCheckMate());
  CHECK(a.GetFEN() ==
        "r3qbr1/p1p1pkp1/1p2p1p1/8/8/8/PPPPP1PP/RNBQ1RK1 b - - 1 1");
}

TEST_CASE("IsDrawByFiftyMoveRule", "[chessarbiter/IsDrawByFiftyMoveRule]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CHECK_FALSE(a.IsDrawByFiftyMoveRule());

  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 99 1");
  CHECK_FALSE(a.IsDrawByFiftyMoveRule());
  a.Play("b1c3");
  CHECK(a.IsDrawByFiftyMoveRule());

  a.Setup("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1");
  CHECK(a.IsDrawByFiftyMoveRule());
}

TEST_CASE("IsDrawByNoMoves", "[chessarbiter/IsDrawByNoMoves]") {
  ChessArbiter a;

  // No move for black
  a.Setup("8/8/8/8/8/8/5K1p/7k b - - 0 1");
  CHECK(a.IsDrawByNoMoves());

  // But move for white
  a.Setup("8/8/8/8/8/8/5K1p/7k w - - 0 1");
  CHECK_FALSE(a.IsDrawByNoMoves());

  // No move for white
  a.Setup("8/8/7r/2K5/b7/2k5/6q1/8 w - - 0 1");
  CHECK(a.IsDrawByNoMoves());

  // But move for black
  a.Setup("8/8/7r/2K5/b7/2k5/6q1/8 b - - 0 1");
  CHECK_FALSE(a.IsDrawByNoMoves());
}

TEST_CASE("IsDrawByRepetitions", "[chessarbiter/IsDrawByRepetitions]") {
  ChessArbiter a;
  // One time
  a.Setup("8/3kp3/8/8/4P3/3K4/8/8 w - - 0 1");

  a.Play("d3d4");
  a.Play("d7d6");
  CHECK_FALSE(a.IsDrawByRepetitions());

  // Two time
  a.Play("d4d3");
  a.Play("d6d7");
  CHECK_FALSE(a.IsDrawByRepetitions());

  a.Play("d3d4");
  a.Play("d7d6");
  CHECK_FALSE(a.IsDrawByRepetitions());

  // Three time
  a.Play("d4d3");
  a.Play("d6d7");
  CHECK(a.IsDrawByRepetitions());
}

TEST_CASE("SimpleCapture", "[SimplePieceCapture]") {
  ChessArbiter a;
  a.Setup("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

  // Ensure capture works
  a.Play("e4d5");
  CHECK(a.GetFEN() ==
        "rnbqkbnr/ppp1pppp/8/3P4/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");
}
