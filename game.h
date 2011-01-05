#ifndef GAME_H
#define GAME_H

#include <vector>

enum Player
{
  PlayerNone = -1,
  Player1 = 0,
  Player2 = 1,
  Player3 = 2,
  Player4 = 3,
  Player5 = 4,
  Player6 = 5,
  Player7 = 6,
  Player8 = 7
};

class GameException : public std::exception
{
public:
  GameException(const char* what)
    : std::exception(what) {}
};

class InvalidMoveGameException : public GameException
{
public:
  InvalidMoveGameException(const char* what)
    : GameException(what) {}
};

template<typename MOVE>
class GameState
{
public:
  virtual ~GameState() {}
  virtual std::vector<MOVE> GetPossibleMoves() const = 0;
  virtual void MakeMove(Player p, MOVE m) = 0;
  Player GetTurn();

protected:
  Player turn;
};

/////////////////////////////
// Template Implementation
/////////////////////////////

template<typename MOVE>
inline Player GameState<MOVE>::GetTurn()
{
  return turn;
}

#endif // GAME_H
