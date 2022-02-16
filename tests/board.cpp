#include "Board.hpp"
#include <catch_amalgamated.hpp>

using namespace chessarbiter;

TEST_CASE("IsEmpty/AddPiece", "[board/IsEmpty/AddPiece]") {
  Board b;
  b.AddPiece('K', "a1");
  b.AddPiece('K', "a8");
  b.AddPiece('K', "c8");

  CHECK_FALSE(b.IsEmpty("a1"));
  CHECK_FALSE(b.IsEmpty("a8"));
  CHECK_FALSE(b.IsEmpty("c8"));

  CHECK(b.IsEmpty("a2"));
  CHECK(b.IsEmpty("a3"));
  CHECK(b.IsEmpty("a4"));
}

TEST_CASE("Clear", "[board/Clear]") {
  Board b;
  b.AddPiece('K', "a1");
  b.AddPiece('A', "a8");
  b.AddPiece('Q', "c8");

  b.Clear();

  CHECK(b.IsEmpty("a1"));
  CHECK(b.IsEmpty("a8"));
  CHECK(b.IsEmpty("c8"));
}

TEST_CASE("GetPieceAt", "[board/GetPieceAt]") {
  Board b;
  b.AddPiece('K', "a1");
  b.AddPiece('A', "a8");
  b.AddPiece('Q', "c8");

  CHECK((b.GetPieceAt("a1")).piece == 'K');
  CHECK((b.GetPieceAt("a8")).piece == 'A');
  CHECK((b.GetPieceAt("c8")).piece == 'Q');

  CHECK_THROWS_AS(b.GetPieceAt("a5"), chessarbiter::NoPieceFound);
}

TEST_CASE("CountPiece", "[board/CountPiece]") {
  Board b;
  b.AddPiece('K', "a1");
  b.AddPiece('K', "a3");

  CHECK(b.CountPiece('K') == 2);

  b.AddPiece('k', "b1");
  b.AddPiece('k', "b5");

  CHECK(b.CountPiece('k') == 2);

  b.AddPiece('b', "h8");
  b.AddPiece('b', "h7");

  CHECK(b.CountPiece('b') == 2);
  CHECK(b.CountPiece('p') == 0);
}

TEST_CASE("GetKingLocation", "[board/GetKingLocation]") {
  Board b;

  b.AddPiece('k', "a1");
  CHECK(b.GetKingLocation(true) == "a1");

  b.AddPiece('K', "h1");
  CHECK(b.GetKingLocation(false) == "h1");

  b.Clear();
  CHECK_THROWS_AS(b.GetKingLocation(true), NoPieceFound);
}

TEST_CASE("RemovePiece", "[board/RemovePiece]") {
  Board b;

  b.AddPiece('k', "a1");
  b.AddPiece('p', "a2");

  REQUIRE(!b.IsEmpty("a1"));
  REQUIRE(!b.IsEmpty("a2"));

  b.RemovePiece("a1");

  REQUIRE(b.IsEmpty("a1"));
  REQUIRE(!b.IsEmpty("a2"));
}

TEST_CASE("Move", "[board/Move]") {
  Board b;

  b.AddPiece('k', "a1");
  b.AddPiece('p', "a2");

  REQUIRE(!b.IsEmpty("a1"));
  REQUIRE(!b.IsEmpty("a2"));

  // Nothing should happend since no piece on a3 (not even a throw)
  CHECK_NOTHROW(b.Move("a3c4"));
  REQUIRE(!b.IsEmpty("a1"));
  REQUIRE(!b.IsEmpty("a2"));

  b.Move("a1a2");
  REQUIRE(b.IsEmpty("a1"));
  REQUIRE(!b.IsEmpty("a2"));

  b.Move("a2h8");
  REQUIRE(b.IsEmpty("a2"));
  REQUIRE(!b.IsEmpty("h8"));
}

TEST_CASE("GetPlayerPieces", "[board/GetPlayerPieces]") {
  Board b;
  b.AddPiece('P', "a2");
  b.AddPiece('B', "b2");
  b.AddPiece('k', "c2");

  std::vector<Piece> pieces = b.GetPlayerPieces(false);
  REQUIRE(pieces.size() == 2);
  REQUIRE(b.GetPlayerPieces(true).size() == 1);

  bool found = false;
  for (Piece &p : pieces) {
    if (p.coord == "a2" && p.piece == 'P') {
      found = true;
    }
  }
  CHECK(found);
  found = false;

  for (Piece &p : pieces) {
    if (p.coord == "b2" && p.piece == 'B') {
      found = true;
    }
  }
  CHECK(found);
  found = false;
}

