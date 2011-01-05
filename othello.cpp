#include "othello.h"

#include <iostream>

using namespace std;

const Vec flankdirs[8] = {Vec(1,0),Vec(-1,0),Vec(0,1),Vec(0,-1),Vec(1,1),Vec(-1,-1),Vec(-1,1),Vec(1,-1)};

OthelloGameState::OthelloGameState()
{
  ResetBoard();
}

OthelloGameState::~OthelloGameState()
{
}

std::vector<OthelloMove> OthelloGameState::GetPossibleMoves() const
{
  std::vector<OthelloMove> moves;
  return moves;
}

bool OthelloGameState::ValidCell(Vec m) const
{
  return 0 <= m.r && m.r < width && 0 <= m.c && m.c < width;
}

int OthelloGameState::CountSequence(Vec c, Vec dir, Player p) const
{
  if( !ValidCell(c) || GetCell(c) != PlayerNone )
    return 0;

  int count = 0;

  for(int count = 0;; ++count)
  {
    c = c + dir;

    if( !ValidCell(c) )
      return 0;

    Player cellp = GetCell(c);

    if( cellp == p)
      return count;
    else if( cellp == PlayerNone )
      return 0;
  }

  return count;
}

bool OthelloGameState::CanMove(Player p, OthelloMove m) const
{
  if(GetCell(m) == PlayerNone )
  {
    for( int i=0; i<8; ++i )
      if( CountSequence(m,flankdirs[i],p) > 0 )
        return true;
  }

  return false;
}

bool OthelloGameState::CanMove(Player p) const
{
  if( turn == p )
    for( int r=0; r < width; ++r )
      for( int c=0; c < width; ++c )
        if( CanMove(p,OthelloMove(r,c) ) )
          return true;

  return false;
}

bool OthelloGameState::FlankDirection(Vec c, Vec dir, Player p)
{
  // Count only >0 if valid flanking move for player p
  const int count = CountSequence(c,dir,p);

  if( count )
  {
    for(;;)
    {
      c = c + dir;
      Player cellp = GetCell(c);
      if( cellp == p)
        return true;
      SetCell(c,p);
    }
  }
  return false;
}

void OthelloGameState::MakeMove(Player p, OthelloMove m)
{
  if(turn != p)
    throw InvalidMoveGameException("Not this players turn");
  
  if(GetCell(m.r,m.c) != PlayerNone)
    throw InvalidMoveGameException("Cell not empty");

  if( !ValidCell(m) )
    throw InvalidMoveGameException("Invalid Cell");

  // Find flanks
  bool valid = false;

  for( int i=0; i<8; ++i )
  {
    valid |= FlankDirection(m,flankdirs[i],p);
  }

  if( !valid )
    throw InvalidMoveGameException("Move does not flank opposition");

  SetCell(m.r,m.c,p);

  turn = (Player)(1 - p);
  if( !CanMove(turn) )
  {
    cout << "Player " << turn+1 << " cannot move. Skipped" << endl;

    turn = (Player)(1 - p);
    if( !CanMove(turn) )
    {
      // Game over
      cout << "Game Over" << endl;
      cout << "Player 1: " << player_tokens[1] << endl;
      cout << "Player 2: " << player_tokens[2] << endl;
    }
  }
}

void OthelloGameState::ResetBoard()
{
  for( int i=0; i<num_cells; ++i )
    cells[i] = PlayerNone;

  tokens_placed = 0;
  player_tokens[0] = num_cells;
  player_tokens[1] = 0;
  player_tokens[2] = 0;

  SetCell(3,3, Player1);
  SetCell(4,4, Player1);
  SetCell(3,4, Player2);
  SetCell(4,3, Player2);

  turn = Player1;
}

Player OthelloGameState::GetCell(int r, int c) const
{
  return cells[r*width + c];
}

Player OthelloGameState::GetCell(Vec v) const
{
  return GetCell(v.r,v.c);
}

void OthelloGameState::SetCell(int r, int c, Player p)
{
  Player& cp = cells[r*width + c];
  player_tokens[cp + 1]--;
  player_tokens[p  + 1]++;
  cp = p;
}

void OthelloGameState::SetCell(Vec v, Player p)
{
  SetCell(v.r,v.c,p);
}
