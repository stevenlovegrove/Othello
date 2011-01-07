#include <iostream>
#include <math.h>

#include <GL/freeglut.h>
#include "othello.h"

using namespace std;

float colour_board[4] = {0.199f,0.53125f,0.199f,1};
float colour_grid[4] = {0,0,0,1};
float colour_p1[4] = {1,1,1,1};
float colour_p2[4] = {0,0,0,1};
float colour_hint[4] = {1,0,0,0.2f};

int win_w;
int win_h;

bool show_moves = false;

OthelloGameState current_game;

void DrawCircle(float radius)
{
  const float pi = 3.14159f;
  const float in = pi / 10.0f;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(0,0);
  for (float i=0; i <= 2*pi; i+=in)
    glVertex2f(cos(i)*radius,sin(i)*radius);
  glEnd();
}

void DrawOthello()
{
  glClearColor(colour_board[0],colour_board[1],colour_board[2],colour_board[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  const float s = 1.0f / OthelloGameState::width;
  const float radius = s / 2.5f;

  glColor4fv(colour_grid);

  // Draw grid lines
  glBegin(GL_LINES);
  for( int i=0; i<=OthelloGameState::width; ++i )
  {
    glVertex2d(0.0,i*s);
    glVertex2d(1.0,i*s);
    glVertex2d(i*s,0.0);
    glVertex2d(i*s,1.0);
  }
  glEnd();

  glTranslated(s/2.0,s/2.0,0);

  Player cp = current_game.GetTurn();

  // Draw cells
  for( int r=0; r<OthelloGameState::width; ++r )
  {
    for( int c=0; c<OthelloGameState::width; ++c )
    {
      Vec m(r,c);
      Player p = current_game.GetCell(m);
      if( p != PlayerNone)
      {
        glColor4fv( p == Player1 ? colour_p1 : colour_p2 );
        DrawCircle(radius);
      }else{
        if( show_moves && current_game.CanMove(cp,m) )
        {
          glColor4fv(colour_hint);
          DrawCircle(radius);
        }
      }
      glTranslated(s,0,0);
    }
    glTranslated(-1,s,0);
  }
  glPopMatrix();
}

void Display()
{
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  //gluLookAt(0, 0, -10.0, 0, 0, 0, 0, 1, 0);
  //glutWireTeapot(1.0);
  DrawOthello();

  glutSwapBuffers();
}
void Reshape(GLint width, GLint height)
{
  win_w = width;
  win_h = height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,1,0,1);
  //gluPerspective(65.0, (float)width / height, 0.1, 1000);
  glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
	  exit (0);
	  break;
  case ' ':
    show_moves = !show_moves;
    glutPostRedisplay();
    break;
  case 'r':
    current_game.ResetBoard();
    glutPostRedisplay();
    break;
  }
}

void Mouse(int button, int state, int x, int y)
{
  if( state )
  {
    int r = (int)(OthelloGameState::width * (float)(win_h-y) / (float)win_h);
    int c = (int)(OthelloGameState::width * (float)x / (float)win_w);
    OthelloMove m(r,c);
    try {
      current_game.MakeMove(current_game.GetTurn(),m);
    }catch(GameException e)
    {
      cout << e.what() << endl;
    }
  }
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  // GLUT Window Initialization:
  glutInit(&argc, argv);
  glutInitWindowSize(640, 640);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Othello");
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);

  // Enable Alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutMainLoop();
  return 0;
}
