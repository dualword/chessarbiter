#include "Fen.hpp"
#include <catch_amalgamated.hpp>

using namespace chessarbiter;

TEST_CASE("Serializer", "[fen/serialize]") {
  FEN f;
  f.board = "p    p p"
            "p      p"
            "        "
            "QQQQQQQQ"
            "kpkpkpkp"
            "        "
            "p p r b "
            "       R";
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R w KQkq - 0 1");

  f.white_castle_short = false;
  f.white_castle_long = false;
  f.black_castle_short = false;
  f.black_castle_long = false;
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R w - - 0 1");

  f.en_passant = "a3";
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R w - a3 0 1");

  f.player = true;
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b - a3 0 1");

  f.halfmove = 5;
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b - a3 5 1");

  f.move = 5;
  REQUIRE(FENParser::Serialize(f) ==
          "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b - a3 5 5");
}

TEST_CASE("Parse", "[fen/parse]") {
  FEN f;
  f = FENParser::Parse(
      "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b - a3 5 5");

  REQUIRE(f.board == "p    p p"
                     "p      p"
                     "        "
                     "QQQQQQQQ"
                     "kpkpkpkp"
                     "        "
                     "p p r b "
                     "       R");
  CHECK(f.white_castle_short == false);
  CHECK(f.white_castle_long == false);
  CHECK(f.black_castle_short == false);
  CHECK(f.black_castle_long == false);
  CHECK(f.move == 5);
  CHECK(f.halfmove == 5);
  CHECK(f.player == true);

  f = FENParser::Parse(
      "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b k a3 5 5");
  CHECK(f.white_castle_short == false);
  CHECK(f.white_castle_long == false);
  CHECK(f.black_castle_short == true);
  CHECK(f.black_castle_long == false);

  f = FENParser::Parse(
      "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b qk a3 5 5");
  CHECK(f.white_castle_short == false);
  CHECK(f.white_castle_long == false);
  CHECK(f.black_castle_short == true);
  CHECK(f.black_castle_long == true);

  f = FENParser::Parse(
      "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b Kqk a3 5 5");
  CHECK(f.white_castle_short == true);
  CHECK(f.white_castle_long == false);
  CHECK(f.black_castle_short == true);
  CHECK(f.black_castle_long == true);

  f = FENParser::Parse(
      "p4p1p/p6p/8/QQQQQQQQ/kpkpkpkp/8/p1p1r1b1/7R b KQqk a3 5 5");
  CHECK(f.white_castle_short == true);
  CHECK(f.white_castle_long == true);
  CHECK(f.black_castle_short == true);
  CHECK(f.black_castle_long == true);
}