TEST_CASE("IsMovePossible", "[board/IsMovePossible]") {
  Board b;
  b.AddPiece('P', "a2");

  // Pawn white
  CHECK(b.IsMovePossible("a2a3"));
  CHECK_FALSE(b.IsMovePossible("a2a1")); // Can't go backward
  CHECK(b.IsMovePossible(
      "a2b3")); // Not that Board is not in charge of pawn singularities
  CHECK(b.IsMovePossible("a2a4"));
  b.AddPiece('p', "a3");
  CHECK_FALSE(b.IsMovePossible("a2a4")); // Now we can't
  b.AddPiece('P', "b3");
  CHECK_FALSE(b.IsMovePossible("a2b3")); // Now we can't
  b.Clear();
  b.AddPiece('P', "a2");
  b.AddPiece('p', "b3");           // Opposite color!
  CHECK(b.IsMovePossible("a2b3")); // Now we can

  // Pawn black
  b.Clear();
  b.AddPiece('p', "a7");
  CHECK(b.IsMovePossible("a7a6"));
  CHECK_FALSE(b.IsMovePossible("a7a8")); // Can't go backward
  CHECK(b.IsMovePossible(
      "a7b6")); // Not that Board is not in charge of pawn singularities
  CHECK(b.IsMovePossible("a7a5"));
  b.AddPiece('p', "a6");
  CHECK_FALSE(b.IsMovePossible("a7a5")); // Now we can't
  b.AddPiece('p', "b6");
  CHECK_FALSE(b.IsMovePossible("a7b6")); // Now we can't
  b.Clear();
  b.AddPiece('p', "a7");
  b.AddPiece('P', "b6");           // Opposite color!
  CHECK(b.IsMovePossible("a7b6")); // Now we can

  // Bishop
  b.Clear();
  b.AddPiece('b', "d5");
  CHECK(b.IsMovePossible("d5h1"));
  CHECK(b.IsMovePossible("d5a8"));
  CHECK(b.IsMovePossible("d5a2"));
  CHECK(b.IsMovePossible("d5g8"));
  b.AddPiece('q', "b7"); // We block the piece in every directions
  b.AddPiece('q', "g2");
  b.AddPiece('q', "b3");
  b.AddPiece('q', "f7");
  CHECK_FALSE(b.IsMovePossible("d5h1"));
  CHECK_FALSE(b.IsMovePossible("d5a8"));
  CHECK_FALSE(b.IsMovePossible("d5a2"));
  CHECK_FALSE(b.IsMovePossible("d5g8"));
  // No rook moves
  CHECK_FALSE(b.IsMovePossible("d5a5"));
  CHECK_FALSE(b.IsMovePossible("d5h5"));
  CHECK_FALSE(b.IsMovePossible("d5d8"));
  CHECK_FALSE(b.IsMovePossible("d5d1"));

  // Rook
  b.Clear();
  b.AddPiece('r', "d5");
  CHECK(b.IsMovePossible("d5a5"));
  CHECK(b.IsMovePossible("d5h5"));
  CHECK(b.IsMovePossible("d5d8"));
  CHECK(b.IsMovePossible("d5d1"));
  b.AddPiece('q', "b5"); // We block the piece in every directions
  b.AddPiece('q', "g5");
  b.AddPiece('q', "d7");
  b.AddPiece('q', "d2");
  CHECK_FALSE(b.IsMovePossible("d5a5"));
  CHECK_FALSE(b.IsMovePossible("d5h5"));
  CHECK_FALSE(b.IsMovePossible("d5d8"));
  CHECK_FALSE(b.IsMovePossible("d5d1"));
  // No bishop moves
  CHECK_FALSE(b.IsMovePossible("d5h1"));
  CHECK_FALSE(b.IsMovePossible("d5a8"));
  CHECK_FALSE(b.IsMovePossible("d5a2"));
  CHECK_FALSE(b.IsMovePossible("d5g8"));

  // Queen like bishop
  b.Clear();
  b.AddPiece('q', "d5");
  CHECK(b.IsMovePossible("d5h1"));
  CHECK(b.IsMovePossible("d5a8"));
  CHECK(b.IsMovePossible("d5a2"));
  CHECK(b.IsMovePossible("d5g8"));
  b.AddPiece('q', "b7"); // We block the piece in every directions
  b.AddPiece('q', "g2");
  b.AddPiece('q', "b3");
  b.AddPiece('q', "f7");
  CHECK_FALSE(b.IsMovePossible("d5h1"));
  CHECK_FALSE(b.IsMovePossible("d5a8"));
  CHECK_FALSE(b.IsMovePossible("d5a2"));
  CHECK_FALSE(b.IsMovePossible("d5g8"));

  // Queen like rook
  b.Clear();
  b.AddPiece('q', "d5");
  CHECK(b.IsMovePossible("d5a5"));
  CHECK(b.IsMovePossible("d5h5"));
  CHECK(b.IsMovePossible("d5d8"));
  CHECK(b.IsMovePossible("d5d1"));
  b.AddPiece('q', "b5"); // We block the piece in every directions
  b.AddPiece('q', "g5");
  b.AddPiece('q', "d7");
  b.AddPiece('q', "d2");
  CHECK_FALSE(b.IsMovePossible("d5a5"));
  CHECK_FALSE(b.IsMovePossible("d5h5"));
  CHECK_FALSE(b.IsMovePossible("d5d8"));
  CHECK_FALSE(b.IsMovePossible("d5d1"));

  // Knight
  b.Clear();
  b.AddPiece('n', "d5");
  CHECK(b.IsMovePossible("d5e7"));
  CHECK(b.IsMovePossible("d5c7"));
  CHECK(b.IsMovePossible("d5b6"));
  CHECK(b.IsMovePossible("d5b4"));
  CHECK(b.IsMovePossible("d5c3"));
  CHECK(b.IsMovePossible("d5e3"));
  CHECK(b.IsMovePossible("d5f4"));
  CHECK(b.IsMovePossible("d5f6"));
  // Not possible
  CHECK_FALSE(b.IsMovePossible("d5d7"));
  CHECK_FALSE(b.IsMovePossible("d5d3"));
  CHECK_FALSE(b.IsMovePossible("d5b5"));
  CHECK_FALSE(b.IsMovePossible("d5f5"));

  // King
  b.Clear();
  b.AddPiece('k', "d5");
  CHECK(b.IsMovePossible("d5d6"));
  CHECK(b.IsMovePossible("d5c6"));
  CHECK(b.IsMovePossible("d5e6"));
  CHECK(b.IsMovePossible("d5c5"));
  CHECK(b.IsMovePossible("d5e5"));
  CHECK(b.IsMovePossible("d5d4"));
  CHECK(b.IsMovePossible("d5c4"));
  CHECK(b.IsMovePossible("d5e4"));

  // Moving a piece at the same place is not possible
  CHECK_FALSE(b.IsMovePossible("d5d5"));
}

