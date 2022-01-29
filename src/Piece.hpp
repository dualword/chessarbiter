#include <string>
#include <vector>

#define PIECE__IS_VALID(f, r) ((f) >= 'a' && (f) <= 'h' && (r) >= '1' && (r) <= '8')
#define PIECE__ADD_MOVE(f, r)                                                         \
  {                                                                            \
    if (PIECE__IS_VALID(f, r)) {                                                      \
      moves.push_back(std::string() + (char)((f)) + (char)((r)));              \
    }                                                                          \
  }

namespace chessarbiter {

/**
 * Member are public for conveniance
 */
class Piece {
public:
  bool isBlack;
  std::string coord;
  char piece;
  Piece(char c, std::string coord);
  /// @brief Get all possible moves according to the type of piece and its position
  std::vector<std::string> GetMoves();
};

} // namespace chessarbiter