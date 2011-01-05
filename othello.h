#ifndef OTHELLO_H
#define OTHELLO_H

#include "game.h"

class Vec
{
public:
  Vec(int r, int c) : r(r),c(c) {}
  int r;
  int c;
};

inline Vec operator+(const Vec& lhs, const Vec& rhs)
{
  return Vec(lhs.r+rhs.r, lhs.c+rhs.c);
}

typedef Vec OthelloMove;

class OthelloGameState : public GameState<OthelloMove>
{
public:
  static const int width = 8;
  static const int num_cells = width*width;

  OthelloGameState();
  ~OthelloGameState();

  void ResetBoard();
  bool ValidCell(Vec v) const;
  Player GetCell(int r, int c) const;
  Player GetCell(Vec v) const;

  std::vector<OthelloMove> GetPossibleMoves() const;
  void MakeMove(Player p, OthelloMove m);

  bool CanMove(Player p) const;
  bool CanMove(Player p, OthelloMove m) const;

protected:
  void SetCell(int r, int c, Player p);
  void SetCell(Vec v, Player p);

  int CountSequence(Vec center, Vec dir, Player p) const;
  bool FlankDirection(Vec center, Vec dir, Player p);

  Player cells[num_cells];
  int tokens_placed;
  int player_tokens[3];
};

#endif // OTHELLO_H