TEST_CASE("ListPossibleMoves", "[board/ListPossibleMoves]") {
  Board b;
  b.AddPiece('K', "a1");
  b.AddPiece('K', "a8");
  b.AddPiece('K', "h1");
  b.AddPiece('K', "h8");
  b.AddPiece('k', "d5"); // Black piece

  std::vector<std::string> moves = b.ListPossibleMoves(false);
  REQUIRE(moves.size() == 12);
  CHECK(std::find(moves.begin(), moves.end(), "a1a2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a1b2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a1b1") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8a7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8b8") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8b7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8g7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8g8") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8h7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1h2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1g1") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1g2") != moves.end());

  b.Clear();
  b.AddPiece('k', "a1");
  b.AddPiece('k', "a8");
  b.AddPiece('k', "h1");
  b.AddPiece('k', "h8");
  b.AddPiece('K', "d5"); // White piece

  moves = b.ListPossibleMoves(true);
  REQUIRE(moves.size() == 12);
  CHECK(std::find(moves.begin(), moves.end(), "a1a2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a1b2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a1b1") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8a7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8b8") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "a8b7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8g7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8g8") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h8h7") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1h2") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1g1") != moves.end());
  CHECK(std::find(moves.begin(), moves.end(), "h1g2") != moves.end());
}

TEST_CASE("Serialize", "[board/Serialize]") {
  Board b;
  b.AddPiece('p', "a8");
  CHECK(b.Serialize() == "p       "
                         "        "
                         "        "
                         "        "
                         "        "
                         "        "
                         "        "
                         "        ");
  b.AddPiece('P', "a1");
  CHECK(b.Serialize() == "p       "
                         "        "
                         "        "
                         "        "
                         "        "
                         "        "
                         "        "
                         "P       ");
  b.AddPiece('k', "d5");
  CHECK(b.Serialize() == "p       "
                         "        "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         "        "
                         "P       ");
  b.AddPiece('Q', "d7");
  CHECK(b.Serialize() == "p       "
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         "        "
                         "P       ");
  b.AddPiece('p', "h2");
  CHECK(b.Serialize() == "p       "
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         "       p"
                         "P       ");
  b.AddPiece('k', "h8");
  CHECK(b.Serialize() == "p      k"
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         "       p"
                         "P       ");
  b.AddPiece('N', "b2");
  CHECK(b.Serialize() == "p      k"
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         " N     p"
                         "P       ");
  b.AddPiece('n', "c2");
  CHECK(b.Serialize() == "p      k"
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "        "
                         " Nn    p"
                         "P       ");
  b.AddPiece('b', "c3");
  CHECK(b.Serialize() == "p      k"
                         "   Q    "
                         "        "
                         "   k    "
                         "        "
                         "  b     "
                         " Nn    p"
                         "P       ");
}

TEST_CASE("IsPieceMoveUnique", "[board/IsPieceMoveUnique]") {
  Board b;
  b.AddPiece('N', "a1");
  b.AddPiece('n', "c1");

  CHECK(b.IsPieceMoveUnique('n', "b3"));
  CHECK(b.IsPieceMoveUnique('N', "b3"));

  b.AddPiece('N', "d2");
  CHECK(b.IsPieceMoveUnique('n', "b3"));
  CHECK_FALSE(b.IsPieceMoveUnique('N', "b3"));
}
